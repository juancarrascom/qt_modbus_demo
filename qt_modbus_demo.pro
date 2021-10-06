
QT = core serialport
CONFIG += console c++11

TARGET = qt_modbus_demo
    target.files =  qt_modbus_demo
    target.path = /

SOURCES += \
    modbusMaster.cpp \
    main.cpp

HEADERS += \
    modbusMaster.h

INSTALLS += target
QT += core serialbus serialport widgets
