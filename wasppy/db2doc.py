#!/usr/bin/env python3
"""Generate docprint-compatible Markdown from a WASP Python input database.

This module is the Python-database counterpart to WASP's ``docprint`` schema
documentation utility.  It executes a database module, obtains its root
``Database.InputObject``, and renders the resulting definition graph as
Markdown containing:

* a bounded-depth table of contents;
* one documentation table for the root and each container;
* occurrence counts, value types, numeric ranges, and ordering rules;
* enumeration and ``ExistsConstraint`` choices and links; and
* referenced choice-list sections for named lists used by ``Enums`` or
  ``discrete``.

The primary programmatic entry point is :func:`generate_markdown`::

    markdown = generate_markdown("path/to/input_database.py")

The module can also be invoked as a command-line program::

    python -m db2doc input_database.py
    python -m db2doc input_database.py --root-class InputDefinition --output input.md

Root selection first honors ``--root-class``/``root_class``, then looks for
``_document``, and finally accepts the sole class defined by the database module
that provides ``definition()``.  The selected object may itself be an
``InputObject`` or may return one from ``definition()``.

Processing has four stages:

1. Parse and instrument the database's AST so the names of choice-list
   expansions such as ``Enums=[*Names]`` survive normal Python unpacking.
2. Execute the instrumented source in a temporary registered module, with the
   database directory at the front of ``sys.path`` for sibling imports.
3. Copy the ``InputObject`` graph into lightweight documentation nodes and
   resolve rule paths against that graph.
4. Append docprint-compatible Markdown to an in-memory list and return it as a
   single string ending in the same final blank line as ``docprint``.

Named references are captured only when a bare name is supplied directly to
``Enums``/``discrete`` or unpacked inside a list or tuple.  Inline values remain
inline choices.  Reference values are normalized to lowercase to match WASP's
case-insensitive enumeration behavior and existing ``docprint`` output.

Security
--------
Database files are executable Python, not passive configuration.  Execution
can import modules, modify files, or perform any other action allowed to the
current process.  Only generate documentation for database files you trust.
Exceptions propagate from :func:`generate_markdown`; :func:`main` converts them
to a diagnostic on stderr and a nonzero exit status.
"""

import argparse
import ast
import Database
import math
from pathlib import Path
import sys
import types


TOC_TAB = 4
MAX_TOC_DEPTH = 3
POS_INF = 2147483647
NEG_INF = -2147483648


class _ReferenceValue(str):
    """String choice tagged with the name of its referenced choice list."""

    def __new__(cls, value, reference_name):
        """Create a string-compatible value carrying reference provenance."""
        result = str.__new__(cls, str(value))
        result.reference_name = reference_name
        return result


class _ReferenceMarker:
    """Sentinel inserted before values expanded from a named choice list."""

    def __init__(self, reference_name):
        """Record the source list name represented by this sentinel."""
        self.reference_name = reference_name


class _ReferenceList(list):
    """List used when a named choice list is passed without ``*`` expansion."""

    def __init__(self, reference_name, values):
        """Tag every materialized choice with *reference_name*."""
        self.reference_name = reference_name
        super().__init__(_ReferenceValue(value, reference_name) for value in values)


class _ReferenceTransformer(ast.NodeTransformer):
    """Instrument named ``Enums`` and ``discrete`` values before execution.

    Normal Python unpacking discards the source variable name.  Calls inserted
    here record that name and tag the runtime values without rewriting unrelated
    assignments or changing ordinary module-level Python semantics.
    """

    def visit_Call(self, node):
        """Wrap bare and starred choice-list names in runtime helper calls."""
        node = self.generic_visit(node)
        for keyword in node.keywords:
            if keyword.arg not in ("Enums", "discrete"):
                continue
            if isinstance(keyword.value, ast.Name):
                name = keyword.value.id
                keyword.value = ast.Call(
                    func=ast.Name(id="__docprint_use_reference__", ctx=ast.Load()),
                    args=[ast.Constant(name), keyword.value],
                    keywords=[])
                continue
            if not isinstance(keyword.value, (ast.List, ast.Tuple)):
                continue
            values = []
            for value in keyword.value.elts:
                if isinstance(value, ast.Starred) and isinstance(value.value, ast.Name):
                    name = value.value.id
                    value = ast.Starred(
                        value=ast.Call(
                            func=ast.Name(id="__docprint_expand_reference__", ctx=ast.Load()),
                            args=[ast.Constant(name), value.value],
                            keywords=[]),
                        ctx=ast.Load())
                values.append(value)
            keyword.value.elts = values
        return node


