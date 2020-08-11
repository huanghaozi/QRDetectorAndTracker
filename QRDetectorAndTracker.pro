TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        QRDetectorAndTracker.cpp \
        main.cpp

INCLUDEPATH += opencv/include \
               zbar/include
LIBS += $$PWD/opencv/bin/libopencv_*.dll \
        $$PWD/zbar/bin/libzbar-0.dll

HEADERS += \
    QRDetectorAndTracker.h
