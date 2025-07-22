TRIBITS_SUBPACKAGE(py)

file(GLOB MODULE  ${CMAKE_CURRENT_SOURCE_DIR}/*.py )

# Copy module and test directory into build tree for python testing
file(COPY ${MODULE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "test"  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "drive"  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


IF(Python3_EXECUTABLE)
  # Must use absolute path or Python __file__ test directives are incorrect
  ADD_TEST( tstWaspPy_test_Xml2Obj python ${CMAKE_CURRENT_BINARY_DIR}/test/test_Xml2Obj.py )
  ADD_TEST( tstWaspPy_test_Json2Python python ${CMAKE_CURRENT_BINARY_DIR}/test/test_Json2Python.py  )
  ADD_TEST( tstWaspPy_test_DDI2Python python ${CMAKE_CURRENT_BINARY_DIR}/test/test_DDI2Python.py  )
  ADD_TEST( tstWaspPy_test_Drive python ${CMAKE_CURRENT_BINARY_DIR}/test/test_Drive.py  )
  # Disabled while CI slurm is inoperable
  # IF (CMAKE_SYSTEM_NAME STREQUAL  "Linux")
  #   ADD_TEST( tstWaspPy_test_Drive_Scheduled python ${CMAKE_CURRENT_BINARY_DIR}/test/test_Drive_Scheduled.py  )
  # ENDIF()
ENDIF()

# Add binary directory to includes so wasp_wrap.cxx can include wasp_wrap.h
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Add test directory
TRIBITS_ADD_TEST_DIRECTORIES(test)

# Install the python module
install(DIRECTORY .
        DESTINATION "wasppy"
        PATTERN "./*.py"
)

################################################################################
################################################################################

# Have to have these in order to generate the wrappers
IF(NOT DEFINED WASP_ENABLE_SWIG)
  set(WASP_ENABLE_SWIG OFF)
ENDIF()
IF(WASP_ENABLE_SWIG)
find_package(SWIG REQUIRED)

#This will replaces cmake variables but not generator expressions if the time comes
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasp.i.in ${CMAKE_CURRENT_BINARY_DIR}/wasp.i.in)
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wasp.i INPUT ${CMAKE_CURRENT_BINARY_DIR}/wasp.i.in)

# Run swig manually vice using setuptools for better control of options
add_custom_target(wasppy ALL
  COMMAND
  #TODO The include format will get cumbersome....
    "${SWIG_EXECUTABLE}" -c++ -python
    -I${wasp_SOURCE_DIR}/waspcore 
    -I${wasp_SOURCE_DIR}/waspson 
    -I${wasp_SOURCE_DIR}/waspddi 
    -I${wasp_SOURCE_DIR}/waspeddi 
    -I${wasp_SOURCE_DIR}/wasphit 
    -I${wasp_SOURCE_DIR}/waspsiren 
    -I${wasp_SOURCE_DIR}/wasphive 
    -I${wasp_SOURCE_DIR}/wasplsp
    wasp.i
  BYPRODUCTS
    wasp_wrap.cxx
    wasp.py
)

#Create setup.py using a version controlled skeleton with generator expressions
#and cmake variables
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/README.md
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_BINARY_DIR}/README.md
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../LICENSE
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.cfg
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg ${CMAKE_CURRENT_BINARY_DIR}/setup.cfg
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Database.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Database.py ${CMAKE_CURRENT_BINARY_DIR}/Database.py
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/Database.py)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/test_swig_bindings.py ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/test/test_swig_bindings.py)


file(READ ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in WASPPY_SETUP)
string(CONFIGURE "${WASPPY_SETUP}" WASPPY_SETUP_CONFIG)
if(CMAKE_CONFIGURATION_TYPES)
  file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup$<$<NOT:$<CONFIG:Release>>:$<CONFIG>>.py CONTENT "${WASPPY_SETUP_CONFIG}")
else()
  file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py CONTENT "${WASPPY_SETUP_CONFIG}")
endif()

#Execute the setup file to generate a wheel
add_custom_target(wasp_wheel ALL "${Python3_EXECUTABLE}" -m build > python_build.log 2>&1
                  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Database.py ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py)
add_dependencies(wasp_wheel wasppy waspcore waspson waspddi waspeddi wasphit waspsiren wasphive wasplsp)

#Configure a basic unit test of the python bindings. In this case the wheels is 
#installed as editable to test in a "natural" way and removed at the end.
message(${Python3_EXECUTABLE})
TRIBITS_ADD_ADVANCED_TEST( test_swig_all
  OVERALL_WORKING_DIRECTORY
    ${CMAKE_CURRENT_BUILD_DIR}
  OVERALL_NUM_MPI_PROCS
    1
  TEST_0
    CMND "${Python3_EXECUTABLE}"
    ARGS -m pip install -e ${CMAKE_CURRENT_BINARY_DIR}
  TEST_1
    CMND "${Python3_EXECUTABLE}"
    ARGS -m unittest test/test_swig_bindings.py -v
  TEST_2
    CMND "${Python3_EXECUTABLE}"
    ARGS -m unittest test/test_lsp_server_methods.py -v
  TEST_3
    CMND "${Python3_EXECUTABLE}"
    ARGS -m unittest test/test_lsp_server_running.py -v
  TEST_4
    CMND "${Python3_EXECUTABLE}"
    ARGS -m pip uninstall --yes ornl-wasp
  TIMEOUT 3000
  CATEGORIES BASIC
)
ENDIF()

#TODO At this point the wheel has been generated. Need the install logic

TRIBITS_SUBPACKAGE_POSTPROCESS()
