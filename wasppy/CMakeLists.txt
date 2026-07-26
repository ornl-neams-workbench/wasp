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
  FIND_PACKAGE(SWIG REQUIRED)
  FIND_PACKAGE(Python3 3.10 REQUIRED COMPONENTS Interpreter Development.SABIModule)

  SET(WASP_SWIG_INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/wasp.i")
  SET(WASP_SWIG_WRAPPER   "${CMAKE_CURRENT_BINARY_DIR}/wasp_wrap.cxx")
  SET(WASP_SWIG_HEADER    "${CMAKE_CURRENT_BINARY_DIR}/wasp_wrap.h")
  SET(WASP_SWIG_PYTHON    "${CMAKE_CURRENT_BINARY_DIR}/wasp.py")

  # Configure CMake variables first, then expand target generator expressions.
  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/wasp.i.in"
    "${CMAKE_CURRENT_BINARY_DIR}/wasp.i.in"
  )
  FILE(
    GENERATE
    OUTPUT "${WASP_SWIG_INTERFACE}"
    INPUT  "${CMAKE_CURRENT_BINARY_DIR}/wasp.i.in"
  )

  # Unlike an ALL custom target, this command runs only when the interface or a
  # directly wrapped header changes.
  SET(WASP_SWIG_HEADERS
    "${wasp_SOURCE_DIR}/waspcore/decl.h"
    "${wasp_SOURCE_DIR}/waspcore/Definition.h"
    "${wasp_SOURCE_DIR}/waspcore/Interpreter.h"
    "${wasp_SOURCE_DIR}/waspcore/Interpreter.i.h"
    "${wasp_SOURCE_DIR}/waspcore/Iterator.h"
    "${wasp_SOURCE_DIR}/waspcore/Object.h"
    "${wasp_SOURCE_DIR}/waspcore/TokenPool.h"
    "${wasp_SOURCE_DIR}/waspcore/TokenPool.i.h"
    "${wasp_SOURCE_DIR}/waspcore/TreeNodePool.h"
    "${wasp_SOURCE_DIR}/waspcore/TreeNodePool.i.h"
    "${wasp_SOURCE_DIR}/waspddi/DDInterpreter.h"
    "${wasp_SOURCE_DIR}/waspddi/DDInterpreter.i.h"
    "${wasp_SOURCE_DIR}/waspddi/DDINodeView.h"
    "${wasp_SOURCE_DIR}/waspddi/DDINodeView.i.h"
    "${wasp_SOURCE_DIR}/waspeddi/EDDInterpreter.h"
    "${wasp_SOURCE_DIR}/waspeddi/EDDInterpreter.i.h"
    "${wasp_SOURCE_DIR}/waspeddi/EDDINodeView.h"
    "${wasp_SOURCE_DIR}/waspeddi/EDDINodeView.i.h"
    "${wasp_SOURCE_DIR}/wasphit/HITInterpreter.h"
    "${wasp_SOURCE_DIR}/wasphit/HITInterpreter.i.h"
    "${wasp_SOURCE_DIR}/wasphit/HITNodeView.h"
    "${wasp_SOURCE_DIR}/wasphit/HITNodeView.i.h"
    "${wasp_SOURCE_DIR}/wasphive/HIVE.h"
    "${wasp_SOURCE_DIR}/wasphive/HIVE.i.h"
    "${wasp_SOURCE_DIR}/wasphive/InputDefinition.i.h"
    "${wasp_SOURCE_DIR}/wasplsp/ClientImpl.h"
    "${wasp_SOURCE_DIR}/wasplsp/Connection.h"
    "${wasp_SOURCE_DIR}/wasplsp/IOStreamConnection.h"
    "${wasp_SOURCE_DIR}/wasplsp/LSP.h"
    "${wasp_SOURCE_DIR}/wasplsp/ServerImpl.h"
    "${wasp_SOURCE_DIR}/wasplsp/SymbolIterator.h"
    "${wasp_SOURCE_DIR}/wasplsp/ThreadConnection.h"
    "${wasp_SOURCE_DIR}/waspsiren/SIRENInterpreter.h"
    "${wasp_SOURCE_DIR}/waspsiren/SIRENInterpreter.i.h"
    "${wasp_SOURCE_DIR}/waspsiren/SIRENResultSet.i.h"
    "${wasp_SOURCE_DIR}/waspson/SONInterpreter.h"
    "${wasp_SOURCE_DIR}/waspson/SONInterpreter.i.h"
    "${wasp_SOURCE_DIR}/waspson/SONNodeView.h"
    "${wasp_SOURCE_DIR}/waspson/SONNodeView.i.h"
  )
  ADD_CUSTOM_COMMAND(
    OUTPUT
      "${WASP_SWIG_WRAPPER}"
      "${WASP_SWIG_HEADER}"
      "${WASP_SWIG_PYTHON}"
    COMMAND
      "${SWIG_EXECUTABLE}" -c++ -python
      -I${wasp_SOURCE_DIR}/waspcore
      -I${wasp_SOURCE_DIR}/waspson
      -I${wasp_SOURCE_DIR}/waspddi
      -I${wasp_SOURCE_DIR}/waspeddi
      -I${wasp_SOURCE_DIR}/wasphit
      -I${wasp_SOURCE_DIR}/waspsiren
      -I${wasp_SOURCE_DIR}/wasphive
      -I${wasp_SOURCE_DIR}/wasplsp
      -o "${WASP_SWIG_WRAPPER}"
      "${WASP_SWIG_INTERFACE}"
    DEPENDS
      "${WASP_SWIG_INTERFACE}"
      "${CMAKE_CURRENT_SOURCE_DIR}/wasp.i.in"
      ${WASP_SWIG_HEADERS}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating WASP Python bindings with SWIG"
    VERBATIM
  )

  # Compile the wrapper once under CMake and link the existing WASP libraries.
  # Py_LIMITED_API and Python3::SABIModule keep the extension compatible with
  # CPython 3.10 and newer.
  # Build the module explicitly instead of Python3_add_library: this project
  # retains older CMake policies through TriBITS, while the helper's USE_SABI
  # argument parsing assumes newer policy behavior.
  ADD_LIBRARY(_wasp MODULE "${WASP_SWIG_WRAPPER}")
  TARGET_LINK_LIBRARIES(_wasp PRIVATE Python3::SABIModule)
  TARGET_COMPILE_DEFINITIONS(_wasp PRIVATE Py_LIMITED_API=0x030A0000)
  TARGET_INCLUDE_DIRECTORIES(
    _wasp
    PRIVATE
      "$<TARGET_PROPERTY:waspcore,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:waspson,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:waspddi,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:waspeddi,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:wasphit,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:waspsiren,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:wasphive,INCLUDE_DIRECTORIES>"
      "$<TARGET_PROPERTY:wasplsp,INCLUDE_DIRECTORIES>"
      "${CMAKE_CURRENT_BINARY_DIR}"
  )

  SET(WASP_PYTHON_LIBRARIES
    wasplsp
    wasphive
    waspsiren
    wasphit
    waspeddi
    waspddi
    waspson
    waspjson
    waspcore
  )
  # Consume the objects produced for the CMake libraries directly.  This keeps
  # the extension self-contained and also works with universal Mach-O objects;
  # some cross-toolchain `ar` implementations produce fat static archives that
  # Apple's linker cannot read.
  FOREACH(WASP_PYTHON_LIBRARY IN LISTS WASP_PYTHON_LIBRARIES)
    TARGET_SOURCES(_wasp PRIVATE "$<TARGET_OBJECTS:${WASP_PYTHON_LIBRARY}>")
  ENDFOREACH()
  SET_TARGET_PROPERTIES(
    _wasp
    PROPERTIES
      PREFIX ""
      SUFFIX ".${Python3_SOSABI}${CMAKE_SHARED_MODULE_SUFFIX}"
      POSITION_INDEPENDENT_CODE ON
  )
  FOREACH(WASP_PYTHON_LIBRARY IN LISTS WASP_PYTHON_LIBRARIES)
    SET_PROPERTY(
      TARGET ${WASP_PYTHON_LIBRARY}
      PROPERTY POSITION_INDEPENDENT_CODE ON
    )
  ENDFOREACH()
  IF(APPLE)
    # Cross-compilers distributed by conda/pixi may inject an arm64-only
    # libc++ ahead of the universal SDK stub.  Suppress that implicit runtime
    # and link both slices against the system's universal libc++.
    SET(WASP_MACOS_LIBCXX
        "${CMAKE_OSX_SYSROOT}/usr/lib/libc++.tbd")
    IF(NOT EXISTS "${WASP_MACOS_LIBCXX}")
      MESSAGE(FATAL_ERROR
        "The macOS SDK does not provide ${WASP_MACOS_LIBCXX}")
    ENDIF()
    TARGET_LINK_OPTIONS(
      _wasp
      PRIVATE
        "-nostdlib++"
        "${WASP_MACOS_LIBCXX}"
        "-Wl,-headerpad_max_install_names"
    )

    # Remove a toolchain-local runtime path when the compiler injected one.
    GET_FILENAME_COMPONENT(WASP_COMPILER_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    GET_FILENAME_COMPONENT(WASP_COMPILER_PREFIX "${WASP_COMPILER_BIN_DIR}" DIRECTORY)
    SET(WASP_COMPILER_LIB_DIR "${WASP_COMPILER_PREFIX}/lib")
    IF(EXISTS "${WASP_COMPILER_LIB_DIR}/libc++.dylib")
      SET_PROPERTY(
        TARGET _wasp
        APPEND
        PROPERTY LINK_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/remove_macos_rpath.cmake"
      )
      ADD_CUSTOM_COMMAND(
        TARGET _wasp
        POST_BUILD
        COMMAND
          "${CMAKE_COMMAND}"
          "-DINPUT_FILE=$<TARGET_FILE:_wasp>"
          "-DRPATH=${WASP_COMPILER_LIB_DIR}"
          -P "${CMAKE_CURRENT_SOURCE_DIR}/remove_macos_rpath.cmake"
        COMMENT "Removing the compiler environment rpath from _wasp"
        VERBATIM
      )
    ENDIF()
  ELSEIF(UNIX)
    SET_PROPERTY(TARGET _wasp PROPERTY BUILD_RPATH "$ORIGIN/lib")
    SET_PROPERTY(TARGET _wasp PROPERTY INSTALL_RPATH "$ORIGIN/lib")
  ENDIF()

  # Preserve the historical target name for callers while making it incremental.
  ADD_CUSTOM_TARGET(
    wasppy ALL
    DEPENDS _wasp "${WASP_SWIG_PYTHON}"
  )

#Create setup.py using a version controlled skeleton with generator expressions
#and cmake variables
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/README.md
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PYPI.md ${CMAKE_CURRENT_BINARY_DIR}/README.md
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/PYPI.md)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.cfg
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg ${CMAKE_CURRENT_BINARY_DIR}/setup.cfg
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Database.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Database.py ${CMAKE_CURRENT_BINARY_DIR}/Database.py
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/Database.py)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sch2db.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sch2db.py ${CMAKE_CURRENT_BINARY_DIR}/sch2db.py
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/sch2db.py)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test/test_swig_bindings.py ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py
  MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/test/test_swig_bindings.py)


