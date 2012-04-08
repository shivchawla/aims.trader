#-------------------------------------------------
#
# Project created by QtCreator 2012-03-10T11:26:00
#
#-------------------------------------------------

TEMPLATE += app
CONFIG += console
CONFIG += qt
CONFIG += x86_64
QT += core gui


SOURCES += main.cpp\
    Platform/Trader/TraderAssistant.cpp \
    Platform/Trader/Trader.cpp \
    Platform/Trader/AdapterWrapper.cpp \
    Platform/Strategy/Strategy.cpp \
    Platform/PosixSocketClient/EPosixClientSocket.cpp \
    Platform/PosixSocketClient/EClientSocketBase.cpp \
    Platform/Commission/CommissionFactory.cpp \
    Platform/Commission/Commission.cpp \
    Platform/Indicator/IndicatorManager.cpp \
    Platform/Indicator/Indicator.cpp \
    Platform/Performance/PerformanceManager.cpp \
    Platform/Position/PositionManager.cpp \
    Platform/Position/Position.cpp \
    Platform/Position/OpenOrder.cpp \
    Platform/Position/Instrument.cpp \
    Platform/Reports/StrategyReportManager.cpp \
    Platform/Reports/StrategyReport.cpp \
    Platform/Reports/Report.cpp \
    Platform/Reports/EventReport.cpp \
    Platform/Startup/Service.cpp \
    Platform/Utils/TradingSchedule.cpp \
    Platform/Utils/SingleApplication.cpp \
    Platform/Utils/Date.cpp \
    Platform/Utils/ConfigurationManager.cpp \
    Platform/Trader/OrderManager.cpp \
    Platform/Trader/InstrumentManager.cpp \
    Platform/Utils/ThreadManager.cpp \
    Platform/Strategy/StrategyManager.cpp \
    Strategy/TestStrategy.cpp \
    ActiveTickFeed/Utils/Streamer.cpp \
    ActiveTickFeed/Utils/stdafx.cpp \
    ActiveTickFeed/Utils/Session.cpp \
    ActiveTickFeed/Utils/Requestor.cpp \
    ActiveTickFeed/Utils/Helper.cpp \
    ActiveTickFeed/Utils/ActiveTickApi.cpp \
    Platform/View/StrategyView.cpp \
    Platform/View/InstrumentView.cpp \
    Platform/View/InstrumentViewItem.cpp \
    Platform/View/TableItem.cpp \
    Platform/View/StrategyViewItem.cpp \
    Platform/Utils/redirect.cpp \
    Platform/Utils/qconsolewidget.cpp \
    Platform/View/MainWindow.cpp \
    Platform/Utils/DataSubscriber.cpp \
    Platform/View/DockWidget.cpp \
    Platform/View/OpenOrderView.cpp \
    Platform/View/OpenOrderViewItem.cpp \
    Platform/Trader/CheckMessageThread.cpp \
    Platform/View/PositionView.cpp \
    Platform/View/PositionViewItem.cpp \
    Strategy/OneMinuteMomentum.cpp \
    Platform/Utils/Timer.cpp \
    Platform/Utils/Converter.cpp \
    Platform/Utils/SnapshotGenerator.cpp \
    Platform/Utils/TimedObject.cpp \
    Indicator/ShortTermWinnersAndLosers.cpp

