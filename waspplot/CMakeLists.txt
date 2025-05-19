##---------------------------------------------------------------------------##
## Plot/CMakeLists.txt
##---------------------------------------------------------------------------##
## Copyright (C) 2025 Oak Ridge National Laboratory, UT-Battelle, LLC.
##---------------------------------------------------------------------------##
## CMAKE for Plot
##---------------------------------------------------------------------------##

INCLUDE(WaspAddCompilerFlags)

TRIBITS_SUBPACKAGE(plot)
SET_DEFAULT(DBC 0)
SET_DEFAULT(TIMING 0)
SET_DEFAULT(DEBUG_OUTPUT 0)
##---------------------------------------------------------------------------##
## INCLUDES

##---------------------------------------------------------------------------##
## HEADER AND SOURCE FILES
##---------------------------------------------------------------------------##


# C/C++ headers
FILE(GLOB HEADERS *.h *.hh *.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/*.h"
    )

# c++ files
SET(SOURCES
CustomPlotFile.cpp
CustomPlotSerialization.cpp
)

##---------------------------------------------------------------------------##
## PACKAGE TARGETS AND INSTALL
##---------------------------------------------------------------------------##

TRIBITS_ADD_LIBRARY(
  waspplot
  SOURCES ${SOURCES}
  NOINSTALLHEADERS ${HEADERS})


INSTALL(FILES
  ${HEADERS}
  DESTINATION
  include/waspplot)

#
# Add test directory for this package
#
TRIBITS_ADD_TEST_DIRECTORIES(test)

##---------------------------------------------------------------------------##
##                   end of Plot/CMakeLists.txt
##---------------------------------------------------------------------------##
TRIBITS_SUBPACKAGE_POSTPROCESS()
