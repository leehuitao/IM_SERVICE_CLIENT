INCLUDEPATH += $$PWD

QT += multimedia mqtt

HEADERS += \
    $$PWD/Encode/aac_encode.h \
    $$PWD/Encode/encode_basic.h \
    $$PWD/Encode/lht_ffmpeg_aac_encode.h \
    $$PWD/Encode/lht_opus_encode.h \
    $$PWD/Encode/opus_encode.h \
    $$PWD/audio_play.h \
    $$PWD/lht_callback.h \
    $$PWD/lht_define.h \
    $$PWD/udpClient/lht_udp_client.h

SOURCES += \
    $$PWD/Encode/aac_encode.cpp \
    $$PWD/Encode/encode_basic.cpp \
    $$PWD/Encode/lht_ffmpeg_aac_encode.cpp \
    $$PWD/Encode/lht_opus_encode.cpp \
    $$PWD/Encode/opus_encode.cpp \
    $$PWD/audio_play.cpp \
    $$PWD/udpClient/lht_udp_client.cpp

DEFINES += __STDC_CONSTANT_MACROS

INCLUDEPATH += $$PWD/include
LIBS += -L$$PWD/lib
LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswscale

win32: LIBS += -L$$PWD/lib/ -lopus

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/opus.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/libopus.a

