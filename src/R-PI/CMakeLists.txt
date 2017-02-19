###  ARM CMAKEFILE. (RASPBERRY-PI, iMX, UDOO...)
###
###  R: sudo apt-get install libcrypto++-dev
###  R: stop and start bluetooth service on any mobile each time you make a change
#### bunget   (read ge as in 'George')
###  libbunget stops the bluetooth service.
###  Author: Marius-C.  

cmake_minimum_required (VERSION 2.6)
PROJECT(BUNGET)



set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

add_definitions(-DDEBUG)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -std=c++11 -fexceptions -Wall -fpermissive")
add_definitions(-DARM_CC)
add_executable (pibunget main.cpp)
include_directories(libbunget/include)

link_directories(./lib/)
TARGET_LINK_LIBRARIES(pibunget crypto++ ${CMAKE_CURRENT_LIST_DIR}/lib/libbunget.a)
SET(EXECUTABLE_OUTPUT_PATH ./bin)

