#include "Platform/View/IOInterface.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/View/MainWindow.h"
#include "Platform/View/StrategyPositionView.h"
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/OpenOrderWidget.h"
#include "Platform/View/StrategyView.h"
#include "Platform/View/MessageView.h"
#include "Platform/Reports/EventReport.h"
#include <QDateTime>
#include "Platform/View/IODatabase.h"
#include "Platform/View/StrategyPositionView3.h"
#include "Platform/View/StrategyView2.h"
#include "Platform/View/StrategyView3.h"
#include "Platform/Position/SpreadManager.h"
#include "Platform/View/InstrumentViewWidget.h"
#include "Platform/View/TradeView.h"

IOInterface::IOInterface()
{
    init();
}

void IOInterface::setupConnections()
{

}

void IOInterface::init()
{
    //_eventReportSPtr = new EventReport();
    //_guiWindow = new MainWindow();
    //_dbInputOutput = new IODatabase();

    _ioDatabase  = &IODatabase::ioDatabase();
    _openOrderView = MainWindow::mainWindow().getOpenOrderView();
    _strategyView = MainWindow::mainWindow().getStrategyView();
    _strategyPositionView = MainWindow::mainWindow().getPositionView();
    _instrumentViewWidget = MainWindow::mainWindow().getInstrumentView();
    _messageView = MainWindow::mainWindow().getMessageView();
    _tradeView = MainWindow::mainWindow().getTradeView();
    setupConnections();
}

InstrumentViewWidget* IOInterface::getInstrumentView()
{
    return MainWindow::mainWindow().getInstrumentView();
}

void IOInterface::setupMainwindow(const QMap<QString, QSize> &customSizeHints)
{}

void IOInterface::addPosition(const StrategyId strategyId, const TickerId tickerId,  const OutputType type)
{
    QMetaObject::invokeMethod(_strategyPositionView, "addPosition", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(TickerId, tickerId));
    QMetaObject::invokeMethod(_ioDatabase, "addPosition", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(TickerId, tickerId));
}

void IOInterface::updatePositionForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    StrategyId strategyId = currentPosition->getStrategyId();
    TickerId tickerId = currentPosition->getTickerId();

    QMetaObject::invokeMethod(_strategyPositionView, "updatePositionForExecution", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(TickerId, tickerId), Q_ARG(PositionDetail, cumulativePosition->getNetPositionDetail()));
    QMetaObject::invokeMethod(_ioDatabase, "updatePositionForExecution", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(TickerId, tickerId), Q_ARG(PositionDetail, cumulativePosition->getNetPositionDetail()));
}

void IOInterface::updatePositionForLastPrice(const Position* position,  const OutputType type)
{
    StrategyId strategyId = position->getStrategyId();
    TickerId tickerId = position->getTickerId();

    QMetaObject::invokeMethod(_strategyPositionView, "updatePositionForLastPrice", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(TickerId, tickerId), Q_ARG(double, position->getRunningPnl()), Q_ARG(double, position->getNetPnL()));
}

void IOInterface::updateSpread(const Spread* spread, const OutputType type)
{
    TickerId tickerId1 = spread->getFirstPosition()->getTickerId();
    TickerId tickerId2 = spread->getSecondPosition()->getTickerId();

    SpreadId spreadId = spread->getSpreadId();
    StrategyId strategyId = spread->getStrategyId();

    if(type==ALL || type==DB)
    {
        QMetaObject::invokeMethod(_ioDatabase, "updateSpreadPosition", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId),
                                  Q_ARG(SpreadId, spreadId), Q_ARG(TickerId, tickerId1), Q_ARG(PositionDetail, spread->getFirstPosition()->getNetPositionDetail()));

        QMetaObject::invokeMethod(_ioDatabase, "updateSpreadPosition", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId),
                                  Q_ARG(SpreadId, spreadId), Q_ARG(TickerId, tickerId2), Q_ARG(PositionDetail, spread->getSecondPosition()->getNetPositionDetail()));

        QMetaObject::invokeMethod(_ioDatabase, "updateSpread", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId),
                                  Q_ARG(SpreadId, spreadId), Q_ARG(SpreadDetail, spread->getSpreadDetail()));
    }
}

void IOInterface::updateOrderExecution(const OpenOrder* openOrder, const OutputType type)
{}

void IOInterface::updateOrderExecution(const OrderId orderId,const OrderDetail& orderDetail, const OutputType type)
{
    QMetaObject::invokeMethod(_openOrderView,"updateOrder", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
    QMetaObject::invokeMethod(_ioDatabase,"updateOrder", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
    QMetaObject::invokeMethod(_tradeView,"addTrade", Qt::QueuedConnection, Q_ARG(OrderDetail, orderDetail));
}


void IOInterface::addOrder(const OpenOrder* openOrder, const String& strategyName, const OutputType type)
{}

void IOInterface::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const OutputType type)
{
    QMetaObject::invokeMethod(_openOrderView, "addOrder", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
    QMetaObject::invokeMethod(_ioDatabase, "addOrder", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
}


void IOInterface::removeOrder(const OrderId orderId, const OutputType type)
{
    QMetaObject::invokeMethod(_openOrderView, "removeOrder", Qt::QueuedConnection, Q_ARG(OrderId, orderId));
}

void IOInterface::updateOrderStatus(const OpenOrder* openOrder, const OutputType type)
{
    OrderId orderId = openOrder->getOrderId();
    OrderStatus status = openOrder->getOrderDetail().status;
   //emit sigmal to order view
    QMetaObject::invokeMethod(_openOrderView, "onStatusUpdate", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderStatus, status));
}

void IOInterface::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats, const OutputType type)
{
    QMetaObject::invokeMethod(_strategyView, "updatePerformance", Qt::QueuedConnection, Q_ARG(StrategyId, strategyId), Q_ARG(PerformanceStats, performanceStats));
}

void IOInterface::reportEvent(const String& reporter, const String& report, const MessageType type)
{
    Logger::MessageType t  = Logger::INFO;
    switch(type)
    {
        case INFO : t = Logger::INFO; break;
        case DEBUG : t = Logger::DEBUG; break;
        case CRITICAL: t = Logger::CRITICAL; break;
        case WARNING: t = Logger::WARNING; break;
    }

    Logger::log(t)<<QDateTime::currentDateTime()<<", "<<reporter<<", "<<report;

    if(type != DEBUG)
        QMetaObject::invokeMethod(_messageView, "reportEvent", Qt::AutoConnection, Q_ARG(QDateTime, QDateTime::currentDateTime()),
                                  Q_ARG(String, reporter), Q_ARG(String, report), Q_ARG(MessageType, type));
}

void IOInterface::addInstrument(const TickerId instrumentId, const InstrumentContract& instrumentContract)
{}

void IOInterface::addInstrument(const TickerId tickerId)
{
    QMetaObject::invokeMethod(_instrumentViewWidget, "onAddInstrument", Qt::QueuedConnection, Q_ARG(TickerId, tickerId));
}

IOInterface* IOInterface::_instance = NULL;


