INCLUDE(WaspAddCompilerFlags)
INCLUDE(WaspFlexAndBison)

TRIBITS_SUBPACKAGE(toml)

WASP_ADD_CXX_FLAGS(
  "-Wno-deprecated-register"
  "-wd4018"
)

WASP_FLEX_AND_BISON(
  FLEX_FILE TOML.lex
  FLEX_OPTIONS -Ca -v
  SED_OPTIONS 's@\#include <FlexLexer.h>@\#include \"waspcore/FlexLexer.h\"@' TOMLLexer.cpp
  BISON_FILE TOML.bison
  BISON_OPTIONS --report=state --report=lookahead
)

SET(SOURCE
TOMLLexer.cpp
TOMLParser.cpp
TOMLNodeView.cpp
)

SET(HEADERS
TOMLLexer.h
TOMLParser.hpp
TOMLInterpreter.h
TOMLInterpreter.i.h
TOMLNodeView.h
TOMLNodeView.i.h
TOML.lex
TOML.bison
)

#
# Add library
TRIBITS_ADD_LIBRARY(wasptoml
  SOURCES ${SOURCE}
  NOINSTALLHEADERS ${HEADERS}
)

#
# Add binary directory includes to wasptoml target
#
target_include_directories(wasptoml PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/wasptoml>
)

#
# Add test directory
TRIBITS_ADD_TEST_DIRECTORIES(test)

INSTALL(FILES ${HEADERS} DESTINATION "include/wasptoml/")
TRIBITS_SUBPACKAGE_POSTPROCESS()
