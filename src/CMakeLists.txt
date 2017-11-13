##
##    Copyright: O-Marius Chincisan & Zirexix Inc 2016-2017
##
##    This program is distributed
##    WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
##

cmake_minimum_required (VERSION 2.6)
PROJECT(BUNGET)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

add_definitions(-DDEBUG)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions -Wall -fpermissive")
include_directories(libbunget/include)
include_directories(libbunget)
add_executable (bunget mainn.cpp crypto.cpp)

###
###  sudo apt-get install libcrypto++-dev
###  bunget   (read ge as in 'George')
###  libbunget stops the bluetooth service.
###  All other BT related services as bluema blue bt should be stopped
###

link_directories(./lib/)
TARGET_LINK_LIBRARIES(bunget ${CMAKE_CURRENT_LIST_DIR}/lib/libbunget.a crypto++)
SET(EXECUTABLE_OUTPUT_PATH ./bin)