class _DocNode:
    """Documentation view of one ``InputObject`` in the definition graph.

    Occurrence rules belong to the edge from the parent ``InputObject`` to its
    child, so they are stored on the child documentation node alongside its
    absolute database path and derived docprint object type.
    """

    def __init__(self, name, input_object, parent=None, min_occurs=None, max_occurs=None):
        """Initialize a documentation node and derive its absolute path."""
        self.name = name
        self.input_object = input_object
        self.parent = parent
        self.min_occurs = min_occurs
        self.max_occurs = max_occurs
        self.children = []
        self.object_type = None
        if parent is None:
            self.path = "/"
        else:
            self.path = ("" if parent.path == "/" else parent.path) + "/" + name

    @property
    def input_name(self):
        """Return the display name, falling back to the database key."""
        return getattr(self.input_object, "_inputName", None) or self.name

    @property
    def description(self):
        """Return a non-null description suitable for Markdown output."""
        return self.input_object._description or ""

    def child(self, name):
        """Return the first direct child named *name*, or ``None``."""
        for item in self.children:
            if item.name == name:
                return item
        return None


def _load_database(path):
    """Execute *path* and return its namespace and captured choice references.

    The temporary module registration gives decorators, annotations, and other
    runtime facilities a normal ``sys.modules`` entry while the database runs.
    Both that entry and the import path are restored even if execution fails.
    """
    source = path.read_text(encoding="utf-8")
    tree = ast.parse(source, filename=str(path))
    tree = _ReferenceTransformer().visit(tree)
    ast.fix_missing_locations(tree)

    references = {}

    def record_reference(name, values):
        """Materialize and record one named choice iterable."""
        expanded = list(values)
        references[name] = [str(value).lower() for value in expanded]
        return expanded

    def expand_reference(name, values):
        """Return tagged values for a starred choice-list expansion."""
        expanded = record_reference(name, values)
        return ([_ReferenceMarker(name)] +
                [_ReferenceValue(value, name) for value in expanded])

    def use_reference(name, values):
        """Return a tagged list for ``Enums=Name`` or ``discrete=Name``."""
        return _ReferenceList(name, record_reference(name, values))

    module_name = "_wasp_input_database_docprint"
    module = types.ModuleType(module_name)
    namespace = module.__dict__
    namespace.update({
        "__builtins__": __builtins__,
        "__docprint_expand_reference__": expand_reference,
        "__docprint_use_reference__": use_reference,
        "__file__": str(path),
        "__package__": None,
    })

    missing = object()
    previous_module = sys.modules.get(module_name, missing)
    sys.modules[module_name] = module
    sys.path.insert(0, str(path.parent))
    try:
        exec(compile(tree, str(path), "exec"), namespace)
    finally:
        sys.path.pop(0)
        if previous_module is missing:
            del sys.modules[module_name]
        else:
            sys.modules[module_name] = previous_module
    return namespace, references


