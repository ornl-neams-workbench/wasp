set -eo pipefail

function ecc () {
    "$@"
    if [[ $? -ne 0 ]]; then exit 1; fi
}

if [[ -d build  ]]; then
   rm -rf build
fi

mkdir -p build/miniconda3
cd build

# Setup a conda install
wget https://code-int.ornl.gov/lefebvre/miniconda/-/raw/main/Miniconda3-py310_24.5.0-0-Linux-x86_64.sh -O ${PWD}/miniconda3/miniconda.sh
bash ${PWD}/miniconda3/miniconda.sh -b -u -p ${PWD}/miniconda3
eval "$(${PWD}/miniconda3/bin/conda shell.bash hook 2> /dev/null)"
#export CONDA_NUMBER_CHANNEL_NOTICES=0
conda env create -f ../ci/env.yml
conda activate wasp_ci
pip install auditwheel
pip install patchelf

ecc cmake -DBUILDNAME="$(uname -s)-GCC-4.8.5-Release-${CI_COMMIT_REF_NAME}" \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -Dwasp_ENABLE_TESTS=ON \
      -DWASP_ENABLE_SWIG=ON \
      -DPython3_EXECUTABLE=`which python`\
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
      -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
      ..

export CMAKE_BUILD_PARALLEL_LEVEL=8

ecc ctest --output-on-failure \
      -D ExperimentalStart \
      -D ExperimentalBuild \
      -D ExperimentalTest 

# clean up prior config for the next bundle config
rm -rf CMake*

ecc cmake -DBUILDNAME="$(uname -s)-GCC-4.8.5-Bundle-${CI_COMMIT_REF_NAME}" \
      -DCPACK_PACKAGE_NAME=WASP \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      -Dwasp_ENABLE_TESTS:BOOL=OFF \
      -Dwasp_ENABLE_SWIG=ON \
      -Dwasp_ENABLE_ALL_PACKAGES:BOOL=ON \
      -DPython3_EXECUTABLE=`which python`\
      -Dwasp_ENABLE_testframework:BOOL=OFF \
      -Dwasp_ENABLE_googletest:BOOL=OFF \
      -Dwasp_ENABLE_wasppy:BOOL=ON \
      -Dwasp_ENABLE_INSTALL_CMAKE_CONFIG_FILES:BOOL=ON \
      -Dwasp_GENERATE_EXPORT_FILE_DEPENDENCIES:BOOL=ON \
      -Dwasp_ENABLE_CPACK_PACKAGING:BOOL=ON \
      -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
      -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
      ..

ecc make -j 8 package

WHEELHOUSE=${CI_PROJECT_DIR}/build/wasppy/wheelhouse
mkdir -p "${WHEELHOUSE}"
ecc auditwheel repair \
    -w "${WHEELHOUSE}" \
    --plat manylinux_2_34_x86_64 \
    "${CI_PROJECT_DIR}"/build/wasppy/dist/ornl_wasp*.whl
ecc auditwheel show "${WHEELHOUSE}"/*.whl
ecc python -m twine check "${WHEELHOUSE}"/*.whl
ecc python "${CI_PROJECT_DIR}/ci/verify_wheel.py" \
    --wheel-dir "${WHEELHOUSE}" \
    --test-dir "${CI_PROJECT_DIR}/wasppy/test"

PYTHON314_ENV=${CI_PROJECT_DIR}/build/python314
ecc conda create \
    --yes \
    --prefix "${PYTHON314_ENV}" \
    --channel conda-forge \
    --override-channels \
    python=3.14
ecc "${PYTHON314_ENV}/bin/python" "${CI_PROJECT_DIR}/ci/verify_wheel.py" \
    --wheel-dir "${WHEELHOUSE}" \
    --test-dir "${CI_PROJECT_DIR}/wasppy/test"

# Copy bundle parts up to parent directory to avoid artifact
# having build directory
cp WASP-*-Linux.sh ../
cp *.whl ../
cp waspConfig_install.cmake ../
