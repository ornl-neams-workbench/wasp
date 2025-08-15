TRIBITS_SUBPACKAGE(core)

TRIBITS_CONFIGURE_FILE(version.h)

SET(SOURCE
Definition.cpp
Interpreter.cpp
utils.cpp
Object.cpp
)

SET(HEADERS
Definition.h
Format.h
FlexLexer.h
Interpreter.h
Interpreter.i.h
Iterator.h
location.hh
Object.h
StringPool.h
StringPool.i.h
TokenPool.h
TokenPool.i.h
TreeNodePool.h
TreeNodePool.i.h
utils.h
decl.h
wasp_bug.h
wasp_math.h
wasp_node.h
${CMAKE_CURRENT_BINARY_DIR}/version.h
)

#
# Add library
TRIBITS_ADD_LIBRARY(waspcore
  SOURCES ${SOURCE}
  NOINSTALLHEADERS ${HEADERS}
)

# Expose directories for the python wrappers to use
target_include_directories( waspcore
  PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  )
#set_property(TARGET waspcore PROPERTY PUBLIC_HEADER ${HEADERS})

#
# Add test directory
TRIBITS_ADD_TEST_DIRECTORIES(test)

INSTALL(FILES ${HEADERS} DESTINATION "include/waspcore/")
TRIBITS_SUBPACKAGE_POSTPROCESS()
