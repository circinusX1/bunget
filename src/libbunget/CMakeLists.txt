cmake_minimum_required (VERSION 2.6)
PROJECT(bunget)

include_directories ("./include")

add_definitions(-DDEBUG)
add_definitions(-DONGOING)
#add_definitions(-DNO_USE_UVLIB )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(LIBRARY_OUTPUT_PATH ../lib)

add_library(bunget STATIC ascon.cpp
                             bt_incinpl.cpp
                             bt_socket.cpp
                             bu_gap.cpp
                             bu_gatt.cpp
                             bu_hci.cpp
                             bybuff.cpp
                             cryptos.cpp
                             gattdefs.cpp
                             hci_socket.cpp
                             l2cap_socket.cpp
                             libbungetpriv.cpp
                             rfcomm_socket.cpp
                             sco_socket.cpp
                             secmanp.cpp
                             uguid.cpp)


SET(LIBRARY_OUTPUT_PATH ../lib)
#add_definitions( -DNO_USE_UVLIB )
add_definitions( -DDEBUG )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

