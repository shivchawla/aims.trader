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

INCLUDEPATH += ../Shared

SOURCES += framework.cpp \
    Business/databasesession.cpp \
    Data/strategyviewdata.cpp \
    Data/strategylinkedpositiondata.cpp \
    Data/strategydata.cpp \
    Data/strategycompositedata.cpp \
    Data/strategybuylistdata.cpp \
    Data/orderdata.cpp \
    Data/exchangedata.cpp \
    DataAccess/strategyviewdb.cpp \
    DataAccess/strategylinkedpositiondb.cpp \
    DataAccess/strategydb.cpp \
    DataAccess/strategycompositedb.cpp \
    DataAccess/strategybuylistdb.cpp \
    DataAccess/orderdb.cpp \
    DataAccess/exchangedb.cpp \
    Utils/dabbu.cpp \
    Data/strategybuylistviewdata.cpp \
    DataAccess/strategybuylistviewdb.cpp \
    ../Shared/Data/InstrumentData.cpp \
    ../Shared/Data/DailyHistoryBarData.cpp \
    ../Shared/Data/ConfigurationData.cpp \
    ../Shared/DataAccess/InstrumentDb.cpp \
    ../Shared/DataAccess/DbBase.cpp \
    ../Shared/DataAccess/DailyHistoryBarDb.cpp \
    ../Shared/DataAccess/ConfigurationDb.cpp \
    ../Shared/Utils/UtilBox.cpp \
    ../Shared/Utils/BootStrapper.cpp

HEADERS += framework.h\
        Framework_global.h \
    Business/databasesession.h \
    Data/strategyviewdata.h \
    Data/strategylinkedpositiondata.h \
    Data/strategydata.h \
    Data/strategycompositedata.h \
    Data/strategybuylistdata.h \
    Data/orderdata.h \
    Data/exchangedata.h \
    DataAccess/strategyviewdb.h \
    DataAccess/strategylinkedpositiondb.h \
    DataAccess/strategydb.h \
    DataAccess/strategycompositedb.h \
    DataAccess/strategybuylistdb.h \
    DataAccess/orderdb.h \
    DataAccess/exchangedb.h \
    Utils/dabbu.h \
    Data/strategybuylistviewdata.h \
    DataAccess/strategybuylistviewdb.h \
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