def _root_definition(namespace, root_name=None):
    """Resolve and instantiate the database root ``InputObject``.

    ``root_name`` is authoritative when supplied.  Otherwise ``_document`` is
    preferred, followed by unambiguous auto-detection among classes defined by
    the executed database module.  Clear errors are raised for missing,
    ambiguous, non-callable, or incorrectly typed roots.
    """
    if root_name is not None:
        if root_name not in namespace:
            raise ValueError("Database has no root named " + repr(root_name))
        candidates = [(root_name, namespace[root_name])]
    elif "_document" in namespace:
        candidates = [("_document", namespace["_document"])]
    else:
        candidates = []
        for name, value in namespace.items():
            if (isinstance(value, type) and value.__module__ == namespace["__name__"] and
                    callable(getattr(value, "definition", None))):
                candidates.append((name, value))
        if len(candidates) != 1:
            names = ", ".join(name for name, unused in candidates) or "none"
            raise ValueError(
                "Could not identify one database root class (candidates: " + names +
                "); supply --root-class")

    name, value = candidates[0]
    if isinstance(value, Database.InputObject):
        definition = value
    else:
        instance = value() if isinstance(value, type) else value
        method = getattr(instance, "definition", None)
        if not callable(method):
            raise ValueError("Root " + repr(name) + " has no definition() method")
        definition = method()
    if not isinstance(definition, Database.InputObject):
        raise TypeError("Root definition() did not return an InputObject")
    return definition


def _build_tree(root_definition):
    """Build and classify the documentation tree rooted at *root_definition*.

    Classification mirrors docprint: explicit ``InputType`` wins, while other
    nodes are inferred as root, container, flag, flagvalue, flagarray, or value
    from their children and the value child's maximum occurrence.
    """
    root = _DocNode("root", root_definition)

    def populate(node):
        """Recursively copy children and their edge occurrence rules."""
        children = node.input_object._children or {}
        for name, child_object in children.items():
            min_occurs = (node.input_object._minOccurs or {}).get(name)
            max_occurs = (node.input_object._maxOccurs or {}).get(name)
            if min_occurs is None:
                min_occurs = (getattr(
                    node.input_object, "_minOccursPath", None) or {}).get(name)
            if max_occurs is None:
                max_occurs = (getattr(
                    node.input_object, "_maxOccursPath", None) or {}).get(name)
            child = _DocNode(name, child_object, node, min_occurs, max_occurs)
            node.children.append(child)
            populate(child)

    def classify(node):
        """Assign docprint object types in post-order."""
        for child in node.children:
            classify(child)
        if node.parent is None:
            node.object_type = "root"
            return
        if node.input_object._inputType is not None:
            node.object_type = str(node.input_object._inputType)
            return
        if len(node.children) == 1 and node.children[0].name == "value":
            maximum = node.children[0].max_occurs
            if maximum is None:
                node.object_type = "flagarray"
            elif _numeric_equal(maximum, 0):
                node.object_type = "flag"
            elif _numeric_equal(maximum, 1):
                node.object_type = "flagvalue"
            else:
                node.object_type = "flagarray"
        elif node.children:
            node.object_type = "container"
        elif (node.name == "value" and node.parent is not None and
              len(node.parent.children) == 1):
            node.object_type = "value"
        else:
            node.object_type = "flagvalue"

    populate(root)
    classify(root)
    return root


def _numeric_equal(value, expected):
    """Compare a possibly textual numeric rule value without raising."""
    try:
        return float(value) == expected
    except (TypeError, ValueError):
        return False


def _md_escape(value):
    """Convert an absolute definition path to a docprint anchor name."""
    if value == "/":
        return "root_anchor"
    return value.replace("/", "").lower()


def _select_nodes(start, path):
    """Select documentation nodes using WASP rule-path syntax.

    A leading slash selects from the root.  Relative paths support ``..`` and
    ``*`` components, and occurrence selectors such as ``name[0]`` are ignored
    because documentation resolves definitions rather than runtime instances.
    Invalid or unmatched components produce an empty selection.
    """
    if path.startswith("/"):
        while start.parent is not None:
            start = start.parent
    current = [start]
    for raw_name in filter(None, path.split("/")):
        name = raw_name.split("[", 1)[0]
        following = []
        for node in current:
            if name == "..":
                if node.parent is not None:
                    following.append(node.parent)
            elif name == "*":
                following.extend(node.children)
            elif "." not in name:
                child = node.child(name)
                if child is not None:
                    following.append(child)
        current = following
        if not current:
            break
    return current


