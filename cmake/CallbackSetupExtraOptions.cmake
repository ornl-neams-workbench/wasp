##---------------------------------------------------------------------------##
## wasp/cmake/CallbackSetupExtraOptions.cmake
## Jordan Lefebvre, lefebvrejp@ornl.gov
##---------------------------------------------------------------------------##

include( "${wasp_SOURCE_DIR}/cmake/WaspDocGen.cmake" )

IF (wasp_SOURCE_DIR)
  # We need to inject the wasp/cmake directory to find several wasp-specific
  # macros
  SET(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${wasp_SOURCE_DIR}/cmake" "${wasp_SOURCE_DIR}/modules" )
ENDIF()

##---------------------------------------------------------------------------##

MACRO(TRIBITS_REPOSITORY_SETUP_EXTRA_OPTIONS)
  #
  # Include the testframework setup
  #
  ADD_SUBDIRECTORY(${wasp_SOURCE_DIR}/testframework/setup)

  # Set CXX11 to be enabled by default.
  SET(${PROJECT_NAME}_ENABLE_CXX11_DEFAULT TRUE)

  ########################################
  # STATIC/SHARED BUILD SETUP
  ########################################

  # Add install RPATH when building shared
  IF(BUILD_SHARED_LIBS AND CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    IF (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      SET(CMAKE_INSTALL_RPATH "@loader_path/../lib")
      SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
      SET(CMAKE_SKIP_BUILD_RPATH FALSE)
      SET(CMAKE_MACOSX_RPATH ON)
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
       # use, i.e. don't skip the full RPATH for the build tree
       SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

       # when building, don't use the install RPATH already
       # (but later on when installing)
       SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

       # the RPATH to be used when installing
       SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN/../lib")

       # don't add the automatically determined parts of the RPATH
       # which point to directories outside the build tree to the install RPATH
       SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    ENDIF()
  ENDIF()

  ########################################
  # wasp PACKAGE SETUP
  ########################################

  #
  # Default MPI to OFF
  #
  SET(TPL_ENABLE_MPI OFF CACHE BOOL "" FORCE)
  #
  # Check if the environment has changed whether we want fortran enabled
  #
  IF(NOT "$ENV{${PROJECT_NAME}_ENABLE_Fortran}" STREQUAL "" )
     SET(${PROJECT_NAME}_ENABLE_Fortran $ENV{${PROJECT_NAME}_ENABLE_Fortran})
  ELSE()
     # ensure fortran compiler is set on
     SET(${PROJECT_NAME}_ENABLE_Fortran OFF)
  ENDIF()
  # Set up wasp cmake directory, used by default option scripts
  SET(wasp_CMAKE_DIR "${wasp_SOURCE_DIR}/cmake" CACHE PATH "")
 
  list(APPEND WASP_README_MD "README.md")
  list(APPEND WASP_SUBPACKAGES "waspcore;waspdakota;waspddi;waspeddi;waspfiller;waspexpr;wasphalite;wasphit;wasphive;waaspjson;wasplsp;waspmcnpi;waspplot;wasppy;waspsiren;waspson;wasptoml;wasplutils")
  foreach(SUBPACKAGE ${WASP_SUBPACKAGES})
    if(EXISTS "${wasp_SOURCE_DIR}/${SUBPACKAGE}/README.md")
      list(APPEND WASP_README_MD "${SUBPACKAGE}/README.md")
    endif()
  endforeach()

  # Setup documentation generation, if pandoc is available
  WASP_DOC_GEN( 
    DOCX_FILE "${wasp_BINARY_DIR}/WorkbenchAnalysisSequenceProcessor.docx"
    PDF_FILE "${wasp_BINARY_DIR}/WorkbenchAnalysisSequenceProcessor.pdf"
    MD_FILES ${WASP_README_MD}
  )

  ########################################
  # INCLUDE DIRECTORY SETUP
  ########################################

  # Set up include paths for wasp packages
  MESSAGE(STATUS "Adding wasp directories for relative include paths.")
  INCLUDE_DIRECTORIES("${wasp_SOURCE_DIR}")
  INCLUDE_DIRECTORIES("${wasp_BINARY_DIR}")

  ########################################
  # PLATFORM SETUP
  ########################################


  ########################################
  # INSTALLATION SETUP
  ########################################
  install(FILES "${wasp_SOURCE_DIR}/LICENSE" DESTINATION .)

ENDMACRO()

##---------------------------------------------------------------------------##
##          end of wasp/cmake/CallbackSetupExtraOptions.cmake
##---------------------------------------------------------------------------##
