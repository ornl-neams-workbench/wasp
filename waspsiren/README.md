# Sequence Input Retrieval Engine (SIREN)

SIREN is a small query language for navigating and selecting nodes in WASP
parse trees. Its syntax is influenced by
[XPath](https://www.w3.org/TR/xpath-31/), but SIREN is an independent language.
XPath expressions should not be assumed to work unless the construct is
documented here.

For executable examples, see
[`tstSIRENInterpreter.cpp`](test/tstSIRENInterpreter.cpp) and
[`tstSIRENParser.cpp`](test/tstSIRENParser.cpp).

## Evaluation model

A query is evaluated from a context node and returns an ordered set of adapted
parse-tree nodes. A relative path begins at the supplied context node. An
absolute path first walks to the root of that node's document.

Each ordinary path step selects children of the nodes produced by the previous
step. Predicates filter the nodes selected by their step; a following `/` step
navigates into the filtered nodes.

For example:

```text
/item[name != 'beta'][name]  # returns matching item nodes
/item[name != 'beta']/name   # returns their name children
```

Names, operators, function names, and string comparisons are case-sensitive.
An expression that matches nothing returns an empty result set.

## Paths and navigation

| Expression | Meaning |
| --- | --- |
| `name` | Select children named `name` from the context node |
| `/name` | Select children named `name` from the document root |
| `/` | Select the document root |
| `..` | Select the parent of the context node, when one exists |
| `parent/child` | Select `child` nodes beneath selected `parent` nodes |
| `//name` | Select matching descendants anywhere below the document root |
| `parent//name` | Select matching descendants below selected `parent` nodes |
| `parent//` | Select all descendants below selected `parent` nodes |
| `node/following-sibling::name` | Select matching siblings after `node` |
| `node/preceding-sibling::name` | Select matching siblings before `node` |

Sibling-axis results are returned in document order. A plain `name` step
selects children of the current node. From a selected node, `../name` selects
all matching children of its parent, regardless of whether they occur before
or after that node. `following-sibling::name` selects only matching siblings
after it, while `preceding-sibling::name` selects only matching siblings before
it.

The sibling axes are useful when document order carries meaning, such as
finding declarations after an override or checking that one element occurs
before another. SIREN currently supports only the `following-sibling` and
`preceding-sibling` axes.

## Predicates

Predicates are written in square brackets and filter the nodes selected by the
step immediately before them.

### Existence and comparison

Within predicates, a bare name or relative path is a node selection. Its
boolean value is true when that selection is not empty. Quoted strings are
scalar string literals.

| Expression | Meaning |
| --- | --- |
| `/item[name]` | Select items containing a `name` child |
| `/item[not(disabled)]` | Select items without a `disabled` child |
| `/item[value = 3.14]` | Select items whose `value` equals `3.14` |
| `/item[value >= 1 and value < 10]` | Combine numeric conditions |
| `/item[name = 'left' or name = 'right']` | Combine string conditions |
| `/item[metadata/name = 'fred']` | Use a nested relative path |
| `/item[enabled][value > 0]` | Apply chained predicates from left to right |

Supported comparison operators are `=`, `!=`, `<`, `<=`, `>`, and `>=`.
`and`/`&&`, `or`/`||`, `not()`, and unary `!` are supported.

Comparison follows these rules:

- If both compared values are numeric, SIREN performs a numeric comparison.
- Otherwise, SIREN performs a case-sensitive string comparison.
- When an operand selects multiple nodes, the comparison succeeds when any
  pair of values satisfies the operator.
- A missing node selection has no values and therefore does not satisfy a
  comparison.

String literals should be quoted. Bare identifiers in predicates are always
interpreted as path operands, not string constants.

### Position, ranges, and strides

SIREN uses one-based positions.

| Expression | Meaning |
| --- | --- |
| `/item[1]` | Select the first candidate item |
| `/item[1:10]` | Select candidates 1 through 10, inclusively |
| `/item[1:10:2]` | Select every second candidate from positions 1 through 10 |
| `/item[position() = last()]` | Select the last item in each predicate context |
| `/item[position() mod 2 = 1]` | Select odd-positioned items |
| `/item[position() - 1 = 1]` | Select the item at position 2 |

The colon range and stride forms are SIREN extensions, not XPath syntax.
Numeric index, range, and stride predicates operate on one combined sequence of
all candidates produced by that path step. Expressions using `position()` and
`last()` instead reset for each input context node. This distinction is
observable when a step is evaluated beneath multiple parents.

For example, if three `key` nodes each contain one `decl` child:

```text
/key/decl[1]                 # first decl across all three keys
/key/decl[position() = 1]    # first decl beneath each key
```

### Functions

| Function | Result |
| --- | --- |
| `position()` | One-based position in the current predicate context |
| `last()` | Number of candidates in the current predicate context |
| `count(path)` | Number of nodes selected by `path` |
| `contains(value, string)` | Whether the first value contains `string` |
| `starts-with(value, string)` | Whether the first value starts with `string` |
| `not(expression)` | Boolean negation of `expression` |

`contains()` and `starts-with()` return false when their first argument selects
no values. When it selects multiple values, only the first selected value is
examined.

### Computed values

Functions, arithmetic, comparisons, and boolean operators produce temporary
values while a predicate is evaluated. These values decide whether the current
node remains selected; they do not create nodes or appear in the final result
set.

For example:

```text
/item[count(name) = 1]          # items containing exactly one name child
/item[position() - 1 = 1]       # the item at position 2
/item[count(name) + count(value) >= 2]
```

Computed values can be numbers, strings, or booleans. Paths provide the data of
their selected nodes. Arithmetic uses the first selected value when a path
selects more than one node. A computed number used directly as a predicate is
treated as a one-based position, so `/item[1 + 1]` selects the second item.

The public SIREN result is always a set of selected nodes. Scalar functions
such as `count(name)` are supported within predicates, but a top-level scalar
query such as `count(/item)` cannot return a number.

### Arithmetic and precedence

Predicate arithmetic supports `+`, `-`, `*`, `div`, `mod`, and `^`. `/` is a
path separator; use `div` for division.

From highest to lowest, predicate precedence is:

1. Parenthesized expressions and function calls
2. Exponentiation (`^`)
3. Unary negation (`-`) and boolean negation (`!`)
4. Multiplication, division, and remainder (`*`, `div`, `mod`)
5. Addition and subtraction (`+`, `-`)
6. Comparisons (`=`, `!=`, `<`, `<=`, `>`, `>=`)
7. Boolean `and`/`&&`
8. Boolean `or`/`||`

Exponentiation associates from right to left. Other binary arithmetic and
boolean operators associate from left to right.

## Combining result sets

Top-level paths can be combined using union, intersection, and difference.

| Expression | Meaning |
| --- | --- |
| `/left \| /right` | Select nodes present in either result |
| `/item intersect /item[1]` | Select nodes present in both results |
| `/item except /item[1]` | Remove right-side nodes from the left result |

`intersect` and `except` bind more tightly than union (`|`) and associate from
left to right. Union does not append a right-side node that is already present
in the accumulated result.

Ordering is stable rather than globally re-sorted:

- Union retains the left result order and appends unique right-side nodes.
- Intersection and `except` retain the left result order.

## Wildcard names

SIREN name patterns support `*` for zero or more characters and `?` for one
character.

| Pattern | Example matches |
| --- | --- |
| `*` | Any child name |
| `*less` | `less`, `wireless` |
| `less*` | `less`, `lesson` |
| `l*s` | `ls`, `less`, `limits` |
| `va?ue` | `value` |

Wildcard matching is case-sensitive. Multiple `*` characters are supported.

## C++ usage

An adapter supplied to `evaluate()` must provide the node navigation interface
documented by `SIRENInterpreter`. All WASP node-view adapters satisfy this
interface.

```cpp
wasp::DefaultSIRENInterpreter selector;
if (!selector.parseString("/item[name != 'beta']/name"))
{
    // Read parser diagnostics from the stream supplied to the interpreter.
}

wasp::SIRENResultSet<MyNodeView> result;
selector.evaluate(context_node, result);

for (std::size_t i = 0; i < result.size(); ++i)
{
    const MyNodeView& node = result.adapted(i);
    // Use node.name(), node.data(), and the remaining adapter interface.
}
```

A parsed SIREN interpreter can be evaluated repeatedly against different
context nodes.

## Differences from XPath

SIREN does not currently implement the complete XPath data model or grammar.
Notable unsupported constructs include:

- The current-node step (`.`). Use the supplied context node directly or begin
  with the child step that should be selected.
- Attribute selection with `@`.
- Namespace-qualified names.
- Node tests such as `text()`, `node()`, and `comment()`.
- Axes other than `following-sibling` and `preceding-sibling`.
- General XPath sequence construction and sequence operators.
- Parenthesized top-level result-set expressions.
- Absolute paths inside predicates.

Quoted path names are supported in ordinary paths, allowing names containing
spaces. Within predicates, quoted text is always a scalar literal, so a node
whose name requires quoting cannot currently be used as a predicate path
operand.

When portability between XPath and SIREN matters, restrict expressions to the
constructs explicitly documented above and test them with the SIREN
interpreter.
