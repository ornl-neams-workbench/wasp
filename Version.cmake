##---------------------------------------------------------------------------##
## wasp/Version.cmake
##---------------------------------------------------------------------------##
# Single file that needs to be changed on a release branch
# or on the development branch in order to configure wasp
# for release mode and set the version.

SET(WASP_NAME "wasp")
SET(WASP_VERSION 5.0.0)
SET(WASP_FULL_VERSION "Workbench Analysis Sequence Processor ${WASP_VERSION}")
SET(WASP_VERSION_STRING "${WASP_VERSION}")
SET(WASP_MAJOR_VERSION 05)
SET(WASP_MAJOR_MINOR_VERSION 050000)
SET(WASP_ENABLE_DEVELOPMENT_MODE_DEFAULT ON) # Change to 'OFF' for a release (beta or final)

# TriBITS expects project-scoped version variables when WASP is built as a
# standalone project. Do not assign these aliases when WASP is a package in a
# larger project, where PROJECT_NAME belongs to the parent project.
IF(PROJECT_NAME STREQUAL "wasp")
  SET(${PROJECT_NAME}_NAME "${WASP_NAME}")
  SET(${PROJECT_NAME}_VERSION "${WASP_VERSION}")
  SET(${PROJECT_NAME}_FULL_VERSION "${WASP_FULL_VERSION}")
  SET(${PROJECT_NAME}_VERSION_STRING "${WASP_VERSION_STRING}")
  SET(${PROJECT_NAME}_MAJOR_VERSION "${WASP_MAJOR_VERSION}")
  SET(${PROJECT_NAME}_MAJOR_MINOR_VERSION "${WASP_MAJOR_MINOR_VERSION}")
  SET(${PROJECT_NAME}_ENABLE_DEVELOPMENT_MODE_DEFAULT
    ${WASP_ENABLE_DEVELOPMENT_MODE_DEFAULT})
ENDIF()

##---------------------------------------------------------------------------##
##                    end of wasp/Version.cmake
##---------------------------------------------------------------------------##
