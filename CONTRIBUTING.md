# Contributing to WASP

Thank you for contributing to the Workbench Analysis Sequence Processor
(WASP). This guide describes the development workflow expected for changes to
the C++, CMake, grammar, and Python components in this repository.

## Reporting issues

Use the [WASP issue tracker](https://code.ornl.gov/neams-workbench/wasp/-/issues)
for reproducible bugs and focused enhancement requests.

A useful bug report includes:

- The WASP version, tag, or commit.
- The operating system and processor architecture.
- Compiler and CMake versions.
- Python and SWIG versions when WASPPY is involved.
- The complete CMake configuration command.
- A minimal input and schema that reproduce the problem.
- Complete diagnostics, test output, or compiler output as text.
- The expected behavior and the observed behavior.

Remove sensitive information from inputs and logs before attaching them.

## Development requirements

The core requirements are:

- A C++ compiler with C++11 support.
- CMake 3.26 or newer.
- Git with submodule support.

WASPPY development additionally requires:

- Python 3.10 or newer.
- SWIG.
- The Python `build` package for wheel generation.

Platform-specific and optional package requirements are reported during CMake
configuration. The CI environment definition in
[`ci/env.yml`](ci/env.yml) is the reference environment for Python-enabled
builds.

## Clone and initialize submodules

Clone the repository and initialize its submodules:

```shell
git clone https://code.ornl.gov/neams-workbench/wasp.git
cd wasp
git submodule update --init --recursive
```

If an existing checkout changes submodule revisions, run the submodule command
again before configuring.

## Configure and build

Use an out-of-source build directory:

```shell
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -Dwasp_ENABLE_ALL_PACKAGES=ON \
  -Dwasp_ENABLE_TESTS=ON

cmake --build build --parallel
```

For multi-configuration generators such as Visual Studio, select the
configuration when building:

```shell
cmake --build build --config Debug --parallel
```

Do not commit build directories, generated binaries, caches, virtual
environments, or test output.

## Run tests

Run the complete configured test suite:

```shell
ctest --test-dir build --output-on-failure
```

For multi-configuration generators:

```shell
ctest --test-dir build -C Debug --output-on-failure
```

List available tests or run a focused subset with:

```shell
ctest --test-dir build -N
ctest --test-dir build -R <test-name-or-pattern> --output-on-failure
```

Behavior changes should include a focused regression test. Test both successful
behavior and relevant failure or boundary conditions. A test should fail
against the deficient implementation and pass with the proposed change.

## WASPPY development

Enable SWIG bindings when configuring:

```shell
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -Dwasp_ENABLE_ALL_PACKAGES=ON \
  -Dwasp_ENABLE_TESTS=ON \
  -DWASP_ENABLE_SWIG=ON \
  -DPython3_EXECUTABLE="$(command -v python)"

cmake --build build --parallel
ctest --test-dir build -R "(WaspPy|wasppy)" --output-on-failure
```

Build the Python wheel with:

```shell
cmake --build build --target wasp_wheel --parallel
```

The deployable wheel must be tested rather than only importing modules from the
source or build tree. The CI verifier installs the wheel into a temporary
virtual environment and runs the binding and language-server tests:

```shell
python ci/verify_wheel.py \
  --wheel-dir build/wasppy/dist \
  --test-dir wasppy/test
```

For a macOS universal2 wheel, also verify both native architectures:

```shell
python ci/verify_wheel.py \
  --wheel-dir build/wasppy/dist \
  --test-dir wasppy/test \
  --require-arch arm64 \
  --require-arch x86_64
```

Platform repair and final deployment checks use `delocate` on macOS,
`auditwheel` on Linux, and `delvewheel` on Windows. The repaired wheel—not the
unrepaired build artifact—is the release artifact.

When changing public Python behavior:

- Update or add tests under `wasppy/test`.
- Update `wasppy/PYPI.md` when installation or public usage changes, and
  `wasppy/README.md` when source-tree build or maintenance guidance changes.
- Update the extended WASPPY tutorial when its examples are affected.
- Preserve the CPython stable ABI unless the compatibility change is
  intentional and documented.

## Formatting

The repository [`.clang-format`](.clang-format) is the source of truth for C++
formatting. Run `clang-format` from within the checkout so it discovers this
configuration:

```shell
clang-format -i path/to/changed_file.cpp path/to/changed_header.h
```

Format only files or regions relevant to the change. Do not combine functional
changes with unrelated whole-file reformatting. Always review the resulting
diff.

Before submitting, check for whitespace errors:

```shell
git diff --check
```

For Python, CMake, shell, grammar, and documentation files, follow the
surrounding style and keep changes focused. Shell and CI changes should preserve
command failures and print actionable output.

## Naming conventions

### Files

- C++ implementation files use `.cpp`.
- C++ headers use `.h`.
- Template implementation headers use `.i.h`.
- Flex grammar files use `.lex`.
- GNU Bison grammar files use `.bison`.

Some generated parser files use other extensions. Do not rename generated
files merely to satisfy these conventions.

### C++

- Classes and other types use `CamelCase`.
- Functions and methods use `lower_snake_case`.
- Local variables and parameters use `lower_snake_case`.
- Class data members use the `m_` prefix followed by `lower_snake_case`.
- A member getter normally uses the member name without the `m_` prefix.

For example:

```cpp
namespace wasp
{
class CommandLine
{
  public:
    int argument_count() const;

  private:
    int    m_argument_count;
    char** m_argument_values;
};
}
```

Apply new naming conventions to new or substantially modified interfaces.
Avoid broad renaming of stable public APIs unless the compatibility impact is
part of the change.

## Generated files

WASP uses Flex, Bison, SWIG, and CMake generation. Edit the controlling source
file rather than a build-tree output:

- Edit `.lex` and `.bison` grammar sources rather than generated lexer/parser
  output.
- Edit `wasppy/wasp.i.in` and wrapped headers rather than `wasp_wrap.cxx` or
  generated `wasp.py`.
- Edit CMake templates such as `.in` files rather than their configured
  build-tree copies.

Do not commit generated build-tree files unless the repository already tracks
that specific artifact and the change intentionally updates it.

## Documentation and changelog

Update documentation in the same change as the behavior it describes.
Examples should be runnable and should use supported public APIs.

Update [`CHANGELOG.md`](CHANGELOG.md) for user-visible additions, behavior
changes, compatibility changes, and significant fixes. Do not add a release
date or version unless preparing an actual release.

## Branches and commits

Create a focused topic branch for one issue or coherent change. Topic branches
should normally live for days or weeks, not months. If longer work is
unavoidable, synchronize it with the target branch regularly.

Prefer small, reviewable commits with imperative subject lines. Keep unrelated
refactoring, formatting, generated output, and behavior changes in separate
commits or merge requests.

## Merge-request checklist

Before requesting review:

- [ ] The change has a clear scope and description.
- [ ] New behavior and bug fixes have regression tests.
- [ ] The configured test suite passes locally.
- [ ] WASPPY wheels were tested from an isolated installation when applicable.
- [ ] Documentation and examples match the implementation.
- [ ] `CHANGELOG.md` is updated when the change is user-visible.
- [ ] No unrelated formatting or generated build output is included.
- [ ] `git diff --check` passes.
- [ ] GitLab CI passes on the applicable Linux, macOS, and Windows jobs.

In the merge-request description, explain the problem, the approach, the tests
performed, and any compatibility or performance implications.
