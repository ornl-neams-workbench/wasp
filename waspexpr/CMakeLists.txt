INCLUDE(WaspFlexAndBison)
INCLUDE(WaspAddCompilerFlags)
TRIBITS_SUBPACKAGE(expr)

WASP_ADD_CXX_FLAGS(
  "-Wno-deprecated-register"
)

WASP_FLEX_AND_BISON(
  FLEX_FILE Expr.lex
  FLEX_OPTIONS -Ca -v
  SED_OPTIONS 's@\#include <FlexLexer.h>@\#include \"waspcore/FlexLexer.h\"@' ExprLexer.cpp
  BISON_FILE Expr.bison
  BISON_OPTIONS --report=state --report=lookahead
)

SET(SOURCE
ExprContext.cpp
ExprLexer.cpp
ExprParser.cpp
)

SET(HEADERS
ExprContext.h
ExprLexer.h
ExprParser.hpp
ExprInterpreter.h
ExprInterpreter.i.h
Expr.lex
Expr.bison
)

#
# Add library
TRIBITS_ADD_LIBRARY(waspexpr
  SOURCES ${SOURCE}
  NOINSTALLHEADERS ${HEADERS}
)

TRIBITS_ADD_EXECUTABLE(exprlist
  SOURCES ListExpr.cpp
  NOEXEPREFIX
  NOEXESUFFIX
  INSTALLABLE
)


# Expose directories for the python wrappers to use
target_include_directories( waspexpr
  PRIVATE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  )

#
# Add test directory
TRIBITS_ADD_TEST_DIRECTORIES(test)

INSTALL(FILES ${HEADERS} DESTINATION "include/waspexpr/")
TRIBITS_SUBPACKAGE_POSTPROCESS()
