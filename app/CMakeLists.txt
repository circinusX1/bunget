##
##    Copyright: O-Marius Chincisan & Zirexix Inc 2016-2017
##
##    This program is distributed
##    WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##
cmake_minimum_required (VERSION 2.6)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

add_definitions(-DDEBUG)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions -Wall -fpermissive -ggdb")

add_executable (bunget mainn.cpp crypto.cpp)
target_link_libraries (bunget bungetLib crypto++)

include_directories(lib/include)


SET(EXECUTABLE_OUTPUT_PATH ./bin)

