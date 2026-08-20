if(NOT DEFINED INPUT_FILE OR NOT DEFINED RPATH)
  message(FATAL_ERROR "INPUT_FILE and RPATH are required")
endif()

find_program(OTOOL otool REQUIRED)
find_program(INSTALL_NAME_TOOL install_name_tool REQUIRED)

execute_process(
  COMMAND "${OTOOL}" -l "${INPUT_FILE}"
  RESULT_VARIABLE OTOOL_RESULT
  OUTPUT_VARIABLE LOAD_COMMANDS
  ERROR_VARIABLE OTOOL_ERROR
)
if(NOT OTOOL_RESULT EQUAL 0)
  message(FATAL_ERROR "Unable to inspect ${INPUT_FILE}: ${OTOOL_ERROR}")
endif()

string(FIND "${LOAD_COMMANDS}" "path ${RPATH} (" RPATH_INDEX)
if(NOT RPATH_INDEX EQUAL -1)
  execute_process(
    COMMAND "${INSTALL_NAME_TOOL}" -delete_rpath "${RPATH}" "${INPUT_FILE}"
    RESULT_VARIABLE REMOVE_RESULT
    ERROR_VARIABLE REMOVE_ERROR
  )
  if(NOT REMOVE_RESULT EQUAL 0)
    message(FATAL_ERROR "Unable to remove ${RPATH} from ${INPUT_FILE}: ${REMOVE_ERROR}")
  endif()
endif()
