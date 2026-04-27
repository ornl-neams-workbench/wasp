INCLUDE(WaspFlexAndBison)
INCLUDE(WaspAddCompilerFlags)
TRIBITS_SUBPACKAGE(filler)

WASP_ADD_CXX_FLAGS(
  "-Wno-deprecated-register"
)

WASP_FLEX_AND_BISON(
  FLEX_FILE Filler.lex
  FLEX_OPTIONS -Ca -v
  SED_OPTIONS 's@\#include <FlexLexer.h>@\#include \"waspcore/FlexLexer.h\"@' FillerLexer.cpp
  BISON_FILE Filler.bison
  BISON_OPTIONS --report=state --report=lookahead
)

SET(SOURCE
FillerLexer.cpp
FillerParser.cpp
)

SET(HEADERS
FillerLexer.h
FillerParser.hpp
FillerInterpreter.h
FillerInterpreter.i.h
Filler.lex
Filler.bison
)

#
# Add library
TRIBITS_ADD_LIBRARY(waspfiller
  SOURCES ${SOURCE}
  NOINSTALLHEADERS ${HEADERS}
)

TRIBITS_ADD_EXECUTABLE(fillerlist
  SOURCES ListFiller.cpp
  NOEXEPREFIX
  NOEXESUFFIX
  INSTALLABLE
)

#
# Add test directory
TRIBITS_ADD_TEST_DIRECTORIES(test)

INSTALL(FILES ${HEADERS} DESTINATION "include/waspfiller/")
TRIBITS_SUBPACKAGE_POSTPROCESS()
