#-------------------------------------------------
#
# Project created by QtCreator 2012-06-21T23:54:03
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = Framework
TEMPLATE = lib

DEFINES += FRAMEWORK_LIBRARY

SOURCES += framework.cpp \
    Business/databasesession.cpp \
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
    Utils/BootStrapper.cpp

HEADERS += framework.h\
        Framework_global.h \
    Business/databasesession.h \
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
    Utils/BootStrapper.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE6383B09
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = Framework.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
