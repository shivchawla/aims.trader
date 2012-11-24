#-------------------------------------------------
#
# Project created by QtCreator 2012-03-10T11:26:00
#
#-------------------------------------------------

TEMPLATE += app
CONFIG += console
#CONFIG += qt
#CONFIG += x86_64
QT += core gui sql network
CONFIG-=app_bundle

SOURCES += main.cpp\
    Platform/Trader/TraderAssistant.cpp \
    Platform/Trader/Trader.cpp \
    Platform/Trader/AdapterWrapper.cpp \
    Platform/Strategy/Strategy.cpp \
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
    Platform/View/StrategyView.cpp \
    Platform/View/InstrumentView.cpp \
    Platform/View/InstrumentViewItem.cpp \
    Platform/View/StrategyViewItem.cpp \
    Platform/Utils/redirect.cpp \
    Platform/Utils/qconsolewidget.cpp \
    Platform/View/MainWindow.cpp \
    Platform/Utils/DataSubscriber.cpp \
    Platform/View/DockWidget.cpp \
    Platform/View/OpenOrderView.cpp \
    Platform/View/OpenOrderViewItem.cpp \
    Platform/Trader/CheckMessageThread.cpp \
    Strategy/OneMinuteMomentum.cpp \
    Platform/Utils/Timer.cpp \
    Platform/Utils/Converter.cpp \
    Platform/Utils/SnapshotGenerator.cpp \
    Platform/Utils/TimedObject.cpp \
    Indicator/ShortTermWinnersAndLosers.cpp\
    ActiveTickFeed/Utils/ActiveTickSession.cpp \
    Platform/View/StrategyPositionViewItem.cpp \
    Platform/View/StrategyPositionView.cpp \
    Platform/Utils/TestDataGenerator.cpp \
    Platform/View/TableCellItem.cpp \
    Platform/View/SpecialDockWidget.cpp \
    Platform/View/ColorDock.cpp \
    Platform/View/SubMainWindow.cpp \
    Platform/View/HeaderListWidget.cpp \
    Platform/View/CustomizeHeaderDialog.cpp \
    Platform/View/HeaderListWidgetItem.cpp \
    Platform/View/OrderEntryDialog.cpp \
    Platform/View/MessageView.cpp \
    Platform/View/MessageViewItem.cpp \
    Platform/View/OpenOrderWidget.cpp \
    Strategy/FractionalMomentum.cpp \
    Indicator/FractionalMomentumIndicator.cpp \
    Platform/PosixSocketClient/EPosixClientSocket.cpp \
    Platform/PosixSocketClient/EClientSocketBase.cpp \
    Strategy/StrategyImpl.cpp \
    Platform/Strategy/StrategyOutput.cpp \
    Platform/View/IOInterface.cpp \
    Platform/View/IODatabase.cpp \
    Platform/View/StrategyPositionModel2.cpp \
    Platform/View/StrategyPositionView2.cpp \
    Platform/View/OpenOrderModel2.cpp \
    Platform/View/OpenOrderView2.cpp \
    Platform/View/StrategyView2.cpp \
    Platform/View/StrategyModel2.cpp \
    Platform/View/StrategyPositionDetailModel.cpp \
    Platform/View/SearchLineEdit.cpp \
    Platform/View/InstrumentViewSubscriber.cpp \
    Platform/View/StrategyViewItem2.cpp \
    Platform/View/TableViewItem2.cpp \
    Platform/View/StrategyView3.cpp \
    Platform/View/InstrumentView2.cpp \
    Platform/View/InstrumentViewItem2.cpp \
    Platform/View/InstrumentViewWidget.cpp \
    Platform/View/StrategyPositionView3.cpp \
    Platform/View/OpenOrderView3.cpp

