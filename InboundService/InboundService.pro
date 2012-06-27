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

INCLUDEPATH += $$PWD/../API/ActiveTickFeed/include
INCLUDEPATH += $$PWD/../API/ActiveTickFeed/lib/
DEPENDPATH += $$PWD/../API/ActiveTickFeed/lib/

macx:LIBS += -L$$PWD/../API/ActiveTickFeed/lib/ -lActiveTickFeedAPI

SOURCES += \
    API/Streamer.cpp \
    API/Session.cpp \
    API/Requestor.cpp \
    API/Helper.cpp \
    stdafx.cpp \
    Main.cpp \
    InboundService.cpp \
    ../Shared/Data/InstrumentData.cpp \
    ../Shared/Data/DailyHistoryBarData.cpp \
    ../Shared/Data/ConfigurationData.cpp \
    ../Shared/Utils/UtilBox.cpp \
    ../Shared/Utils/BootStrapper.cpp \
    ../Shared/DataAccess/InstrumentDb.cpp \
    ../Shared/DataAccess/DbBase.cpp \
    ../Shared/DataAccess/DailyHistoryBarDb.cpp \
    ../Shared/DataAccess/ConfigurationDb.cpp

HEADERS += ./API/Helper.h ./API/Requestor.h ./API/Session.h ./API/Streamer.h \
    stdafx.h \
    InboundService.h \
    ../Shared/Data/InstrumentData.h \
    ../Shared/Data/DailyHistoryBarData.h \
    ../Shared/Data/ConfigurationData.h \
    ../Shared/DataAccess/InstrumentDb.h \
    ../Shared/DataAccess/DbBase.h \
    ../Shared/DataAccess/DailyHistoryBarDb.h \
    ../Shared/DataAccess/ConfigurationDb.h \
    ../Shared/Utils/UtilBox.h \
    ../Shared/Utils/Constants.h \
    ../Shared/Utils/BootStrapper.h

unix:!symbian: LIBS += -L$$PWD/../../../ActiveTickSDK/lib/ -lActiveTickFeedAPI

INCLUDEPATH += $$PWD/../../../ActiveTickSDK/include
DEPENDPATH += $$PWD/../../../ActiveTickSDK/include

OTHER_FILES += \
    InboundService.ini
