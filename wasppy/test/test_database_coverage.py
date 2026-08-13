"""
Additional unittest coverage for Database.py.

These tests avoid the real WASP parser by providing a tiny in-memory node and
interpreter. They focus on Database.py behavior that is otherwise hard to reach
from generated parser fixtures alone.

"""

import importlib.util
import pathlib
import sys
import types
import unittest


class FakeNode:
    def __init__(self, name, text=None, children=None, decorative=False):
        self._name = name
        self._text = str(text if text is not None else name)
        self._children = list(children or [])
        self._decorative = decorative

    def name(self):
        return self._name

    def isDecorative(self):
        return self._decorative

    def child_count(self):
        return len(self._children)

    def info(self):
        return f"{self._name}={self._text}"

    def __iter__(self):
        return iter(self._children)

    def __len__(self):
        return len(self._children)

    def __str__(self):
        return self._text


class FakeInterpreter:
    def __init__(self):
        self.errors = []

    def createErrorDiagnostic(self, node, message):
        self.errors.append((node.name() if node is not None else None, message))

    def messages(self):
        return [message for _, message in self.errors]


def leaf(name, text):
    return FakeNode(name, text)


def parent(name, *children):
    return FakeNode(name, name, children)


class DatabaseCoverageTest(unittest.TestCase):
    def make_result(self, name, definition, interpreter=None, value=None):
        '''Convenience method to assist in creating fake interpreter trees'''
        db = self.db

        if interpreter is None:
            interpreter = FakeInterpreter()

        result = db.DeserializedResult(FakeNode(name, value), interpreter)
        result.definition = definition
        result.parent = None
        return result
    @classmethod
    def setUpClass(cls):
        fake_wasp = types.ModuleType("wasp")
        fake_wasp.WaspNode = FakeNode
        fake_wasp.Interpreter = FakeInterpreter
        sys.modules.setdefault("wasp", fake_wasp)

        path = pathlib.Path(__file__).parent.with_name("Database.py")
        spec = importlib.util.spec_from_file_location("Database_under_test", path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        cls.db = module

    # ------------------------- DeserializedResult coverage ---------------------

    def test_from_default_scalar_and_list_shapes(self):
        db = self.db
        interp = FakeInterpreter()
        scalar = db.DeserializedResult.fromDefault(FakeNode("root"), interp, 3.5)
        vector = db.DeserializedResult.fromDefault(FakeNode("root"), interp, ["a", "b"])

        self.assertTrue(scalar.hasValue())
        self.assertEqual(scalar.value(), 3.5)
        self.assertEqual(vector.valuelist(), ["a", "b"])
        self.assertEqual(scalar.todict(), {"value": {":=": 3.5}})
        self.assertEqual(vector.todict(), {"value": [{":=": "a"}, {":=": "b"}]})

    def test_select_wildcard_preserves_user_input_order(self):
        db = self.db
        interp = FakeInterpreter()
        root = db.DeserializedResult(FakeNode("root"), interp)
        first = db.DeserializedResult(FakeNode("item", "first"), interp)
        first.store("first")
        second = db.DeserializedResult(FakeNode("item", "second"), interp)
        second.store("second")
        other = db.DeserializedResult(FakeNode("other", "third"), interp)
        other.store("third")

        root.userData["item"] = [first, second]
        root.userData["other"] = other

        self.assertEqual([r.storedResult() for r in root.select("*")], ["first", "second", "third"])

    def test_store_multiple_values_should_remain_flat_list(self):
        db = self.db
        dr = db.DeserializedResult(FakeNode("value"), FakeInterpreter())
        dr.store("a")
        dr.store("b")
        dr.store("c")

        self.assertEqual(dr.storedResult(), ["a", "b", "c"])

    def test_len_counts_falsy_stored_scalar_values(self):
        db = self.db
        dr = db.DeserializedResult(FakeNode("value"), FakeInterpreter())
        dr.store(0)
        self.assertEqual(len(dr), 1)

    # ----------------------------- InputObject coverage ------------------------

    def test_unknown_and_decorative_children(self):
        db = self.db
        schema = db.InputObject()
        schema.create("known", Action=db.storeStr)
        interp = FakeInterpreter()

        schema.deserialize(parent("root", leaf("known", "ok"), leaf("comment", "ignored"), leaf("bad", "no")), interp)
        self.assertTrue(any("unknown key" in message for message in interp.messages()))

        interp = FakeInterpreter()
        schema.deserialize(parent("root", leaf("known", "ok"), FakeNode("comment", "ignored", decorative=True)), interp)
        self.assertEqual(interp.errors, [])

    def test_required_single_min_max_and_default(self):
        db = self.db
        schema = db.InputObject()
        schema.create("required", MinOccurs=1, MaxOccurs=1, Action=db.storeStr)
        schema.create("with_default", Default=[1, 2], Action=db.storeInt)
        interp = FakeInterpreter()

        result = schema.deserialize(parent("root", leaf("required", "one"), leaf("required", "two")), interp)

        messages = "\n".join(interp.messages())
        self.assertIn("occurrence exceeds maximum allowed occurrence of 1", messages)
        self.assertEqual(result["with_default"].valuelist(), [1, 2])

    def test_input_value_preserves_falsey_defaults(self):
        db = self.db
        for default in (0, False, ""):
            with self.subTest(default=default):
                schema = db.InputObject()
                schema.create("value_with_default", Default=default) \
                    .create("value", Action=db.storeInt)

                self.assertEqual(schema.inputValue("value_with_default"), default)

    def test_input_value_default_precedes_enumerations(self):
        db = self.db
        schema = db.InputObject()
        schema.create("choice", Default="explicit") \
            .create("value", Enums=["first", "second"], Action=db.storeStr)

        self.assertEqual(schema.inputValue("choice"), "explicit")

    def test_input_value_uses_first_enumeration_without_default(self):
        db = self.db
        schema = db.InputObject()
        schema.create("choice") \
            .create("value", Enums=["first", "second"], Action=db.storeStr)

        self.assertEqual(schema.inputValue("choice"), "first")

    def test_input_value_uses_type_placeholders(self):
        db = self.db
        schema = db.InputObject()
        expected_values = {
            "string_value": (db.storeStr, "insert_string_here"),
            "integer_value": (db.storeInt, "1"),
            "float_value": (db.storeFloat, "0.0"),
        }
        for key, (action, _) in expected_values.items():
            schema.create(key).create("value", Action=action)

        for key, (_, expected) in expected_values.items():
            with self.subTest(key=key):
                self.assertEqual(schema.inputValue(key), expected)

    def test_input_value_uses_generic_fallback_without_value_metadata(self):
        db = self.db
        schema = db.InputObject()
        schema.create("untyped").create("value")

        self.assertEqual(schema.inputValue("untyped"), "0")

    def test_input_template_and_type_metadata(self):
        db = self.db
        schema = db.InputObject(InputTmpl="document", InputType="container")
        child = schema.create(
            "choice", InputTmpl="choice_template", InputType="palette"
        )
        sibling = schema.create(
            "option", InputVars=["option_one", "option_two"]
        )

        self.assertEqual(schema.inputTmpl(), "document")
        self.assertEqual(schema.inputType(), "container")
        self.assertEqual(child.inputTmpl(), "choice_template")
        self.assertEqual(child.inputType(), "palette")
        self.assertEqual(None, sibling.inputTmpl())
        self.assertEqual(None, sibling.inputType())
        self.assertEqual(["option_one", "option_two"], sibling.inputVars())

    def test_missing_required_child_diagnostic(self):
        db = self.db
        schema = db.InputObject()
        schema.create("required", MinOccurs=1, Action=db.storeStr)
        interp = FakeInterpreter()

        schema.deserialize(parent("root"), interp)

        self.assertTrue(any("occurrences of required when 1 are required" in message for message in interp.messages()))

    def test_enums_and_numeric_bounds_cover_all_bound_types(self):
        db = self.db
        schema = db.InputObject()
        schema.create("enum", Action=db.storeStr, Enums=["a", "b"])
        schema.create("min_inc", Action=db.storeFloat, MinValInc=1.0)
        schema.create("max_inc", Action=db.storeFloat, MaxValInc=10.0)
        schema.create("min_exc", Action=db.storeFloat, MinValExc=1.0)
        schema.create("max_exc", Action=db.storeFloat, MaxValExc=10.0)
        interp = FakeInterpreter()

        schema.deserialize(
            parent(
                "root",
                leaf("enum", "c"),
                leaf("min_inc", "0.9"),
                leaf("max_inc", "10.1"),
                leaf("min_exc", "1.0"),
                leaf("max_exc", "10.0"),
            ),
            interp,
        )

        messages = "\n".join(interp.messages())
        self.assertIn("not one of the allowed values", messages)
        self.assertIn("less than the allowed minimum inclusive", messages)
        self.assertIn("greater than the allowed maximum inclusive", messages)
        self.assertIn("less than or equal to the allowed minimum exclusive", messages)
        self.assertIn("greater than or equal to the allowed maximum exclusive", messages)

    def test_store_int_and_float_conversion_errors(self):
        db = self.db
        schema = db.InputObject()
        schema.create("int_value", Action=db.storeInt)
        schema.create("float_value", Action=db.storeFloat)
        interp = FakeInterpreter()

        schema.deserialize(parent("root", leaf("int_value", "x"), leaf("float_value", "y")), interp)

        messages = "\n".join(interp.messages())
        self.assertIn("expected to be an integer", messages)
        self.assertIn("expected to be a float", messages)

    # ---------------------------- constraint coverage -------------------------

    def test_unique_constraint_reports_both_duplicate_nodes(self):
        db = self.db
        schema = db.InputObject()
        schema.create("a", Action=db.storeStr)
        schema.create("b", Action=db.storeStr)
        schema.addUniqueConstraint(["a", "b"])
        interp = FakeInterpreter()

        schema.deserialize(parent("root", leaf("a", "same"), leaf("b", "same")), interp)

        self.assertEqual(sum("must be unique" in message for message in interp.messages()), 2)

    def test_exists_constraint_with_discrete_and_missing_target(self):
        db = self.db
        schema = db.InputObject()
        schema.create("use", Action=db.storeStr)
        schema.create("define", Action=db.storeStr)
        schema.addExistsConstraint(db.ExistsConstraint(["use"], target=["define"], discrete=["builtin"]))

        ok_interp = FakeInterpreter()
        schema.deserialize(parent("root", leaf("use", "builtin")), ok_interp)
        self.assertEqual(ok_interp.errors, [])

        bad_interp = FakeInterpreter()
        schema.deserialize(parent("root", leaf("use", "missing"), leaf("define", "known")), bad_interp)
        self.assertTrue(any("required existing targets" in message for message in bad_interp.messages()))

    def test_exists_constraint_discrete_lookup(self):
        db = self.db
        root = db.InputObject()
        root.create("source", Action=db.storeStr)
        root.create("target_a", Action=db.storeStr)
        root.create("target_b", Action=db.storeStr)
        root.addExistsConstraint(
            db.ExistsConstraint(
                ["source"], target=["target_a"], discrete=["builtin_a"]
            )
        )
        root.addExistsConstraint(
            db.ExistsConstraint(
                ["source"], target=["target_b"], discrete=["builtin_b"]
            )
        )

        child = root.create("child")
        child.create("source", Action=db.storeStr)
        child.create("target", Action=db.storeStr)
        child.addExistsConstraint(
            db.ExistsConstraint(
                ["source"], target=["target"], discrete=["nested_builtin"]
            )
        )

        root.create("plain_source", Action=db.storeStr)
        root.create("plain_target", Action=db.storeStr)
        root.addExistsConstraint(
            db.ExistsConstraint(["plain_source"], target=["plain_target"])
        )

        self.assertEqual(
            root.getExistsConstraintDiscretesGivenSource("source"),
            ["builtin_a", "builtin_b"],
        )
        self.assertEqual(
            root.getExistsConstraintDiscretesGivenSource("child/source"),
            ["nested_builtin"],
        )
        self.assertIsNone(
            root.getExistsConstraintDiscretesGivenSource("plain_source")
        )
        self.assertIsNone(root.getExistsConstraintDiscretesGivenSource("missing"))

    def test_at_most_at_least_exactly_constraints(self):
        db = self.db
        at_most = db.InputObject()
        at_most.create("a", Action=db.storeStr)
        at_most.create("b", Action=db.storeStr)
        at_most.addAtMostConstraint(db.SourcePredicatedTarget(target={"a": None, "b": None}, count=1))
        interp = FakeInterpreter()
        at_most.deserialize(parent("root", leaf("a", "1"), leaf("b", "2")), interp)
        self.assertTrue(any("more than 1" in message for message in interp.messages()))

        at_least = db.InputObject()
        at_least.create("a", Action=db.storeStr)
        at_least.create("b", Action=db.storeStr)
        at_least.addAtLeastConstraint(db.SourcePredicatedTarget(target={"a": None, "b": None}, count=2))
        interp = FakeInterpreter()
        at_least.deserialize(parent("root", leaf("a", "1")), interp)
        self.assertTrue(any("fewer than 2" in message for message in interp.messages()))

        exactly = db.InputObject()
        exactly.create("a", Action=db.storeStr)
        exactly.create("b", Action=db.storeStr)
        exactly.addExactlyConstraint(db.SourcePredicatedTarget(target={"a": None, "b": None}, count=1))
        interp = FakeInterpreter()
        exactly.deserialize(parent("root", leaf("a", "1"), leaf("b", "2")), interp)
        self.assertTrue(any("exactly 1" in message for message in interp.messages()))

    def test_source_predicated_constraint_skips_when_source_absent(self):
        db = self.db
        schema = db.InputObject()
        schema.create("source", Action=db.storeStr)
        schema.create("a", Action=db.storeStr)
        schema.create("b", Action=db.storeStr)
        schema.addExactlyConstraint(db.SourcePredicatedTarget(source="source", target={"a": None, "b": None}, count=2))
        interp = FakeInterpreter()

        schema.deserialize(parent("root", leaf("a", "1")), interp)

        self.assertEqual(interp.errors, [])

    def test_source_predicated_target_value_matching_and_predicated_strings(self):
        db = self.db
        root = db.DeserializedResult(FakeNode("root"), FakeInterpreter())
        child = db.DeserializedResult(FakeNode("choice", "YES"), FakeInterpreter())
        child.store("YES")
        root.userData["choice"] = [child]

        constraint = db.SourcePredicatedTarget(target={"choice": ["yes", "no"], "missing": None})

        self.assertEqual(list(constraint.targets(root)), ["choice=YES"])
        self.assertEqual(constraint.targets_predicated(), ["choice=yes", "choice=no", "missing"])

    def test_nested_exists_constraint_path_lookup(self):
        db = self.db
        root = db.InputObject()
        child = root.create("child")
        child.create("source", Action=db.storeStr)
        child.create("target", Action=db.storeStr)
        child.addExistsConstraint(db.ExistsConstraint(["source"], target=["target"]))

        self.assertEqual(root.getExistsConstraintTargetGivenSource("child/source"), ["child/target"])
        self.assertEqual(root.getExistsConstraintSourceGivenTarget("child/target"), ["child/source"])

    def test_serialize_includes_description_enums_and_occurs(self):
        db = self.db
        schema = db.InputObject(Desc="root object")
        schema.create("color", MinOccurs=1, MaxOccurs=1, Desc="allowed color", Enums=["red", "blue"])
        io = db.StringIO()

        schema.serialize(io)

        text = io.getvalue()
        self.assertIn("Description='root object'", text)
        self.assertIn("MinOccurs(color)=1", text)
        self.assertIn("MaxOccurs(color)=1", text)
        self.assertIn("Description='allowed color'", text)
        self.assertIn("ValueEnums[red blue]", text)

    def test_deserialized_exists_target_lookup_returns_scoped_result(self):
        """
        Verify that getExistsConstraintTargetLookups() returns the
        DeserializedResult corresponding to the scope in which the
        ExistsConstraint is declared.

        Schema
        ======

            root
            └── outer
                └── inner
                    ├── use_id
                    └── defined_id

            ExistsConstraint(
                source=["use_id"],
                target=["defined_id"]
            ) declared on 'inner'

        Deserialized Tree
        =================

            root
            └── outer
                └── inner
                    ├── use_id = "foo"      <-- lookup originates here
                    └── defined_id = "foo"

        Expected
        ========

        Looking up targets from 'use_id' should return a single
        ExistsConstraintLookup whose scope is the 'inner'
        DeserializedResult and whose target path is "defined_id".
        Selecting "defined_id" from that scope should resolve to the
        sibling node under 'inner'.
        """
        db = self.db
        interpreter = FakeInterpreter()

        root_def = db.InputObject()
        outer_def = root_def.create("outer")
        inner_def = outer_def.create("inner")

        use_id_def = inner_def.create("use_id", Action=db.storeStr)
        defined_id_def = inner_def.create("defined_id", Action=db.storeStr)

        constraint = db.ExistsConstraint(["use_id"], target=["defined_id"])
        inner_def.addExistsConstraint(constraint)

        root_dr = self.make_result("root", root_def, interpreter)
        outer_dr = self.make_result("outer", outer_def, interpreter)
        inner_dr = self.make_result("inner", inner_def, interpreter)
        use_id_dr = self.make_result("use_id", use_id_def, interpreter, "foo")
        defined_id_dr = self.make_result("defined_id", defined_id_def, interpreter, "foo")

        root_dr.addResult(outer_dr, root_def)
        outer_dr.addResult(inner_dr, outer_def)
        inner_dr.addResult(use_id_dr, inner_def)
        inner_dr.addResult(defined_id_dr, inner_def)

        lookups = use_id_dr.getExistsConstraintTargetLookups()

        self.assertIsNotNone(lookups)
        self.assertEqual(len(lookups), 1)

        lookup = lookups[0]
        self.assertIs(lookup.scope, inner_dr)
        self.assertEqual(lookup.source, "use_id")
        self.assertEqual(lookup.target, "defined_id")
        self.assertIs(lookup.constraint, constraint)

        targets = lookup.scope.select(lookup.target)
        self.assertIsNotNone(targets)
        self.assertEqual(len(targets), 1)
        self.assertIs(targets[0], defined_id_dr)    
    
    def test_deserialized_exists_source_lookup_returns_scoped_result(self):
        """
        Verify that getExistsConstraintSourceLookups() returns the
        DeserializedResult corresponding to the scope in which the
        ExistsConstraint is declared.

        Schema
        ======

            root
            └── outer
                └── inner
                    ├── use_id
                    └── defined_id

            ExistsConstraint(
                source=["use_id"],
                target=["defined_id"]
            ) declared on 'inner'

        Deserialized Tree
        =================

            root
            └── outer
                └── inner
                    ├── use_id = "foo"
                    └── defined_id = "foo"  <-- lookup originates here

        Expected
        ========

        Looking up sources from 'defined_id' should return a single
        ExistsConstraintLookup whose scope is the 'inner'
        DeserializedResult and whose source path is "use_id".
        Selecting "use_id" from that scope should resolve to the
        sibling node under 'inner'.
        """
        
        db = self.db
        interpreter = FakeInterpreter()

        root_def = db.InputObject()
        outer_def = root_def.create("outer")
        inner_def = outer_def.create("inner")

        use_id_def = inner_def.create("use_id", Action=db.storeStr)
        defined_id_def = inner_def.create("defined_id", Action=db.storeStr)

        constraint = db.ExistsConstraint(["use_id"], target=["defined_id"])
        inner_def.addExistsConstraint(constraint)

        root_dr = self.make_result("root", root_def, interpreter)
        outer_dr = self.make_result("outer", outer_def, interpreter)
        inner_dr = self.make_result("inner", inner_def, interpreter)
        use_id_dr = self.make_result("use_id", use_id_def, interpreter, "foo")
        defined_id_dr = self.make_result("defined_id", defined_id_def, interpreter, "foo")

        root_dr.addResult(outer_dr, root_def)
        outer_dr.addResult(inner_dr, outer_def)
        inner_dr.addResult(use_id_dr, inner_def)
        inner_dr.addResult(defined_id_dr, inner_def)

        lookups = defined_id_dr.getExistsConstraintSourceLookups()

        self.assertIsNotNone(lookups)
        self.assertEqual(len(lookups), 1)

        lookup = lookups[0]
        self.assertIs(lookup.scope, inner_dr)
        self.assertEqual(lookup.source, "use_id")
        self.assertEqual(lookup.target, "defined_id")
        self.assertIs(lookup.constraint, constraint)

        sources = lookup.scope.select(lookup.source)
        self.assertIsNotNone(sources)
        self.assertEqual(len(sources), 1)
        self.assertIs(sources[0], use_id_dr)

    def test_deserialized_exists_lookup_uses_nearest_constraint_scope(self):
        """
        Verify that ExistsConstraint lookup uses the scope in which the
        constraint is declared rather than the root of the document.

        Schema
        ======

            root
            ├── defined_id
            └── outer
                └── inner
                    ├── use_id
                    └── defined_id

            ExistsConstraint(
                source=["use_id"],
                target=["defined_id"]
            ) declared on 'inner'

        Deserialized Tree
        =================

            root
            ├── defined_id = "root_value"
            └── outer
                └── inner
                    ├── use_id = "inner_value"   <-- lookup originates here
                    └── defined_id = "inner_value"

        Expected
        ========

        The lookup should return the 'inner' DeserializedResult as the
        lookup scope. Selecting "defined_id" from that scope must resolve
        to the sibling node under 'inner' and not the similarly named
        'defined_id' located at the root of the document.
        """
        db = self.db
        interpreter = FakeInterpreter()

        root_def = db.InputObject()
        root_defined_id_def = root_def.create("defined_id", Action=db.storeStr)

        outer_def = root_def.create("outer")
        inner_def = outer_def.create("inner")

        use_id_def = inner_def.create("use_id", Action=db.storeStr)
        inner_defined_id_def = inner_def.create("defined_id", Action=db.storeStr)

        inner_def.addExistsConstraint(
            db.ExistsConstraint(["use_id"], target=["defined_id"])
        )

        root_dr = self.make_result("root", root_def, interpreter)
        root_defined_id_dr = self.make_result(
            "defined_id", root_defined_id_def, interpreter, "root_value"
        )
        outer_dr = self.make_result("outer", outer_def, interpreter)
        inner_dr = self.make_result("inner", inner_def, interpreter)
        use_id_dr = self.make_result("use_id", use_id_def, interpreter, "inner_value")
        inner_defined_id_dr = self.make_result(
            "defined_id", inner_defined_id_def, interpreter, "inner_value"
        )

        root_dr.addResult(root_defined_id_dr, root_def)
        root_dr.addResult(outer_dr, root_def)
        outer_dr.addResult(inner_dr, outer_def)
        inner_dr.addResult(use_id_dr, inner_def)
        inner_dr.addResult(inner_defined_id_dr, inner_def)

        lookups = use_id_dr.getExistsConstraintTargetLookups()

        self.assertIsNotNone(lookups)
        self.assertEqual(len(lookups), 1)

        lookup = lookups[0]
        self.assertIs(lookup.scope, inner_dr)

        targets = lookup.scope.select(lookup.target)
        self.assertEqual(len(targets), 1)
        self.assertIs(targets[0], inner_defined_id_dr)
        self.assertIsNot(targets[0], root_defined_id_dr)        
if __name__ == "__main__":
    unittest.main(verbosity=2)
