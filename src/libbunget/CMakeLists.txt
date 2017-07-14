
##
##    Copyright: O-Marius Chincisan 2016-2017 Zirexix Inc
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
PROJECT(bunget)

include_directories ("./include")

#add_definitions(-DDEBUG)
add_definitions(-DACL_MTU_FRAG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(LIBRARY_OUTPUT_PATH ../lib)

add_library(bunget STATIC ascon.cpp
                             bt_incinpl.cpp
                             bt_socket.cpp
                             bu_gap.cpp
                             bu_gatt.cpp
                             bu_hci.cpp
                             bybuff.cpp
                             gattdefs.cpp
                             hci_socket.cpp
                             l2cap_socket.cpp
                             libbungetpriv.cpp
                             rfcomm_socket.cpp
                             sco_socket.cpp
                             secmanp.cpp
                             uguid.cpp)
SET(LIBRARY_OUTPUT_PATH ../lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

