#include "Platform/View/OutputInterface.h"
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

OutputInterface::OutputInterface():QObject(), Singleton<OutputInterface>()
{
    init();
}

void OutputInterface::setupConnections()
{
    //QThread* thread = ThreadManager::Instance()->requestThread();
    //moveToThread(thread);
    QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const TickerId)), _guiWindow->getPositionView(), SLOT(addPosition(const StrategyId, const TickerId)));
    //QObject::connect(this, SIGNAL(positionRemoved(const StrategyId, const PositionId)), MainWindow::mainWindow()->getPositionView(), SLOT(removePosition(const StrategyId, const PositionId)));
    QObject::connect(this, SIGNAL(positionUpdatedForExecution(const StrategyId, const TickerId, const long , const long , const long , const double , const double , const double, const double, const double, const double, const double, const double, const double, const double )),
                    _guiWindow->getPositionView(), SLOT(updatePositionForExecution(const StrategyId, const TickerId, const long, const long, const long, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double)));

    QObject::connect(this, SIGNAL(positionUpdatedForLastPrice(const StrategyId, const TickerId, const double, const double)), _guiWindow->getPositionView(), SLOT(updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double)));

    OpenOrderWidget* openOrderView = _guiWindow->getOpenOrderView();
    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)), openOrderView, SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)), openOrderView, SLOT(removeOrder(const OrderId)));
    QObject::connect(this, SIGNAL(orderUpdated(const OrderId,const long,const long, const double, const double)), openOrderView, SLOT(onExecutionUpdate(const OrderId, const long, const long, const double, const double)));
    //QObject::connect(this, SIGNAL(orderStatusUpdated(const OrderId, const String)), openOrderView, SLOT(onStatusUpdate(const OrderId, const String)));
    QObject::connect(this, SIGNAL(orderStatusUpdated(const OrderId, const OrderStatus)), openOrderView, SLOT(onStatusUpdate(const OrderId, const OrderStatus)));

    StrategyView* strategyView = _guiWindow->getStrategyView();
    QObject::connect(this, SIGNAL(strategyUpdated(const StrategyId, const PerformanceStats&)), strategyView, SLOT(updateStrategy(const StrategyId, const PerformanceStats&)));

    MessageView* messageView = _guiWindow->getMessageView();
    connect(this, SIGNAL(eventReported(const String, const String, const String, const MessageType)), messageView, SLOT(reportEvent(const String, const String, const String, const MessageType)));

    InstrumentView* instrumentView = _guiWindow->getInstrumentView();
    connect(this, SIGNAL(instrumentAdded(TickerId,Contract)), instrumentView, SLOT(addInstrument(TickerId,Contract)));

}

void OutputInterface::init()
{
    _eventReportSPtr = new EventReport();
    _guiWindow = new MainWindow();
    //_dbSession = new DatabaseSession();
}

InstrumentView* OutputInterface::getInstrumentView()
{
    return _guiWindow->getInstrumentView();
}

//DatabaseSession* OutputInterface::getDatabaseSession()
//{
//    return _dbSession;
//}

void OutputInterface::setupMainwindow(const QMap<QString, QSize> &customSizeHints)
{
    _guiWindow->setup(customSizeHints);
     setupConnections();
}

OutputInterface::~OutputInterface()
{}

void OutputInterface::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    //_strategyInstrumentDiagnostics.addDiagnostic(strategyId, tickerId);
     emit positionCreated(strategyId, tickerId);
}

void OutputInterface::updatePositionForExecution(const Position* position)
{
    //now emit signals
    StrategyId strategyId = position->getStrategyId();
    TickerId tickerId = position->getTickerId();
    long sharesBought = position->getSharesBought();
    long sharesSold = position->getSharesSold();
    long netShares = position->getNetShares();
    double avgBought = position->getAvgBought();
    double avgSold = position->getAvgSold();
    double totalValueBought = position->getTotalValueBought();
    double totalValueSold = position->getTotalValueSold();
    double netTotal = position->getNetTotal();
    double realizedPnl = position->getRealizedPnl();
    double runningPnl = position->getRunningPnl();
    double PnL = position->getPnL();
    double totalCommission = position->getTotalCommission();
    double netTotalIncCommission = position->getNetTotalIncCommission();
    emit positionUpdatedForExecution(strategyId, tickerId, sharesBought, sharesSold, netShares, avgBought, avgSold, totalValueBought, totalValueSold, netTotal, realizedPnl, runningPnl, PnL, totalCommission, netTotalIncCommission) ;
}

