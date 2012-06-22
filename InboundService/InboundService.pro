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


SOURCES += \
    API/Streamer.cpp \
    API/Session.cpp \
    API/Requestor.cpp \
    API/Helper.cpp \
    Data/strategyviewdata.cpp \
    Data/strategylinkedpositiondata.cpp \
    Data/strategydata.cpp \
    Data/strategycompositedata.cpp \
    Data/strategybuylistdata.cpp \
    Data/orderdata.cpp \
    Data/InstrumentData.cpp \
    Data/exchangedata.cpp \
    Data/DailyHistoryBarData.cpp \
    Data/ConfigurationData.cpp \
    DataAccess/strategyviewdb.cpp \
    DataAccess/strategylinkedpositiondb.cpp \
    DataAccess/strategydb.cpp \
    DataAccess/strategycompositedb.cpp \
    DataAccess/strategybuylistdb.cpp \
    DataAccess/orderdb.cpp \
    DataAccess/InstrumentDb.cpp \
    DataAccess/exchangedb.cpp \
    DataAccess/DbBase.cpp \
    DataAccess/DailyHistoryBarDb.cpp \
    DataAccess/ConfigurationDb.cpp \
    Utils/UtilBox.cpp \
    Utils/dabbu.cpp \
    Utils/BootStrapper.cpp \
    unitTests.cpp \
    stdafx.cpp \
    Program.cpp \
    Main.cpp \
    InboundService.cpp \
    InboundBase.cpp \
    HistoryBarInbound.cpp \
    Business/databasesession.cpp

HEADERS += ./API/Helper.h ./API/Requestor.h ./API/Session.h ./API/Streamer.h \
    Data/strategyviewdata.h \
    Data/strategylinkedpositiondata.h \
    Data/strategydata.h \
    Data/strategycompositedata.h \
    Data/strategybuylistdata.h \
    Data/orderdata.h \
    Data/InstrumentData.h \
    Data/exchangedata.h \
    Data/DailyHistoryBarData.h \
    Data/ConfigurationData.h \
    DataAccess/strategyviewdb.h \
    DataAccess/strategylinkedpositiondb.h \
    DataAccess/strategydb.h \
    DataAccess/strategycompositedb.h \
    DataAccess/strategybuylistdb.h \
    DataAccess/orderdb.h \
    DataAccess/InstrumentDb.h \
    DataAccess/exchangedb.h \
    DataAccess/DbBase.h \
    DataAccess/DailyHistoryBarDb.h \
    DataAccess/ConfigurationDb.h \
    Utils/UtilBox.h \
    Utils/dabbu.h \
    Utils/Constants.h \
    Utils/BootStrapper.h \
    unitTests.h \
    stdafx.h \
    InboundService.h \
    InboundBase.h \
    HistoryBarInbound.h \
    Business/databasesession.h

unix:!symbian: LIBS += -L$$PWD/../../../ActiveTickSDK/lib/ -lActiveTickFeedAPI

INCLUDEPATH += $$PWD/../../../ActiveTickSDK/include
DEPENDPATH += $$PWD/../../../ActiveTickSDK/include
