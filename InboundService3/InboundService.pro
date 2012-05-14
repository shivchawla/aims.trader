#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T19:59:21
#
#-------------------------------------------------

CONFIG += console
CONFIG += qt
CONFIG += x86_64
QT += core gui
QT += sql

SOURCES += UtilBox.cpp \
    stdafx.cpp \
    Session.cpp \
    Requestor.cpp \
    Program.cpp \
    Main.cpp \
    InstrumentDb.cpp \
    InstrumentData.cpp \
    InboundService.cpp \
    InboundBase.cpp \
    HistoryBarInbound.cpp \
    HistoryBarDb.cpp \
    HistoryBarData.cpp \
    Helper.cpp \
    DbBase.cpp \
    ConfigurationDb.cpp \
    ConfigurationData.cpp \
    BootStrapper.cpp \
    dabbu.cpp \
    Streamer.cpp

HEADERS += \
    UtilBox.h \
    stdafx.h \
    Session.h \
    Requestor.h \
    InstrumentDb.h \
    InstrumentData.h \
    InboundService.h \
    InboundBase.h \
    HistoryBarInbound.h \
    HistoryBarDb.h \
    HistoryBarData.h \
    Helper.h \
    DbBase.h \
    ConfigurationDb.h \
    ConfigurationData.h \
    BootStrapper.h \
    dabbu.h \
    Streamer.h

macx: LIBS += -L$$PWD/../ActiveTick/bin/ -lActiveTickFeedAPI
macx: LIBS += -L$$PWD/../ActiveTick/bin/ -lboost_system
macx: LIBS += -L$$PWD/../ActiveTick/bin/ -lboost_thread

INCLUDEPATH += $$PWD/../ActiveTick/bin
DEPENDPATH += $$PWD/../ActiveTick/bin
LIBS += -L$$PWD/../ActiveTick/bin/

INCLUDEPATH += $$PWD/../ActiveTick/include

INCLUDEPATH += /usr/local/mysql-5.5.8-osx10.6-x86_64/include
LIBS += -L/usr/local/mysql-5.5.8-osx10.6-x86_64/lib --lmysqlclient.16


