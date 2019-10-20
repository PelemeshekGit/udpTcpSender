QT += core network gui widgets

DEFINES -= QT_NO_DEBUG_OUTPUT
DEFINES += MAKE_LOG

TEMPLATE = app
TARGET = UdpTcpSender


include( Tcp/Tcp.pri )
include( Udp/Udp.pri )
include( Widget/Widget.pri )
include( Ethernet/Ethernet.pri )
include( TopManager/TopManager.pri )
include( Version/Version.pri )

INCLUDEPATH +=  ./Widget \
                ./Ethernet \
                ./Tcp \
                ./Udp \
                ./TopManager \
                ./Version

SOURCES += \
    main.cpp

RESOURCES += \
    res.qrc

CONFIG *= warn_on
QMAKE_CXXFLAGS *= -std=c++11

UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

