##
##    Copyright: O-Marius Chincisan 2016
##
##    This program is distributed
##    WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##
##    This program, or portions of it cannot be used in commercial
##    products without the written consent of the author.
##
##

cmake_minimum_required (VERSION 2.6)
PROJECT(BUNGET)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions -Wall -fpermissive")
include_directories(libbunget/include)
add_executable (bunget main.cpp)

###
###  sudo apt-get install libcrypto++-dev
###  bunget   (read ge as in 'George')
###  libbunget stops the bluetooth service.
###
###
add_definitions( -DDEBUG )
link_directories(./lib/)
TARGET_LINK_LIBRARIES(bunget ${CMAKE_CURRENT_LIST_DIR}/lib/libbunget.a crypto++)
SET(EXECUTABLE_OUTPUT_PATH ./bin)
