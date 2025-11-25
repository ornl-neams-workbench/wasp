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

ctest --output-on-failure \
      -D ExperimentalStart \
      -D ExperimentalBuild \
      -D ExperimentalTest 

delocate-wheel -w ${CI_PROJECT_DIR}/build/wasppy/dist ${CI_PROJECT_DIR}/build/wasppy/dist/ornl_wasp*.whl

# On CI machines the plat name is changed because of OS version... revert to 
WHL=${CI_PROJECT_DIR}/build/wasppy/dist/ornl_wasp-4.4.1-cp38-abi3-macosx_11_0_universal2.whl
if [ -f ${WHL} ]; then
mv -v ${WHL} ${CI_PROJECT_DIR}/build/wasppy/dist/ornl_wasp-4.4.1-cp38-abi3-macosx_10_6_universal2.whl
fi