def _is_tagged_value(node):
    """Return whether *node* is the canonical ``id``/``value`` tagged object."""
    return len(node.children) == 2 and [item.name for item in node.children] == ["id", "value"]


def _collapse_value_node(node, relative_path):
    """Collapse a non-tagged explicit ``value`` target to its holding object.

    Docprint links ordinary explicit value children to the surrounding keyed
    value or array.  Tagged objects keep separate links for ``id`` and ``value``.
    The display path is shortened in parallel with the selected node.
    """
    if (node.name == "value" and node.parent is not None and
            node.parent.parent is not None and not _is_tagged_value(node.parent)):
        node = node.parent
        if relative_path.rsplit("/", 1)[-1] == "value":
            relative_path = relative_path.rsplit("/", 1)[0]
    return node, relative_path


def _linked_rule_value(node, value):
    """Render a path-valued rule as a Markdown link when it resolves uniquely.

    Constants and ``NoLimit`` return ``None`` so their callers can format them
    numerically.  An unresolved path is returned as plain text.
    """
    if not isinstance(value, str):
        return None
    stripped = value.strip().strip('"\'')
    if stripped == "NoLimit" or _is_number(stripped):
        return None
    selected = _select_nodes(node, stripped)
    if len(selected) != 1:
        return stripped
    target, stripped = _collapse_value_node(selected[0], stripped)
    label = stripped.rsplit("/", 1)[-1]
    return "[{}](#{})".format(label, _md_escape(target.path))


def _occurrence_value(node, value, default, is_maximum):
    """Return numeric and display forms of one occurrence bound.

    The numeric form supports range-shape decisions in :func:`_how_many`; the
    display form preserves links and docprint's ``NoLimit`` spelling.
    """
    integer = POS_INF if is_maximum else default
    if value is None:
        return integer, "NoLimit" if is_maximum else str(default)
    linked = _linked_rule_value(node, value)
    if linked is not None:
        return integer, linked
    if isinstance(value, str):
        value = value.strip().strip('"\'')
    if value == "NoLimit":
        integer = POS_INF if is_maximum else NEG_INF
    else:
        integer = int(value)
    return integer, "NoLimit" if integer == POS_INF else str(integer)


def _how_many(node, default_minimum):
    """Format a node's minimum/maximum occurrences in docprint terminology."""
    min_int, min_text = _occurrence_value(node, node.min_occurs, default_minimum, False)
    max_int, max_text = _occurrence_value(node, node.max_occurs, POS_INF, True)
    if min_text == max_text:
        return min_text
    if min_int + 1 == max_int:
        return min_text + " or " + max_text
    if max_text == "NoLimit":
        return min_text + " or more"
    return min_text + " to " + max_text


def _value_node(node):
    """Return an explicit ``value`` child, or *node* for terminal definitions."""
    return node.child("value") or node


def _value_type(node):
    """Determine the displayed value type from metadata or storage action."""
    explicit = getattr(node.input_object, "_valueType", None)
    if explicit:
        value_type = str(explicit)
    else:
        action = node.input_object._action
        action_name = getattr(action, "__name__", "")
        if action is Database.storeInt or action_name == "storeInt":
            value_type = "Int"
        elif action is Database.storeFloat or action_name == "storeFloat":
            value_type = "Real"
        else:
            value_type = "String"
    return "Integer" if value_type == "Int" else value_type


def _split_choices(values):
    """Separate inline choice values from named reference-list usages."""
    raw_values = []
    reference_names = []
    if isinstance(values, _ReferenceList):
        reference_names.append(values.reference_name)
    for value in values or []:
        if isinstance(value, (_ReferenceMarker, _ReferenceValue)):
            name = value.reference_name
            if name not in reference_names:
                reference_names.append(name)
        else:
            raw_values.append(str(value).lower())
    return raw_values, reference_names


def _format_constant(value, value_type):
    """Format a numeric bound with docprint-compatible precision."""
    if value_type == "Real":
        return format(float(value), ".1f")
    return str(int(float(value)))


