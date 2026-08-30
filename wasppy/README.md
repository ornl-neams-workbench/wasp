# WASPPY

WASPPY provides Python bindings for the Workbench Analysis Sequence Processor
(WASP). It exposes WASP parsers, parse-tree navigation, schema validation,
diagnostics, and language-server components to Python applications.

This document covers development and testing in the WASP source tree. For the
package-installation guide shown on PyPI, see [PYPI.md](PYPI.md). The extended
Python API example is in
[docs/chemistry_example.md](docs/chemistry_example.md).

## Package structure

WASPPY combines generated and hand-written Python code with a native extension:

| Component | Purpose |
|---|---|
| `wasp.i` | SWIG interface for the C++ APIs |
| `wasp.py` | SWIG-generated Python module |
| `_wasp` | Native extension linked from the CMake-built WASP libraries |
| `Database.py` | Programmatic input-definition API |
| `sch2db.py` | Schema-to-database conversion support |
| `db2doc.py` | Database-to-documentation generation support |
| `test/` | Binding, API, and wheel tests |

CMake compiles the WASP libraries and links their objects into `_wasp`. The
wheel target packages that prebuilt extension; setuptools does not compile a
second copy of the C++ sources.

The extension uses Python's stable ABI with Python 3.10 as its minimum version.
Release wheels are platform-specific: Linux x86-64, Windows x86-64, and macOS
universal2 (`arm64` and `x86_64`).

## Configure and build

WASPPY requires CMake 3.26 or newer, Python 3.10 or newer, SWIG, and the usual
C++ build tools. From the repository root:

```shell
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -Dwasp_ENABLE_ALL_PACKAGES=ON \
  -Dwasp_ENABLE_TESTS=ON \
  -DWASP_ENABLE_SWIG=ON \
  -DPython3_EXECUTABLE="$(command -v python)"
cmake --build build --parallel
```

See the repository [README](../README.md) for the complete build options and
[CONTRIBUTING.md](../CONTRIBUTING.md) for development environment and
multi-configuration generator guidance.

## Test the bindings

Run the configured WASPPY tests with:

```shell
ctest --test-dir build -R "(WaspPy|wasppy)" --output-on-failure
```

The binding tests exercise the generated Python layer against the native
extension. When changing packaging, also build and test the artifact that users
will install:

```shell
cmake --build build --target wasp_wheel --parallel
python ci/verify_wheel.py \
  --wheel-dir build/wasppy/dist \
  --test-dir wasppy/test
```

The wheel verifier installs the wheel into an isolated virtual environment,
checks its metadata and native extension, imports the public modules from
outside the source tree, runs a parser smoke test, and executes the packaged
binding test suite.

On macOS, a universal2 build must contain both requested architectures in the
extension. Configure with:

```shell
cmake -S . -B build \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 \
  -DWASP_ENABLE_SWIG=ON
```

The build fails rather than silently publishing a single-architecture wheel if
either architecture is missing.

## Python interfaces

The primary Python interfaces are:

- `Interpreter` and `Syntax` for SON, HIT, DDI, and EDDI documents
- `WaspNode` and `VectorWaspNode` for parse-tree navigation
- `Database.InputObject` for programmatic input definitions and conversion
- the LSP bindings for language-server integrations

Nodes are non-owning views into interpreter-managed storage. Keep the
`Interpreter` alive for as long as any nodes obtained from it are in use.

For installation and introductory examples, read [PYPI.md](PYPI.md). For a
complete schema, navigation, and `InputObject` walkthrough, read the
[chemistry tutorial](docs/chemistry_example.md).

## Packaging documentation

The two top-level WASPPY documents intentionally serve different audiences:

- `README.md` is the repository and developer guide. Its relative links resolve
  on the GitLab instance hosting the working checkout.
- `PYPI.md` is the public package description. It uses absolute links to the
  public WASP repository because PyPI cannot resolve repository-relative links.

During the CMake build, `PYPI.md` is copied to the build directory as
`README.md`. `setup.py` reads that generated file as the wheel's long
description. Edit `PYPI.md` for PyPI-facing installation or usage changes, and
edit this file for source-tree build or maintenance changes.

Before publishing, run:

```shell
python -m twine check build/wasppy/dist/*
```

The release workflow and required checks are documented in
[CONTRIBUTING.md](../CONTRIBUTING.md).
