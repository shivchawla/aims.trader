#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T19:59:21
#
#-------------------------------------------------

QT       += core
QT       += sql

//QT       -= gui

TARGET = InboundService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../../ActiveTickSDK/include
INCLUDEPATH += $$PWD/../Shared

SOURCES += \
    API/Streamer.cpp \
    API/Session.cpp \
    API/Requestor.cpp \
    API/Helper.cpp \
    stdafx.cpp \
    Main.cpp \
    InboundService.cpp

HEADERS += ./API/Helper.h ./API/Requestor.h ./API/Session.h ./API/Streamer.h \
    stdafx.h \
    InboundService.h

unix:!symbian: LIBS += -L$$PWD/../../../ActiveTickSDK/lib/ -lActiveTickFeedAPI

INCLUDEPATH += $$PWD/../../../ActiveTickSDK/include
DEPENDPATH += $$PWD/../../../ActiveTickSDK/include

OTHER_FILES += \
    InboundService.ini
