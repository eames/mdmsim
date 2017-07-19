# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MDMTrace CXX Fortran)

ADD_DEFINITIONS(-std=c++11)

#————————————————————————————————————————————————————————————————————————————

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#Setup MDMTrace
include_directories("/hdfs/software/MDMTrace/include")
link_directories("/hdfs/software/MDMTrace/lib")

# #Find ROOT
# find_package(ROOT REQUIRED)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
# include_directories(${ROOT_INCLUDE_DIR})
# link_directories(${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    # ${PROJECT_SOURCE_DIR}/analysis/include
                    # ${Geant4_INCLUDE_DIR}
		   )
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.F)
list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/src/mdmTraceApp.cpp")
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/analysis/include/json/*.h)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_library(MDMTrace SHARED ${sources} ${headers})
add_executable(mdmTraceApp ${PROJECT_SOURCE_DIR}/src/mdmTraceApp.cpp)
target_link_libraries(mdmTraceApp MDMTrace) # ${ROOT_LIBRARIES}

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mdmTraceApp DESTINATION $ENV{HOME}/install/bin)
install(TARGETS MDMTrace DESTINATION $ENV{HOME}/install/lib)


