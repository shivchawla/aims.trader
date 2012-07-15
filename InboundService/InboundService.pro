#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T19:59:21
#
#-------------------------------------------------

QT       += core
QT       += sql

TARGET = InboundService
CONFIG -=app_bundle

CONFIG(release, debug|release){
DEFINES += QT_NO_DEBUG_STREAM QT_NO_WARNING_OUTPUT
}

CONFIG(debug, debug|release){
CONFIG += console
}

INCLUDEPATH += $$PWD/../Shared/
INCLUDEPATH += $$PWD/../API/ActiveTickFeed/include
INCLUDEPATH += $$PWD/../API/ActiveTickFeed/lib
#DEPENDPATH += $$PWD/../API/ActiveTickFeed/lib/
#DEPENDPATH += $$PWD/../API/ActiveTickFeed/include

macx:LIBS += -L$$PWD/../API/ActiveTickFeed/lib -lActiveTickFeedAPI

unix:!symbian:!macx:LIBS += -L$$PWD/../API/ActiveTickFeed/lib/ -lActiveTickFeedAPI

#macx:LIBS += -L/Users/shivkumarchawla/QtSDK/Desktop/Qt/4.8.1/gcc/plugins/sqldrivers -lqsqlmysql_debug

INCLUDEPATH += /usr/local/mysql/lib
INCLUDEPATH += /usr/local/mysql/include
#macx:LIBS+=-L/usr/local/mysql-5.5.8-osx10.6-x86_64/lib -lmysqlclient_r
#macx:LIBS+=-L/usr/local/mysql-5.5.8-osx10.6-x86_64/lib -lmysqlclient.16


SOURCES += \
    API/Streamer.cpp \
    API/Session.cpp \
    API/Requestor.cpp \
    API/Helper.cpp \
    stdafx.cpp \
    Main.cpp \
    InboundService.cpp \
    ../Shared/Data/InstrumentData.cpp \
    ../Shared/Utils/UtilBox.cpp \
    ../Shared/Utils/BootStrapper.cpp \
    ../Shared/DataAccess/InstrumentDb.cpp \
    ../Shared/DataAccess/DbBase.cpp \
    ../Shared/DataAccess/DailyHistoryBarDb.cpp \
    DataManager.cpp \
    ../Shared/Data/GeneralConfigurationData.cpp \
    ../Shared/DataAccess/GeneralConfigurationDb.cpp \
    ../Shared/DataAccess/MinuteHistoryBarDb.cpp

HEADERS += ./API/Helper.h ./API/Requestor.h ./API/Session.h ./API/Streamer.h \
    stdafx.h \
    InboundService.h \
    ../Shared/Data/InstrumentData.h \
    ../Shared/DataAccess/InstrumentDb.h \
    ../Shared/DataAccess/DbBase.h \
    ../Shared/DataAccess/DailyHistoryBarDb.h \
    ../Shared/Utils/UtilBox.h \
    ../Shared/Utils/Constants.h \
    ../Shared/Utils/BootStrapper.h \
    DataManager.h \
    ../Shared/Data/GeneralConfigurationData.h \
    ../Shared/DataAccess/GeneralConfigurationDb.h \
    ../Shared/DataAccess/MinuteHistoryBarDb.h \
    ../Shared/Data/HistoryBarData.h

#unix:!symbian: LIBS += -L$$PWD/../../../ActiveTickSDK/lib/ -lActiveTickFeedAPI

#INCLUDEPATH += $$PWD/../../../ActiveTickSDK/include
#DEPENDPATH += $$PWD/../../../ActiveTickSDK/include

OTHER_FILES += \
    Config.ini


#NOTES
#Th sqldrivers folder has been added to the app folder.
#Added DYLD_LIBRARY_PATH. Add /usr/local/sql/lib