def _is_number(value):
    """Return whether *value* can be interpreted as a floating-point number."""
    try:
        float(value)
        return True
    except (TypeError, ValueError):
        return False


def _format_bound(node, value, value_type, inclusive, is_minimum):
    """Return the delimiter and content for one numeric range endpoint.

    Endpoints may be constants, infinities, ``NoLimit``, or paths to other
    definition nodes.  Brackets and parentheses are escaped for Markdown.
    """
    default_value = "-INF" if is_minimum else "+INF"
    if is_minimum:
        delimiter = "\\[" if inclusive else "\\("
    else:
        delimiter = "\\]" if inclusive else "\\)"
    if value is None:
        return delimiter if not inclusive else ("\\(" if is_minimum else "\\)"), default_value

    linked = _linked_rule_value(node, value)
    if linked is not None:
        return delimiter, linked
    if isinstance(value, str):
        value = value.strip().strip('"\'')
    if value == "NoLimit":
        return "\\(" if is_minimum else "\\)", default_value
    numeric = float(value)
    if ((is_minimum and numeric == NEG_INF) or
            (not is_minimum and numeric == POS_INF) or math.isinf(numeric)):
        return "\\(" if is_minimum else "\\)", default_value
    return delimiter, _format_constant(value, value_type)


def _range_string(node, value_type):
    """Build the documented numeric interval for *node*, if applicable.

    Inclusive rules take precedence over exclusive rules on each side, and
    constant values take precedence over their corresponding path attributes.
    Non-numeric value types have no range section.
    """
    if value_type not in ("Integer", "Real"):
        return ""
    obj = node.input_object
    min_val_inc = (obj._minValInc if obj._minValInc is not None else
                   getattr(obj, "_minValIncPath", None))
    min_val_exc = (obj._minValExc if obj._minValExc is not None else
                   getattr(obj, "_minValExcPath", None))
    max_val_inc = (obj._maxValInc if obj._maxValInc is not None else
                   getattr(obj, "_maxValIncPath", None))
    max_val_exc = (obj._maxValExc if obj._maxValExc is not None else
                   getattr(obj, "_maxValExcPath", None))
    if min_val_inc is not None:
        min_delim, minimum = _format_bound(node, min_val_inc, value_type, True, True)
    elif min_val_exc is not None:
        min_delim, minimum = _format_bound(node, min_val_exc, value_type, False, True)
    else:
        min_delim, minimum = "\\(", "-INF"
    if max_val_inc is not None:
        max_delim, maximum = _format_bound(node, max_val_inc, value_type, True, False)
    elif max_val_exc is not None:
        max_delim, maximum = _format_bound(node, max_val_exc, value_type, False, False)
    else:
        max_delim, maximum = "\\)", "+INF"
    return min_delim + minimum + "," + maximum + max_delim


def _holding_name(node):
    """Return the container label used for an ``ExistsConstraint`` target."""
    return "root" if node.parent is None or node.parent.parent is None else node.parent.input_name


def _order_context(rule):
    """Extract the context path from an ordering rule tuple or scalar."""
    if isinstance(rule, (list, tuple)):
        return rule[0] if rule else None
    return rule


def _gather_exists(root):
    """Index ``ExistsConstraint`` instances by each resolved source node.

    Each entry retains the context where the constraint was declared because
    its source and target paths are relative to that context.
    """
    by_source = {}

    def visit(context):
        """Collect constraints recursively from one definition context."""
        for constraint in context.input_object._exists or []:
            for source_path in constraint._source:
                for source in _select_nodes(context, source_path):
                    by_source.setdefault(id(source), []).append((context, constraint))
        for child in context.children:
            visit(child)

    visit(root)
    return by_source


def _exists_restrictions(node, exists_by_source):
    """Collect choices, references, and target links restricting *node*."""
    raw_choices = []
    reference_names = []
    keys = []
    for context, constraint in exists_by_source.get(id(node), []):
        raw, references = _split_choices(constraint._discrete)
        raw_choices.extend(raw)
        for name in references:
            if name not in reference_names:
                reference_names.append(name)
        for target_path in constraint._target:
            selected = _select_nodes(context, target_path)
            if len(selected) != 1:
                keys.append(str(target_path) + "<br>")
                continue
            target, display_path = _collapse_value_node(selected[0], str(target_path))
            label = display_path.rsplit("/", 1)[-1]
            keys.append("[{} {}](#{})<br>".format(
                _holding_name(target), label, _md_escape(target.path)))
    return raw_choices, reference_names, keys


def _restrictions(node, exists_by_source, used_references):
    """Render all range, choice, and input-key restrictions for a table cell.

    Reference names encountered here are added to ``used_references`` so only
    referenced lists that appear in documentation receive appendix sections.
    """
    value_type = _value_type(node)
    value_range = _range_string(node, value_type)
    raw_choices, reference_names = _split_choices(node.input_object._enums)
    exists_raw, exists_references, keys = _exists_restrictions(node, exists_by_source)
    raw_choices.extend(exists_raw)
    for name in exists_references:
        if name not in reference_names:
            reference_names.append(name)

    choices = "".join(value + "<br>" for value in raw_choices)
    for name in reference_names:
        choices += "REF:[{}](#ref-{})<br>".format(name, _md_escape(name))
        used_references.add(name)
    key_text = "".join(keys)

    result = ""
    if value_range:
        result += "__Range__<br>" + value_range
        if choices or key_text:
            result += "<br><br>"
    if choices:
        result += "__Choices__<br>" + choices
        if key_text:
            result += "<br>"
    if key_text:
        result += "__InputKeys__<br>" + key_text
    return result


def _breadcrumb(node):
    """Build linked root-to-node navigation matching docprint output."""
    if node.parent is None:
        return "[/](#root_anchor)"
    lineage = []
    current = node
    while current.parent is not None:
        lineage.append(current)
        current = current.parent
    lineage.reverse()
    pieces = ["[/](#root_anchor)"]
    for index, item in enumerate(lineage):
        link = "[{}](#{})".format(item.input_name, _md_escape(item.path))
        pieces.append(link if index == 0 else "/" + link)
    return "".join(pieces)


def _write_toc(lines, node, level):
    """Append the bounded-depth table of contents in definition order."""
    if node.object_type == "root":
        lines.extend([
            '<a name="startofinput"></a>',
            "",
            "### Table Of Contents",
            "- [root](#root_anchor)",
        ])
    elif ((node.object_type == "container" or level == 1) and
          level <= MAX_TOC_DEPTH and node.name != "EndOfSchema"):
        lines.append(" " * (TOC_TAB * level) + "- [{}](#{})".format(
            node.input_name, _md_escape(node.path)))
    for child in node.children:
        _write_toc(lines, child, level + 1)
    if node.object_type == "root":
        lines.extend([
            "- [Referenced Choice Lists](#refchoicelists)",
            "",
            "---",
            "",
        ])


