# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MDMTrace Fortran CXX)

# C++ SETUP
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# FORTRAN SETUP
set (CMAKE_Fortran_COMPILER "gfortran")
set (CMAKE_Fortran_FLAGS "-finit-local-zero -falign-commons -fno-automatic")

#————————————————————————————————————————————————————————————————————————————

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

## Setup MDMTrace
include_directories("/hdfs/software/MDMTrace/include")
link_directories("/hdfs/software/MDMTrace/lib")

## Find ROOT
EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
EXECUTE_PROCESS(COMMAND root-config --libs --glibs OUTPUT_VARIABLE ROOT_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

# ## Set up nptool
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I$ENV{NPTOOL}/NPLib/include")
# set (NPTOOL_LIBS "-L$ENV{NPTOOL}/NPLib/lib -lNPInitialConditions -lNPTiara")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/RAYTKIN1.F)
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmTraceApp.cpp")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmTraceTiara.cpp")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmExample.cpp")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmExample1.cpp")
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmNptool.cpp")
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/analysis/include/json/*.h)


#----------------------------------------------------------------------------
# Add the shared library & executables, and link it to the necessary libraries
#
add_library(MDMTrace SHARED ${sources} ${headers})
add_executable(mdmExample ${PROJECT_SOURCE_DIR}/src/mdmExample.cpp)
add_executable(mdmTraceTiara ${PROJECT_SOURCE_DIR}/src/mdmTraceTiara.cpp)
add_executable(mdmExample1 ${PROJECT_SOURCE_DIR}/src/mdmExample1.cpp)
add_executable(mdmTraceApp ${PROJECT_SOURCE_DIR}/src/mdmTraceApp.cpp)
# add_executable(mdmNptool ${PROJECT_SOURCE_DIR}/src/mdmNptool.cpp)
target_link_libraries(mdmExample MDMTrace)
target_link_libraries(mdmTraceTiara MDMTrace)
target_link_libraries(mdmTraceApp MDMTrace)
target_link_libraries(mdmExample1 MDMTrace  ${ROOT_LIBRARIES})
# target_link_libraries(mdmNptool MDMTrace ${ROOT_LIBRARIES} ${NPTOOL_LIBS})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mdmTraceApp DESTINATION ${CMAKE_INSTALL_PREFIX}/install/bin)
install(TARGETS MDMTrace DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


