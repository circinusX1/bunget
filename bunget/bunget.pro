#
# sudo setcap cap_net_raw+eip ./build-bunget-Desktop-Debug/
#
QT += core
QT -= gui

DEFINES += DEBUG
DEFINES += _DEBUG

TARGET = bunget
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += ../src/libbunget/include
INCLUDEPATH += ../src/libbunget

QMAKE_CXXFLAGS += -Wno-missing-braces -Wno-unused-result
QMAKE_CXXFLAGS += -std=c++11 -fgnu-tm
QMAKE_CXXFLAGS += -Wno-missing-braces -Wno-unused-result -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-missing-braces -Wno-unused-result -Wno-unused-parameter
LIBS         += -lpthread -lcrypto++


TEMPLATE = app

SOURCES += \
    ../src/libbunget/ascon.cpp \
    ../src/libbunget/bt_incinpl.cpp \
    ../src/libbunget/bt_socket.cpp \
    ../src/libbunget/bu_gap.cpp \
    ../src/libbunget/bu_gatt.cpp \
    ../src/libbunget/bu_hci.cpp \
    ../src/libbunget/bybuff.cpp \
    ../src/libbunget/gattdefs.cpp \
    ../src/libbunget/hci_config.cpp \
    ../src/libbunget/hci_socket.cpp \
    ../src/libbunget/l2cap_socket.cpp \
    ../src/libbunget/libbungetpriv.cpp \
    ../src/libbunget/rfcomm_socket.cpp \
    ../src/libbunget/sco_socket.cpp \
    ../src/libbunget/secmanp.cpp \
    ../src/libbunget/uguid.cpp \
    ../src/crypto.cpp \
    ../src/mainn.cpp

HEADERS += \
    ../src/libbunget/include/bluetooth.h \
    ../src/libbunget/include/hci_lib.h \
    ../src/libbunget/include/hci.h \
    ../src/libbunget/include/icryptos.h \
    ../src/libbunget/include/l2cap.h \
    ../src/libbunget/include/libbunget.h \
    ../src/libbunget/include/rfcomm.h \
    ../src/libbunget/include/sco.h \
    ../src/libbunget/include/uuid.h \
    ../src/libbunget/ascon.h \
    ../src/libbunget/bt_socket.h \
    ../src/libbunget/bu_gap.h \
    ../src/libbunget/bu_gatt.h \
    ../src/libbunget/bu_hci.h \
    ../src/libbunget/bungetconf.h \
    ../src/libbunget/bybuff.h \
    ../src/libbunget/gattdefs.h \
    ../src/libbunget/hci_config.h \
    ../src/libbunget/hci_socket.h \
    ../src/libbunget/l2cap_socket.h \
    ../src/libbunget/lebiendians.h \
    ../src/libbunget/libbungetpriv.h \
    ../src/libbunget/rfcomm_socket.h \
    ../src/libbunget/sco_socket.h \
    ../src/libbunget/secmanp.h \
    ../src/libbunget/uguid.h \
    ../src/crypto.h

DISTFILES += \
    ../src/libbunget/CMakeLists.txt



QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -O0

QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O3
QMAKE_CXXFLAGS_DEBUG += -O0

QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -O0

QMAKE_CFLAGS_DEBUG -= -O1
QMAKE_CFLAGS_DEBUG -= -O2
QMAKE_CFLAGS_DEBUG -= -O3
QMAKE_CFLAGS_DEBUG += -O0

QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -O0