def _write_documentation(lines, node, exists_by_source, used_references):
    """Append documentation tables recursively for root and container nodes.

    Container children link to their own tables.  Terminal-like children are
    rendered directly as rows containing occurrence, type, restriction, and
    description data.  ``EndOfSchema`` is an implementation marker and is not
    included in user-facing documentation.
    """
    if node.object_type not in ("root", "container"):
        return
    lines.append('### <a name="{}"></a>{}'.format(_md_escape(node.path), _breadcrumb(node)))
    if node.description and node.object_type != "root":
        lines.extend(["#### " + node.description, ""])
    if node.object_type == "container":
        lines.extend(["##### How Many: " + _how_many(node, 0), ""])
    lines.extend([
        "Name|Type|HowMany|ValueType|Restrictions|Description",
        ":---:|:---:|:---:|:---:|:---:|:---:|",
    ])

    for child in node.children:
        if child.name == "EndOfSchema":
            continue
        if child.object_type == "container":
            display_type = "TaggedValue" if _is_tagged_value(child) else "SubObject"
            lines.append("[{}](#{})|{}|{}|||{}|".format(
                child.input_name, _md_escape(child.path), display_type,
                _how_many(child, 0), child.description))
            continue

        display_type = ""
        if child.object_type == "flagarray":
            value = _value_node(child)
            display_type = "Array of Size<br>" + _how_many(value, 1)
            if _order_context(getattr(
                    value.input_object, "_increaseOver", None)) == "..":
                display_type += "<br>Increasing Values"
            elif _order_context(getattr(
                    value.input_object, "_decreaseOver", None)) == "..":
                display_type += "<br>Decreasing Values"
        elif child.name == "id":
            display_type = "Tag"
        elif child.name == "value":
            display_type = "Value"
        elif child.object_type == "flagvalue":
            display_type = "KeyedValue"

        value = _value_node(child)
        lines.append('{}<a name="{}"></a>|{}|{}|{}|{}|{}|'.format(
            child.input_name, _md_escape(child.path), display_type,
            _how_many(child, 0), _value_type(value),
            _restrictions(value, exists_by_source, used_references),
            child.description))

    lines.extend(["", "---", ""])
    for child in node.children:
        _write_documentation(lines, child, exists_by_source, used_references)


def _write_references(lines, references, used_references):
    """Append alphabetized sections for named choice lists actually referenced."""
    header_printed = False
    for name in sorted(references):
        if name not in used_references:
            continue
        if not header_printed:
            lines.extend([
                "[Start of Input](#startofinput)",
                "",
                '## Referenced Choice Lists<a name="refchoicelists"></a>',
            ])
            header_printed = True
        lines.extend([
            "",
            '### <a name="ref-{}"></a>{}'.format(_md_escape(name), name),
            " ".join(references[name]) + " ",
            "",
            "[Start of Input](#startofinput)",
            "",
            "---",
            "",
        ])


def generate_markdown(database_path, root_class=None):
    """Generate documentation for a WASP Python input database.

    Parameters
    ----------
    database_path : str or os.PathLike
        Path to the trusted Python database file to execute.
    root_class : str, optional
        Name of the root class or ``InputObject`` attribute.  When omitted,
        ``_document`` or unambiguous class auto-detection is used.

    Returns
    -------
    str
        Docprint-compatible Markdown ending in a final blank line.

    Raises
    ------
    OSError, SyntaxError
        If the database cannot be read or parsed.
    ValueError, TypeError
        If a valid root definition cannot be selected.
    Exception
        Any exception raised while executing the trusted database or building
        its definition is allowed to propagate.
    """
    path = Path(database_path).resolve()
    namespace, references = _load_database(path)
    root_definition = _root_definition(namespace, root_class)
    root = _build_tree(root_definition)
    exists_by_source = _gather_exists(root)
    used_references = set()
    lines = []
    _write_toc(lines, root, 0)
    _write_documentation(lines, root, exists_by_source, used_references)
    _write_references(lines, references, used_references)
    return "\n".join(lines) + "\n"


def _parse_args(argv):
    """Parse command-line arguments from *argv*."""
    parser = argparse.ArgumentParser(
        description="Generate docprint-compatible Markdown from a WASP Python input database.")
    parser.add_argument("database", help="path to the Python input database")
    parser.add_argument("-o", "--output", help="write Markdown here instead of stdout")
    parser.add_argument(
        "--root-class",
        help="database root class/attribute (default: _document or auto-detect)")
    return parser.parse_args(argv)


def main(argv=None):
    """Run the command-line interface and return a process exit status.

    Markdown is written only after generation succeeds.  Errors are reported
    to stderr with status 1; successful output to a file or stdout returns 0.
    """
    args = _parse_args(argv)
    try:
        markdown = generate_markdown(args.database, args.root_class)
        if args.output:
            Path(args.output).write_text(markdown, encoding="utf-8", newline="\n")
        else:
            sys.stdout.write(markdown)
        return 0
    except Exception as error:
        print("***Error: " + str(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
