#!/usr/bin/env python3
"""Install and test one deployable WASP wheel in an isolated environment."""

import argparse
import shutil
import subprocess
import sys
import tempfile
import venv
from pathlib import Path
from zipfile import ZipFile


TEST_MODULES = (
    "test.test_swig_bindings",
    "test.test_lsp_server_methods",
    "test.test_lsp_server_running",
)


def run(command, *, cwd=None):
    """Run a command and fail immediately while preserving its output."""
    subprocess.run(command, cwd=cwd, check=True)


def find_wheel(wheel_dir):
    """Return the only wheel in wheel_dir so stale artifacts cannot be tested."""
    wheels = sorted(wheel_dir.glob("*.whl"))
    if len(wheels) != 1:
        raise RuntimeError(
            f"Expected exactly one wheel in {wheel_dir}, found {len(wheels)}: "
            f"{[wheel.name for wheel in wheels]}"
        )
    return wheels[0].resolve()


def verify_tags(wheel):
    """Ensure the filename and embedded wheel tags agree on cp310-abi3."""
    try:
        _, _, python_tag, abi_tag, platform_tag = wheel.stem.rsplit("-", 4)
    except ValueError as error:
        raise RuntimeError(f"Invalid wheel filename: {wheel.name}") from error

    if (python_tag, abi_tag) != ("cp310", "abi3"):
        raise RuntimeError(
            f"Expected a cp310-abi3 wheel, found {python_tag}-{abi_tag}"
        )

    filename_tag = f"{python_tag}-{abi_tag}-{platform_tag}"
    with ZipFile(wheel) as archive:
        metadata_files = [
            name for name in archive.namelist() if name.endswith(".dist-info/WHEEL")
        ]
        if len(metadata_files) != 1:
            raise RuntimeError(
                f"Expected one WHEEL metadata file, found {metadata_files}"
            )
        metadata = archive.read(metadata_files[0]).decode("utf-8")

    metadata_tags = {
        line.removeprefix("Tag: ").strip()
        for line in metadata.splitlines()
        if line.startswith("Tag: ")
    }
    if filename_tag not in metadata_tags:
        raise RuntimeError(
            f"Filename tag {filename_tag!r} is not present in WHEEL metadata: "
            f"{sorted(metadata_tags)}"
        )


def verify_architectures(wheel, architectures):
    """Verify requested Mach-O slices without importing from the build tree."""
    if not architectures:
        return
    if sys.platform != "darwin":
        raise RuntimeError("--require-arch is only supported on macOS")

    with tempfile.TemporaryDirectory(prefix="wasp-wheel-arch-") as temporary:
        temporary_path = Path(temporary)
        with ZipFile(wheel) as archive:
            extensions = [
                name
                for name in archive.namelist()
                if name.endswith((".so", ".dylib"))
            ]
            if not extensions:
                raise RuntimeError(f"No Mach-O extension found in {wheel}")
            archive.extractall(temporary_path, extensions)

        for extension in extensions:
            extension_path = temporary_path / extension
            run(["lipo", str(extension_path), "-verify_arch", *architectures])


def venv_python(environment):
    """Return the Python executable inside a virtual environment."""
    if sys.platform == "win32":
        return environment / "Scripts" / "python.exe"
    return environment / "bin" / "python"


def verify_installation(wheel, test_dir):
    """Install the wheel and run tests where source/build imports are impossible."""
    with tempfile.TemporaryDirectory(prefix="wasp-wheel-test-") as temporary:
        temporary_path = Path(temporary)
        environment = temporary_path / "venv"
        venv.EnvBuilder(with_pip=True, clear=True).create(environment)
        python = venv_python(environment)

        run(
            [
                str(python),
                "-m",
                "pip",
                "install",
                "--no-deps",
                "--no-index",
                str(wheel),
            ],
            cwd=temporary_path,
        )
        run([str(python), "-m", "pip", "check"], cwd=temporary_path)

        smoke_test = """
from pathlib import Path
import Database
import _wasp
import sch2db
import wasp

extension = Path(_wasp.__file__).resolve()
environment = Path(sys.prefix).resolve()
if environment not in extension.parents:
    raise RuntimeError(f"_wasp was not imported from the test environment: {extension}")
if not hasattr(wasp, "HIVE"):
    raise RuntimeError("Installed wasp module does not expose HIVE")
print(f"Imported deployed extension: {extension}")
"""
        run(
            [str(python), "-I", "-c", "import sys\n" + smoke_test],
            cwd=temporary_path,
        )

        shutil.copytree(test_dir, temporary_path / "test")
        for module in TEST_MODULES:
            run(
                [str(python), "-m", "unittest", module, "-v"],
                cwd=temporary_path,
            )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--wheel-dir", required=True, type=Path)
    parser.add_argument("--test-dir", required=True, type=Path)
    parser.add_argument(
        "--require-arch",
        action="append",
        default=[],
        help="Mach-O architecture required in every bundled native library",
    )
    arguments = parser.parse_args()

    wheel = find_wheel(arguments.wheel_dir.resolve())
    test_dir = arguments.test_dir.resolve()
    if not test_dir.is_dir():
        raise RuntimeError(f"Test directory does not exist: {test_dir}")

    print(f"Verifying deployable wheel: {wheel}")
    verify_tags(wheel)
    verify_architectures(wheel, arguments.require_arch)
    verify_installation(wheel, test_dir)
    print(f"Deployable wheel passed: {wheel.name}")


if __name__ == "__main__":
    main()
