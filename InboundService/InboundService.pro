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
    Helper.cpp \
    DbBase.cpp \
    ConfigurationDb.cpp \
    ConfigurationData.cpp \
    BootStrapper.cpp \
    dabbu.cpp \
    Streamer.cpp \
    DailyHistoryBarData.cpp \
    DailyHistoryBarDb.cpp \
    strategyviewdata.cpp \
    strategylinkedpositiondb.cpp \
    strategylinkedpositiondata.cpp \
    strategydb.cpp \
    strategydata.cpp \
    strategybuylistdb.cpp \
    strategybuylistdata.cpp \
    orderdb.cpp \
    orderdata.cpp \
    strategyviewdb.cpp \
    unitTests.cpp \
    exchangedb.cpp \
    exchangedata.cpp \
    strategybusiness.cpp \
    strategycompositedata.cpp \
    strategycompositedb.cpp

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
    Helper.h \
    DbBase.h \
    ConfigurationDb.h \
    ConfigurationData.h \
    BootStrapper.h \
    dabbu.h \
    Streamer.h \
    Constants.h \
    DailyHistoryBarData.h \
    DailyHistoryBarDb.h \
    strategyviewdb.h \
    strategyviewdata.h \
    strategylinkedpositiondb.h \
    strategylinkedpositiondata.h \
    strategydb.h \
    strategydata.h \
    strategybuylistdb.h \
    strategybuylistdata.h \
    orderdb.h \
    orderdata.h \
    unitTests.h \
    exchangedb.h \
    exchangedata.h \
    strategybusiness.h \
    strategycompositedata.h \
    strategycompositedb.h

unix:!symbian: LIBS += -L$$PWD/../../../ActiveTickSDK/lib/ -lActiveTickFeedAPI

INCLUDEPATH += $$PWD/../../../ActiveTickSDK/include
DEPENDPATH += $$PWD/../../../ActiveTickSDK/include
