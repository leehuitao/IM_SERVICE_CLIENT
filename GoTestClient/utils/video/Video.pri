INCLUDEPATH += $$PWD

QT += multimedia mqtt

DEFINES += __STDC_CONSTANT_MACROS

win32: LIBS += -L$$PWD/lib/ -lavcodec
win32: LIBS += -L$$PWD/lib/ -lavdevice
win32: LIBS += -L$$PWD/lib/ -lavfilter
win32: LIBS += -L$$PWD/lib/ -lavformat
win32: LIBS += -L$$PWD/lib/ -lavutil
win32: LIBS += -L$$PWD/lib/ -lswscale

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

HEADERS += \
    $$PWD/Encode/video_decode.h \
    $$PWD/Encode/x264_decode.h \
    $$PWD/Encode/x264_encode.h \
    $$PWD/vedio_play.h

SOURCES += \
    $$PWD/Encode/video_decode.cpp \
    $$PWD/Encode/x264_decode.cpp \
    $$PWD/Encode/x264_encode.cpp \
    $$PWD/vedio_play.cpp