void OutputInterface::updatePositionForLastPrice(const Position* position)
{
    StrategyId strategyId = position->getStrategyId();
    TickerId tickerId = position->getTickerId();
    double runningPnl = position->getRunningPnl();
    double PnL = position->getPnL();
     //now emit signals
    emit positionUpdatedForLastPrice(strategyId, tickerId, runningPnl, PnL);
}

void OutputInterface::updateOrderExecution(const OpenOrder* openOrder)
{
     //emit sigmal to order view
    OrderId orderId = openOrder->getOrderId();
    long filledShares = openOrder->getFilledShares();
    long pendingShares = openOrder->getPendingShares();
    double avgFillPrice = openOrder->getAvgFillPrice();
    double lastFillPrice = openOrder->getLastFillPrice();

    emit orderUpdated(orderId, filledShares, pendingShares, avgFillPrice, lastFillPrice);
    updateOrderStatus(openOrder);
}

void OutputInterface::addOrder(const OpenOrder* openOrder, const String& strategyName)
{
    OrderId orderId = openOrder->getOrderId();
    Order order =     openOrder->getOrder();
    Contract contract = openOrder->getContract();
    //emit sigmal to order view
    emit orderPlaced(orderId, order, contract, strategyName);
    updateOrderStatus(openOrder);
}

void OutputInterface::removeOrder(const OrderId orderId)
{
    //emit sigmal to order view
    emit orderDeleted(orderId);
}

void OutputInterface::updateOrderStatus(const OpenOrder* openOrder)
{
    OrderId orderId = openOrder->getOrderId();
    //String orderStatus =  openOrder->getOrderStatusString();
    OrderStatus status = openOrder->getOrderStatus();
   //emit sigmal to order view
    //emit orderStatusUpdated(orderId, orderStatus);
    emit orderStatusUpdated(orderId, status);
}


void OutputInterface::updateStrategy(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    emit strategyUpdated(strategyId, performanceStats);
}

void OutputInterface::reportEvent(const String& reporter, const String& report, const MessageType type)
{
    emit eventReported(QDateTime::currentDateTime().toString(), reporter, report, type);
}

void OutputInterface::addInstrument(const TickerId tickerId, const Contract& contract)
{
    emit instrumentAdded(tickerId, contract);
}


/*void OutputInterface::onExecutionUpdate(const Position* position)
{
   //_strategyInstrumentDiagnostics.updateDiagnostic(position);
    emit positionUpdatedForExection(position);
   //inform output users
}

void OutputInterface::onLastPriceUpdate(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
   //_strategyInstrumentDiagnostics.updateDiagnostic(strategyId, tickerId, runningPnl, pnl);
   //inform output users
    emit positionUpdatedForLastPrice(strategyId, tickerId, runningPnl, pnl);
}

void OutputInterface::removePosition(const StrategyId, const PositionId)
{
   //inform output users
}*/

/*void OutputInterface::updatePosition(const StrategyId strategyId, const TickerId tickerId, const ExecutionStatus& status)
{
   _strategyInstrumentDiagnostics.updatePosition(strategyId, tickerId, status);
    const StrategyInstrumentData* data = _strategyInstrumentDiagnostics.getStrategyInstrumentData(strategyId, tickerId);
    if(data)
    {
        emit executionUpdated(strategyId, tickerId, data->getSharesBought(), data->getSharesSold(), data->getNetShares(), data->getAvgBought(), data->getAvgSold(), data->getTotalValueBought(), data->getTotalValueSold(), data->getNetTotal(), data->getRealizedPnl(), data->getRunningPnl(), data->getPnL(), data->getTotalCommision(), data->getNetTotalIncCommission());
    }
}

void OutputInterface::updatePosition(const TickerId tickerId, const double lastPrice)
{
    //go over all the strategies for this tickerId
    // check the net positions
    //confirm how the logic works in IB
}

void OutputInterface::updatePosition(const StrategyId strategyId, const TickerId tickerId, const double lastprice)
{
    _strategyInstrumentDiagnostics.updatePosition(strategyId, tickerId, lastprice);
    const StrategyInstrumentData* data = _strategyInstrumentDiagnostics.getStrategyInstrumentData(strategyId, tickerId);
    if(data)
    {
        emit lastPriceUpdated(strategyId, tickerId, data->getRunningPnl(), data->getPnL());
    }
}

void OutputInterface::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double value)
{
    switch(tickType)
    {
        case LAST: updatePosition(tickerId, value); break;
        default: break;
    }
}*/


