#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="${CI_PROJECT_DIR:-$(cd "${SCRIPT_DIR}/.." && pwd)}"
PROJECT_DIR="$(cd "${PROJECT_DIR}" && pwd)"
GENERATOR="${WASP_CPACK_GENERATOR:?WASP_CPACK_GENERATOR is required}"
PLATFORM="${WASP_CPACK_PLATFORM:?WASP_CPACK_PLATFORM is required}"
INSTALLER_URL="${WASP_MINICONDA_INSTALLER_URL:?WASP_MINICONDA_INSTALLER_URL is required}"
BUILD_DIR="${WASP_CPACK_BUILD_DIR:-${PROJECT_DIR}/build-cpack-${PLATFORM}}"
OUTPUT_DIR="${PROJECT_DIR}/dist/cpack"
BUILD_JOBS="${WASP_BUILD_JOBS:-8}"

if [[ "${BUILD_DIR}" != /* ]]; then
    BUILD_DIR="${PROJECT_DIR}/${BUILD_DIR}"
fi
if [[ -z "${BUILD_DIR}" || "${BUILD_DIR}" == "/" ||
      "${BUILD_DIR}" == "${PROJECT_DIR}" ]]; then
    echo "Refusing to use unsafe build directory '${BUILD_DIR}'" >&2
    exit 1
fi

rm -rf "${BUILD_DIR}"
rm -rf "${OUTPUT_DIR}"
mkdir -p "${BUILD_DIR}" "${OUTPUT_DIR}"

MINICONDA_DIR="${BUILD_DIR}/miniconda3"
INSTALLER="${BUILD_DIR}/miniconda.sh"
curl --fail --location --retry 3 "${INSTALLER_URL}" --output "${INSTALLER}"
bash "${INSTALLER}" -b -p "${MINICONDA_DIR}"
"${MINICONDA_DIR}/bin/conda" env create \
    --prefix "${BUILD_DIR}/wasp_ci" \
    --file "${PROJECT_DIR}/ci/env.yml"

CMAKE="${BUILD_DIR}/wasp_ci/bin/cmake"
CPACK="${BUILD_DIR}/wasp_ci/bin/cpack"
CMAKE_ARGS=(
    -S "${PROJECT_DIR}"
    -B "${BUILD_DIR}/package"
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=OFF
    -DCPACK_GENERATOR="${GENERATOR}"
    -DWASP_CPACK_PLATFORM="${PLATFORM}"
    -DWASP_ENABLE_SWIG=OFF
    -Dwasp_ENABLE_ALL_PACKAGES=ON
    -Dwasp_ENABLE_CPACK_PACKAGING=ON
    -Dwasp_ENABLE_TESTS=OFF
    -Dwasp_ENABLE_testframework=OFF
    -Dwasp_ENABLE_wasppy=OFF
)

if [[ -n "${WASP_CPACK_ARCHITECTURES:-}" ]]; then
    CMAKE_ARGS+=("-DCMAKE_OSX_ARCHITECTURES=${WASP_CPACK_ARCHITECTURES}")
fi

"${CMAKE}" "${CMAKE_ARGS[@]}"
"${CMAKE}" --build "${BUILD_DIR}/package" \
    --parallel "${BUILD_JOBS}"

"${CMAKE}" -E chdir "${BUILD_DIR}/package" \
    "${CPACK}" --config CPackConfig.cmake -G "${GENERATOR}"

case "${GENERATOR}" in
    STGZ) extension="sh" ;;
    productbuild) extension="pkg" ;;
    *)
        echo "Unsupported Unix CPack generator '${GENERATOR}'" >&2
        exit 1
        ;;
esac

packages=()
while IFS= read -r package; do
    packages+=("${package}")
done < <(find "${BUILD_DIR}/package" -maxdepth 1 -type f \
    -name "*.${extension}" -print)

if [[ ${#packages[@]} -eq 0 ]]; then
    echo "CPack did not produce a .${extension} artifact" >&2
    exit 1
fi

cp "${packages[@]}" "${OUTPUT_DIR}/"
"${CMAKE}" -E sha256sum "${OUTPUT_DIR}"/*."${extension}" \
    > "${OUTPUT_DIR}/SHA256SUMS-${PLATFORM}.txt"
