MACRO(TRIBITS_PROJECT_DEFINE_PACKAGING)
  SET(CPACK_PACKAGE_NAME "WASP")
  SET(CPACK_PACKAGE_VENDOR "Oak Ridge National Laboratory")
  SET(
    CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Workbench Analysis Sequence Processor command-line utilities"
  )
  SET(CPACK_PACKAGE_VERSION "${wasp_VERSION}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "WASP")
  # The utilities are statically linked for these distribution packages, so
  # library, header, documentation, and unrelated support-script components
  # are not needed at runtime.
  SET(CPACK_COMPONENTS_ALL wasputils)
  CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/LICENSE"
    "${PROJECT_BINARY_DIR}/LICENSE.txt"
    COPYONLY
  )
  SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_BINARY_DIR}/LICENSE.txt")
  IF(WASP_CPACK_PLATFORM)
    SET(
      CPACK_PACKAGE_FILE_NAME
      "WASP-${CPACK_PACKAGE_VERSION}-${WASP_CPACK_PLATFORM}"
    )
  ENDIF()
  SET(
    CPACK_PROJECT_CONFIG_FILE
    "${PROJECT_SOURCE_DIR}/CMakeCPackOptions.cmake"
  )
ENDMACRO()
