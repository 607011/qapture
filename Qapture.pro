# $Id: Qapture.pro 9e28cb30f3ba 2012/04/27 14:19:35 Oliver Lau <oliver@von-und-fuer-lau.de> $

QT += core gui

DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += C:/OpenCV-VS2010/build/include

LIBS += -LC:/OpenCV-VS2010/lib/Debug -lopencv_core231d -lopencv_highgui231d -lopencv_imgproc231d

TARGET = Qapture
TEMPLATE = app

SOURCES += main.cpp \
    OpenCV.cpp \
    VideoWidget.cpp \
    mainwindow.cpp

HEADERS += \
    OpenCV.h \
    VideoWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui


