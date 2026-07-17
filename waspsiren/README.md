# Sequence Input Retrieval Engine (SIREN) 
SIREN is a syntax for navigating and selecting document elements. It is heavily influenced by the XML XPath [https://www.w3schools.com/xml/xpath_syntax.asp] component within the XSLT standard.

For code examples using SIREN, see the SIREN interpreter tests in the code repository.

## Selecting Nodes
The selection of nodes is performed via a path expression. Path expressions can be relative to a current node or absolute to the document.

An empty result set will be produced if no elements in the document match the given path expression.

|Expression | Description |
| --------- | ----------- |
|_nodename_ | Selects all nodes with the name "_nodename_" that are children of the current node |
|/          | Selects from the root of the document|
|.          | Selects the current node|
|..         | selects the parent of the current node|


### Selection Examples

|Expression | Description |
| --------- | ----------- |
|_value_    | Selects all nodes with the name "_value_" that are children of the current node |
|/_value_   | Selects all nodes with the name "_value_" that are children of the root of the document |
|./_value_  | Selects all nodes with the name "_value_" from the current node |
|../_value_ | selects all nodes with the name "_value_" that are children of the parent of the current node |
|//_value_ | Selects matching descendants anywhere below the document root |
|_parent_//_value_ | Selects matching descendants below each selected _parent_ |
|_value_/following-sibling::_value_ | Selects matching siblings after each selected node |
|_value_/preceding-sibling::_value_ | Selects matching siblings before each selected node |

### Predicates
Selection of document elements may require predicated search patterns that evaluate the position of value of the element.

Predicates can be used at all level of the path expression and are expressed as 1-base array indices, ranges, or strides, or token value equality.

|Expression | Description |
| --------- | ----------- |
|_value_[1] | Selects the first node with the name "_value_" that is a child of the current node |
|_value_[1:10] | Selects the first ten nodes with the name "_value_" that are children of the current node |
|_value_[1:10:2] | Selects every other node (stride of 2) with the name "_value_" that are children of the current node |
|_child_[_value_ = 3.14] | Selects all nodes with the name "_child_" of the current node where the _child_'s _value_ is equal to 3.14  |
|_child_[_name_ = 'fred']/_ear_ | Selects all nodes with the name "_ear_" which are children of _child_ of the current node, only when _child_'s name is 'fred' |
|_child_[_name_ = 'fred']/_ear_[hairy='true'] | Selects all nodes with the name "_ear_" which are children of _child_ of the current node, only when _child_'s name is 'fred' and the ear is hairy |

Predicates also support `<`, `<=`, `>`, `>=`, `!=`, `and` (or `&&`),
`or` (or `||`), `not()`, and multiple chained predicates. A bare relative
path is an existence test. Numeric predicates are one-based, and
`position()` and `last()` expose the current predicate context. Predicate
arithmetic supports `+`, `-`, `*`, `div`, `mod`, and `^`.
Within predicates, bare names are path operands and quoted strings are scalar
literals.

|Expression | Description |
| --------- | ----------- |
|_child_[_name_] | Selects children that contain a _name_ child |
|_child_[not(_disabled_)] | Selects children without a _disabled_ child |
|_child_[_value_ >= 1 and _value_ < 10] | Combines predicate conditions |
|_child_[_enabled_][_value_ > 0] | Applies predicates from left to right |
|_child_[position() = last()] | Selects the last matching child |
|_child_[position() mod 2 = 1] | Selects odd-positioned children |
|_child_[_metadata_/_name_ = 'fred'] | Uses a nested relative path |

The predicate functions `count()`, `contains()`, and `starts-with()` are
available. `contains()` and `starts-with()` compare the string value of their
first selected node.

### Combining Result Sets

Top-level selections can be combined with union (`|`), `intersect`, and
`except`. Results are de-duplicated and retain the left operand's document
order.

|Expression | Description |
| --------- | ----------- |
|/_left_ \| /_right_ | Selects nodes present in either result |
|/_item_ intersect /_item_[1] | Selects nodes present in both results |
|/_item_ except /_item_[1] | Removes nodes in the right result from the left result |


### Selecting Unknown Nodes
Certain parts of the document may not be known. For this reason, wildcards are supported in the expression path.

|Expression | Description |
| --------- | ----------- |
|\* | Selects all nodes that are children of the current node, regardless of name |
|\*less | Selects all nodes that are children of the current node, where the node name is '_less_' or ends with '_less_' |
|less\* | Selects all nodes that are children of the current node, where the node name is '_less_' or starts with '_less_' |
|l\*s | Selects all nodes that are children of the current node, where the node name is '_ls_' or starts with '_l_' and ends with '_s_' with any character between|
