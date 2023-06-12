QT += core serialbus
CONFIG += c++11

TARGET = qtcandlelightcanbus
TEMPLATE = lib
CONFIG += plugin

SOURCES += plugin.cpp\
    device.cpp\
    devices.cpp\
    interface.cpp \
    listener.cpp\
    timing.cpp\

HEADERS += \
    device.hpp \
    devices.hpp\
    interface.hpp \
    listener.hpp \
    plugin.hpp \
    timing.hpp\

#CandleLight API
SOURCES += \
    api/candle.c \
    api/candle_ctrl_req.c

HEADERS += \
    api/candle.h \
    api/candle_defs.h \
    api/ch_9.h

win32: LIBS += -lSetupApi
win32: LIBS += -lOle32
win32: LIBS += -lwinusb
