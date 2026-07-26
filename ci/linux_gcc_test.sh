#!/usr/bin/env bash

set -eo pipefail

# GitLab supplies CI_PROJECT_DIR and CI_COMMIT_REF_NAME.  Local runs derive
# equivalent values from this script's location and the current Git checkout.
# WASP_BUILD_DIR, WASP_BUILD_JOBS, WASP_MINICONDA_INSTALLER_URL, CC, CXX, and
# WASP_COMPILER_LABEL may be set to customize a local run.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="${CI_PROJECT_DIR:-$(cd "${SCRIPT_DIR}/.." && pwd)}"
PROJECT_DIR="$(cd "${PROJECT_DIR}" && pwd)"
BUILD_DIR="${WASP_BUILD_DIR:-${PROJECT_DIR}/build}"
if [[ "${BUILD_DIR}" != /* ]]; then
    BUILD_DIR="${PROJECT_DIR}/${BUILD_DIR}"
fi
BUILD_JOBS="${WASP_BUILD_JOBS:-8}"
MINICONDA_INSTALLER_URL="${WASP_MINICONDA_INSTALLER_URL:-https://code-int.ornl.gov/lefebvre/miniconda/-/raw/main/Miniconda3-py310_24.5.0-0-Linux-x86_64.sh}"
CC="${CC:-/usr/bin/gcc}"
CXX="${CXX:-/usr/bin/g++}"
COMPILER_LABEL="${WASP_COMPILER_LABEL:-GCC-4.8.5}"

if [[ -n "${CI_COMMIT_REF_NAME:-}" ]]; then
    BUILD_REF="${CI_COMMIT_REF_NAME}"
elif BUILD_REF="$(git -C "${PROJECT_DIR}" branch --show-current 2>/dev/null)" &&
     [[ -n "${BUILD_REF}" ]]; then
    :
elif BUILD_REF="$(git -C "${PROJECT_DIR}" rev-parse --short HEAD 2>/dev/null)"; then
    :
else
    BUILD_REF="local"
fi

if [[ -z "${BUILD_DIR}" || "${BUILD_DIR}" == "/" ||
      "${BUILD_DIR}" == "${PROJECT_DIR}" ]]; then
    echo "Refusing to use unsafe build directory '${BUILD_DIR}'" >&2
    exit 1
fi

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}/miniconda3"
cd "${BUILD_DIR}"

# Setup a conda install
wget "${MINICONDA_INSTALLER_URL}" -O "${PWD}/miniconda3/miniconda.sh"
bash "${PWD}/miniconda3/miniconda.sh" -b -u -p "${PWD}/miniconda3"
eval "$(${PWD}/miniconda3/bin/conda shell.bash hook 2> /dev/null)"
#export CONDA_NUMBER_CHANNEL_NOTICES=0
conda env create -f "${PROJECT_DIR}/ci/env.yml"
conda activate wasp_ci
pip install auditwheel
pip install patchelf

cmake -DBUILDNAME="$(uname -s)-${COMPILER_LABEL}-Release-${BUILD_REF}" \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -Dwasp_ENABLE_TESTS=ON \
      -DWASP_ENABLE_SWIG=ON \
      -DPython3_EXECUTABLE="$(command -v python)" \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      -DCMAKE_CXX_COMPILER:FILEPATH="${CXX}" \
      -DCMAKE_C_COMPILER:FILEPATH="${CC}" \
      "${PROJECT_DIR}"

export CMAKE_BUILD_PARALLEL_LEVEL="${BUILD_JOBS}"

ctest --output-on-failure \
      -D ExperimentalStart \
      -D ExperimentalBuild \
      -D ExperimentalTest 

# clean up prior config for the next bundle config
rm -rf CMake*

cmake -DBUILDNAME="$(uname -s)-${COMPILER_LABEL}-Bundle-${BUILD_REF}" \
      -DCPACK_PACKAGE_NAME=WASP \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      -Dwasp_ENABLE_TESTS:BOOL=OFF \
      -Dwasp_ENABLE_SWIG=ON \
      -Dwasp_ENABLE_ALL_PACKAGES:BOOL=ON \
      -DPython3_EXECUTABLE="$(command -v python)" \
      -Dwasp_ENABLE_testframework:BOOL=OFF \
      -Dwasp_ENABLE_googletest:BOOL=OFF \
      -Dwasp_ENABLE_wasppy:BOOL=ON \
      -Dwasp_ENABLE_INSTALL_CMAKE_CONFIG_FILES:BOOL=ON \
      -Dwasp_GENERATE_EXPORT_FILE_DEPENDENCIES:BOOL=ON \
      -Dwasp_ENABLE_CPACK_PACKAGING:BOOL=ON \
      -DCMAKE_CXX_COMPILER:FILEPATH="${CXX}" \
      -DCMAKE_C_COMPILER:FILEPATH="${CC}" \
      "${PROJECT_DIR}"

cmake --build . --target package --parallel "${BUILD_JOBS}"

WHEELHOUSE="${BUILD_DIR}/wasppy/wheelhouse"
mkdir -p "${WHEELHOUSE}"
auditwheel repair \
    -w "${WHEELHOUSE}" \
    --plat manylinux_2_34_x86_64 \
    "${BUILD_DIR}"/wasppy/dist/ornl_wasp*.whl
auditwheel show "${WHEELHOUSE}"/*.whl
python -m twine check "${WHEELHOUSE}"/*.whl
python "${PROJECT_DIR}/ci/verify_wheel.py" \
    --wheel-dir "${WHEELHOUSE}" \
    --test-dir "${PROJECT_DIR}/wasppy/test"

PYTHON314_ENV="${BUILD_DIR}/python314"
conda create \
    --yes \
    --prefix "${PYTHON314_ENV}" \
    --channel conda-forge \
    --override-channels \
    python=3.14
"${PYTHON314_ENV}/bin/python" "${PROJECT_DIR}/ci/verify_wheel.py" \
    --wheel-dir "${WHEELHOUSE}" \
    --test-dir "${PROJECT_DIR}/wasppy/test"

# Copy bundle parts up to parent directory to avoid artifact
# having build directory
cp WASP-*-Linux.sh "${PROJECT_DIR}/"
cp "${WHEELHOUSE}"/*.whl "${PROJECT_DIR}/"
cp waspConfig_install.cmake "${PROJECT_DIR}/"
