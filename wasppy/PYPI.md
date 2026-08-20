# ORNL WASP Python bindings

WASPPY parses, navigates, and validates SON, HIT, DDI, and EDDI input
documents from Python. It provides Python access to the
[Workbench Analysis Sequence Processor (WASP)](https://code.ornl.gov/neams-workbench/wasp)
parse trees, schema validation, diagnostics, and language-server components.

## Installation

```shell
python -m pip install ornl-wasp
```

The distribution name is `ornl-wasp`; the primary import name is `wasp`.
WASPPY requires Python 3.10 or newer.

Prebuilt wheels are published for:

- Linux x86-64
- macOS universal2 (`arm64` and `x86_64`), requiring macOS 11 or newer
- Windows x86-64

WASPPY is currently distributed as platform-specific wheels. For other
platforms, follow the
[WASP source-build instructions](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/README.md#code-configuration-and-compilation).

## Quick start

Parse in-memory SON input and navigate its parse tree:

```python
from wasp import Interpreter, Syntax

source = """
config {
    answer = 42
}
"""

document = Interpreter(Syntax.SON, data=source, path="<memory>")
if not document:
    messages = document.parseDiagnostics() or []
    raise RuntimeError("\n".join(str(message) for message in messages))

root = document.root()
answer = root.config.answer.value[0]
print(int(answer))  # 42
```

Keep the `Interpreter` alive while using nodes obtained from it. `WaspNode`
objects are views into storage owned by their interpreter.

## Supported syntaxes

| Syntax | Parse without a schema | Validate with a SON schema |
|---|---:|---:|
| SON | Yes | Yes |
| HIT | Yes | Yes |
| DDI | No | Required |
| EDDI | No | Required |

Create an interpreter with `Syntax.SON`, `Syntax.HIT`, `Syntax.DDI`, or
`Syntax.EDDI`. For definition-driven DDI and EDDI documents, provide a
SON-formatted schema:

```python
from wasp import Interpreter, Syntax

document = Interpreter(
    Syntax.DDI,
    path="input.ddi",
    schema="schema.sch",
)
```

For SON and HIT, a schema is optional for parsing and can be supplied when
validation is required.

## Navigating the parse tree

Attribute access selects children by name:

```python
values = document.root().section.parameter.value
```

Named selection returns a `VectorWaspNode` because a name may occur more than
once. Index the result when a single node is expected:

```python
first_value = document.root().section.parameter.value[0]
```

Use bracket syntax for names that conflict with Python keywords:

```python
class_nodes = document.root()["class"]
```

Nodes also expose their name, source location, path, original data, and
children.

## Validation and diagnostics

When a schema is supplied, call `errors()` to validate the parsed document:

```python
errors = document.errors()
if errors:
    for error in errors:
        print(error)
```

Parse failures are available through `parseDiagnostics()`. Applications using
`Database.InputObject` can create additional error, warning, information, and
hint diagnostics during deserialization.

## Programmatic input definitions

The `Database` module lets applications define their expected input structure
in Python with occurrence rules, types, defaults, enumerations, value
constraints, uniqueness rules, and custom conversion actions.

See the
[complete chemistry and schema tutorial](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/wasppy/docs/chemistry_example.md)
for:

- SON and HIT input examples
- A complete validation schema
- Attribute-style parse-tree navigation
- An equivalent `InputObject` definition
- Custom validation diagnostics

## Project links

- [Source repository](https://code.ornl.gov/neams-workbench/wasp)
- [WASP documentation](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/README.md)
- [Changelog](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/CHANGELOG.md)
- [Contributing](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/CONTRIBUTING.md)
- [Issue tracker](https://code.ornl.gov/neams-workbench/wasp/-/issues)
- [License](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/LICENSE)

## License

WASP is distributed under the UT-Battelle Open Source Software License. See the
[license text](https://code.ornl.gov/neams-workbench/wasp/-/blob/master/LICENSE)
for terms and third-party notices.
