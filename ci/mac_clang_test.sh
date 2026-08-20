set -eo pipefail

mkdir build
cd build

#git clone https://code.ornl.gov/warroom/miniconda.git
curl -O https://code-int.ornl.gov/lefebvre/miniconda/-/raw/main/Miniconda3-py310_24.5.0-0-MacOSX-arm64.sh
bash ./Miniconda3-py310_24.5.0-0-MacOSX-arm64.sh -b -p ${PWD}/miniconda3
eval "$(${PWD}/miniconda3/bin/conda shell.bash hook 2> /dev/null)"
export CONDA_NUMBER_CHANNEL_NOTICES=0
conda env create -f ../ci/env.yml
conda activate wasp_ci
pip install delocate build

cmake -DBUILDNAME="$(uname -s)-Release-${CI_COMMIT_REF_NAME}" \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -DWASP_ENABLE_SWIG=ON \
      -Dwasp_ENABLE_TESTS=ON \
      -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      ..

export CMAKE_BUILD_PARALLEL_LEVEL=8

TEST_STATUS=0
ctest --output-on-failure \
      -D ExperimentalStart \
      -D ExperimentalBuild \
      -D ExperimentalTest || TEST_STATUS=$?

WHEELHOUSE=${CI_PROJECT_DIR}/build/wasppy/wheelhouse
mkdir -p "${WHEELHOUSE}"
delocate-wheel \
      -w "${WHEELHOUSE}" \
      "${CI_PROJECT_DIR}"/build/wasppy/dist/ornl_wasp*.whl
delocate-listdeps "${WHEELHOUSE}"/*.whl
python -m twine check "${WHEELHOUSE}"/*.whl
python "${CI_PROJECT_DIR}/ci/verify_wheel.py" \
      --wheel-dir "${WHEELHOUSE}" \
      --test-dir "${CI_PROJECT_DIR}/wasppy/test" \
      --require-arch arm64 \
      --require-arch x86_64

PYTHON314_ENV=${CI_PROJECT_DIR}/build/python314
conda create \
      --yes \
      --prefix "${PYTHON314_ENV}" \
      --channel conda-forge \
      --override-channels \
      python=3.14
"${PYTHON314_ENV}/bin/python" "${CI_PROJECT_DIR}/ci/verify_wheel.py" \
      --wheel-dir "${WHEELHOUSE}" \
      --test-dir "${CI_PROJECT_DIR}/wasppy/test" \
      --require-arch arm64 \
      --require-arch x86_64

exit "${TEST_STATUS}"