HEADERS  += \
    Platform/Trader/TraderAssistant.h \
    Platform/Trader/Trader.h \
    Platform/Trader/AdapterWrapper.h \
    Platform/Strategy/Strategy.h \
    Platform/Commission/CommissionFactory.h \
    Platform/Commission/Commission.h \
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
    Platform/Strategy/StrategyManager.h \
    Strategy/TestStrategy.h \
    ActiveTickFeed/Utils/Streamer.h \
    ActiveTickFeed/Utils/stdafx.h \
    ActiveTickFeed/Utils/Session.h \
    ActiveTickFeed/Utils/Requestor.h \
    ActiveTickFeed/Utils/Helper.h \
    ActiveTickFeed/Utils/ActiveTickApi.h \
    Platform/View/TableView.h \
    Platform/View/StrategyView.h \
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
    Platform/Model/StrategyModel.h \
    Platform/Utils/Enum.h \
    Strategy/OneMinuteMomentum.h \
    Platform/Utils/Timer.h \
    Platform/Model/Mode.h \
    Platform/Utils/Converter.h \
    Platform/Utils/SnapshotGenerator.h \
    Platform/Utils/TimedObject.h \
    Indicator/ShortTermWinnersAndLosers.h\
    Platform/Enumerations/DataSource.h \
    Platform/Model/StrategyPositionModel.h \
    Platform/Model/InstrumentPositionModel.h \
    Platform/View/StrategyPositionViewItem.h \
    Platform/View/StrategyPositionView.h \
    Platform/Utils/TestDataGenerator.h \
    Platform/View/TableViewItem.h \
    Platform/Utils/Singleton.h \
    Platform/View/TableCellItem.h \
    Platform/View/SpecialDockWidget.h \
    Platform/View/ColorDock.h \
    Platform/Model/InstrumentModel.h \
    Platform/Model/DataModel.h \
    Platform/View/SubMainWindow.h \
    Platform/View/HeaderListWidget.h \
    Platform/View/CustomizeHeaderDialog.h \
    Platform/View/HeaderListWidgetItem.h \
    Platform/View/AddOrderDialog.h \
    Platform/View/OrderEntryDialog.h \
    Platform/View/MessageView.h \
    Platform/View/MessageViewItem.h \
    Platform/Model/MessageModel.h \
    Platform/View/OpenOrderWidget.h \
    Strategy/FractionalMomentum.h \
    Indicator/FractionalMomentumIndicator.h \
    Platform/PosixSocketClient/EPosixClientSocketPlatform.h \
    Platform/PosixSocketClient/EPosixClientSocket.h \
    AimsTraderGlobal.h \
    Strategy/StrategyFactory.h \
    Strategy/StrategyImpl.h \
    Platform/Strategy/StrategyOutput.h \
    Platform/View/IODatabase.h \
    Platform/View/IOInterface.h \
    Strategy/PercentileMomentum.h \
    Platform/View/StrategyPositionModel2.h \
    Platform/View/StrategyPositionView2.h \
    Platform/View/TableView2.h \
    Platform/View/OpenOrderModel2.h \
    Platform/View/GeneralModel2.h \
    Platform/View/OpenOrderView2.h \
    Platform/View/StrategyView2.h \
    Platform/View/StrategyModel2.h \
    Platform/View/StrategyPositionDetailModel.h \
    Platform/View/SearchLineEdit.h \
    Platform/View/InstrumentViewSubscriber.h \
    Platform/View/StrategyViewItem2.h \
    Platform/View/TableViewItem2.h \
    Platform/View/TableView3.h \
    Platform/View/StrategyView3.h \
    Platform/View/InstrumentView2.h \
    Platform/View/InstrumentViewItem2.h \
    Platform/View/InstrumentViewWidget.h \
    Platform/View/StrategyPositionView3.h \
    Platform/View/StrategyPositionViewItem2.h \
    Platform/View/OpenOrderView3.h \
    Platform/View/OpenOrderViewItem2.h

INCLUDEPATH += $$PWD/../Shared/
INCLUDEPATH += $$PWD/../API/ActiveTickFeed/include/
INCLUDEPATH += $$PWD/../Framework/


INCLUDEPATH += /usr/local/mysql/lib
INCLUDEPATH += /usr/local/mysql/include

macx: LIBS += -L$$PWD/../API/ActiveTickFeed/lib/ -lActiveTickFeedAPI
macx: LIBS += -L$$PWD/../API/ActiveTickFeed/lib/ -lboost_system
macx: LIBS += -L$$PWD/../API/ActiveTickFeed/lib/ -lboost_thread
macx: LIBS += -L$$PWD/../Framework-Debug/ -lFramework.1.0.0


ICON = Aims.icns

FORMS += \
    Platform/View/CustomizeHeaderDialog.ui

RESOURCES += \
    Resources.qrc