# file(READ) does not register its input as a CMake configuration dependency.
# Ensure edits to the setup template regenerate setup.py during a normal build.
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in"
)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in WASPPY_SETUP)
string(CONFIGURE "${WASPPY_SETUP}" WASPPY_SETUP_CONFIG)
if(CMAKE_CONFIGURATION_TYPES)
  file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup$<$<NOT:$<CONFIG:Release>>:$<CONFIG>>.py CONTENT "${WASPPY_SETUP_CONFIG}")
else()
  file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py CONTENT "${WASPPY_SETUP_CONFIG}")
endif()

set (wheel_file_deps 
      ${CMAKE_CURRENT_BINARY_DIR}/Database.py
      ${CMAKE_CURRENT_BINARY_DIR}/sch2db.py
      ${CMAKE_CURRENT_BINARY_DIR}/test/test_swig_bindings.py
      ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in
      ${CMAKE_CURRENT_BINARY_DIR}/README.md
      ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
      ${CMAKE_CURRENT_BINARY_DIR}/setup.cfg
    )
# Package the CMake-built extension.  Setuptools copies it into the wheel and
# does not compile or link C++ code.
SET(WASP_WHEEL_BUILD_ARGS --wheel --no-isolation)
IF(APPLE)
  LIST(FIND CMAKE_OSX_ARCHITECTURES "arm64" WASP_ARM64_INDEX)
  LIST(FIND CMAKE_OSX_ARCHITECTURES "x86_64" WASP_X86_64_INDEX)
  IF(WASP_ARM64_INDEX EQUAL -1 OR WASP_X86_64_INDEX EQUAL -1)
    MESSAGE(FATAL_ERROR
      "The macOS Python wheel requires both arm64 and x86_64 in "
      "CMAKE_OSX_ARCHITECTURES; found '${CMAKE_OSX_ARCHITECTURES}'")
  ENDIF()
  STRING(REPLACE "." "_" WASP_MACOS_WHEEL_VERSION
         "${CMAKE_OSX_DEPLOYMENT_TARGET}")
  LIST(APPEND WASP_WHEEL_BUILD_ARGS
    "-C--build-option=--plat-name=macosx_${WASP_MACOS_WHEEL_VERSION}_universal2"
  )
ENDIF()

SET(WASP_WHEEL_STAMP "${CMAKE_CURRENT_BINARY_DIR}/.wasp_wheel.stamp")
ADD_CUSTOM_COMMAND(
  OUTPUT "${WASP_WHEEL_STAMP}"
  COMMAND "${CMAKE_COMMAND}" -E rm -rf "${CMAKE_CURRENT_BINARY_DIR}/dist"
  COMMAND
    "${Python3_EXECUTABLE}" -m build ${WASP_WHEEL_BUILD_ARGS}
  COMMAND "${CMAKE_COMMAND}" -E touch "${WASP_WHEEL_STAMP}"
  DEPENDS
    _wasp
    ${wheel_file_deps}
    "${CMAKE_CURRENT_BINARY_DIR}/setup.py"
    "${WASP_SWIG_PYTHON}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Packaging the prebuilt WASP Python extension"
  VERBATIM
)
ADD_CUSTOM_TARGET(wasp_wheel ALL DEPENDS "${WASP_WHEEL_STAMP}")

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
