##---------------------------------------------------------------------------##
## Monte Carlo N-Particle Input Interpreter (MCNPI)/CMakeLists.txt
## Robert A. Lefebvre
## Wednesday June 15 09:21 2022
##---------------------------------------------------------------------------##
## Copyright (C) 2017 Oak Ridge National Laboratory, UT-Battelle, LLC.
##---------------------------------------------------------------------------##
## CMAKE for MCNPI
##---------------------------------------------------------------------------##

INCLUDE(WaspAddCompilerFlags)
INCLUDE(WaspFlexAndBison)


TRIBITS_SUBPACKAGE(mcnpi)
SET_DEFAULT(DBC 0)
SET_DEFAULT(TIMING 0)
SET_DEFAULT(DEBUG_OUTPUT 0)
##---------------------------------------------------------------------------##
## INCLUDES

##---------------------------------------------------------------------------##
## HEADER AND SOURCE FILES
##---------------------------------------------------------------------------##

WASP_ADD_CXX_FLAGS(
  "-Wno-deprecated-register"
)

WASP_FLEX_AND_BISON(
  FLEX_FILE MCNPLexer.lex
  FLEX_OPTIONS -Ca -v
  SED_OPTIONS 's@\#include <FlexLexer.h>@\#include \"waspcore/FlexLexer.h\"@' MCNPParser.cpp
  BISON_FILE MCNPParser.bison
  BISON_OPTIONS --report=state --report=lookahead --report=solved -g
)

# C/C++ headers
FILE(GLOB HEADERS *.h *.hh *.hpp *.lex *.bison
    "${CMAKE_CURRENT_BINARY_DIR}/*.h"
    )

# c++ files
SET(SOURCES
MCNPLexer.cpp
MCNPParser.cpp
Model.cpp
# MCNPNodeView.cpp
)

##---------------------------------------------------------------------------##
## PACKAGE TARGETS AND INSTALL
##---------------------------------------------------------------------------##

TRIBITS_ADD_LIBRARY(
  waspmcnpi
  SOURCES ${SOURCES}
  NOINSTALLHEADERS ${HEADERS})

INSTALL(FILES
  ${HEADERS}
  DESTINATION
  include/waspmcnpi)

#
# Add test directory for this package
#
TRIBITS_ADD_TEST_DIRECTORIES(test)

##---------------------------------------------------------------------------##
##                   end of MCNPI/CMakeLists.txt
##---------------------------------------------------------------------------##
TRIBITS_SUBPACKAGE_POSTPROCESS()