HEADERS  += \
    Platform/Trader/TraderAssistant.h \
    Platform/Trader/Trader.h \
    Platform/Trader/AdapterWrapper.h \
    Platform/Strategy/Strategy.h \
    Platform/PosixSocketClient/EPosixClientSocketPlatform.h \
    Platform/PosixSocketClient/EPosixClientSocket.h \
    Platform/Commission/CommissionFactory.h \
    Platform/Commission/Commission.h \
    Platform/Enumerations/TickType.h \
    Platform/Enumerations/ServerJob.h \
    Platform/Enumerations/SecurityType.h \
    Platform/Enumerations/OrderStatus.h \
    Platform/Indicator/IndicatorManager.h \
    Platform/Indicator/Indicator.h \
    Platform/Performance/PerformanceManager.h \
    Platform/Position/PositionManager.h \
    Platform/Position/Position.h \
    Platform/Position/OpenOrder.h \
    Platform/Position/Instrument.h \
    Platform/Reports/StrategyReportManager.h \
    Platform/Reports/StrategyReport.h \
    Platform/Reports/Report.h \
    Platform/Reports/EventReport.h \
    Platform/Shared/TwsSocketClientErrors.h \
    Platform/Shared/StdAfx.h \
    Platform/Shared/shared_ptr.h \
    Platform/Shared/ScannerSubscription.h \
    Platform/Shared/OrderState.h \
    Platform/Shared/Order.h \
    Platform/Shared/IBString.h \
    Platform/Shared/HScrollListBox.h \
    Platform/Shared/Execution.h \
    Platform/Shared/EWrapper.h \
    Platform/Shared/EClientSocketBaseImpl.h \
    Platform/Shared/EClientSocketBase.h \
    Platform/Shared/EClient.h \
    Platform/Shared/Contract.h \
    Platform/Shared/CommonDefs.h \
    Platform/Startup/Service.h \
    Platform/Utils/TradingSchedule.h \
    Platform/Utils/TimeZone.h \
    Platform/Utils/StrategyFinder.h \
    Platform/Utils/SingleApplication.h \
    Platform/Utils/Date.h \
    Platform/Utils/ConfigurationManager.h \
    Platform/Utils/ClassFinder.h \
    Platform/Utils/Bootstrap.h \
    Platform/Trader/OrderManager.h \
    Platform/Trader/InstrumentManager.h \
    Platform/Utils/ThreadManager.h \
    Platform/typedefs.h \
    Platform/Strategy/StrategyManager.h \
    Strategy/TestStrategy.h \
    ActiveTickFeed/Utils/Streamer.h \
    ActiveTickFeed/Utils/stdafx.h \
    ActiveTickFeed/Utils/Session.h \
    ActiveTickFeed/Utils/Requestor.h \
    ActiveTickFeed/Utils/Helper.h \
    ActiveTickFeed/Shared/ATServerAPIDefines.h \
    ActiveTickFeed/Utils/ActiveTickApi.h \
    Platform/View/TableView.h \
    Platform/View/StrategyView.h \
    Platform/View/TableItem.h \
    Platform/View/InstrumentView.h \
    Platform/View/InstrumentViewItem.h \
    Platform/View/StrategyViewItem.h \
    Platform/Utils/redirect.h \
    Platform/Utils/qconsolewidget.h \
    Platform/View/MainWindow.h \
    Platform/Reports/EventReportManager.h \
    Platform/Utils/DataSubscriber.h \
    Platform/View/DockWidget.h \
    Platform/View/OpenOrderView.h \
    Platform/Model/OpenOrderModel.h \
    Platform/View/OpenOrderViewItem.h \
    Platform/Trader/CheckMessageThread.h \
    Platform/Enumerations/DataSource.h \
    Platform/View/PositionView.h \
    Platform/View/PositionViewItem.h \
    Platform/Model/PositionModel.h \
    Platform/Model/StrategyModel.h \
    Platform/Utils/Enum.h \
    Strategy/OneMinuteMomentum.h \
    Platform/Utils/Timer.h \
    Platform/Enumerations/PositionStatus.h \
    Platform/Model/Mode.h \
    Platform/Utils/Converter.h \
    Platform/Utils/SnapshotGenerator.h \
    Platform/Utils/TimedObject.h \
    Indicator/ShortTermWinnersAndLosers.h

macx: LIBS += -L$$PWD/bin/ -lActiveTickFeedAPI
macx: LIBS += -L$$PWD/bin/ -lboost_system
macx: LIBS += -L$$PWD/bin/ -lboost_thread

INCLUDEPATH += $$PWD/bin
DEPENDPATH += $$PWD/bin

INCLUDEPATH += $$PWD/ActiveTickFeed/include
INCLUDEPATH += $$PWD/ActiveTickFeed

ICON = Aims.icns

