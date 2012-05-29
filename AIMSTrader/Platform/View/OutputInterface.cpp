#include "Platform/View/OutputInterface.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/View/MainWindow.h"
#include "Platform/View/StrategyPositionView.h"
#include "Platform/View/OpenOrderView.h"

OutputInterface::OutputInterface():QObject()
{
    //QThread* thread = ThreadManager::Instance()->requestThread();
    //moveToThread(thread);
    QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const TickerId)), MainWindow::mainWindow()->getPositionView(), SLOT(addPosition(const StrategyId, const TickerId)));
    //QObject::connect(this, SIGNAL(positionRemoved(const StrategyId, const PositionId)), MainWindow::mainWindow()->getPositionView(), SLOT(removePosition(const StrategyId, const PositionId)));
    QObject::connect(this, SIGNAL(positionUpdatedForExecution(const StrategyId, const TickerId, const long , const long , const long , const double , const double , const double, const double, const double, const double, const double, const double, const double, const double )),
                     MainWindow::mainWindow()->getPositionView(), SLOT(updatePositionForExecution(const StrategyId, const TickerId, const long, const long, const long, const double, const double, const double, const double, const double, const double, const double, const double, const double, const double)));

    QObject::connect(this, SIGNAL(positionUpdatedForLastPrice(const StrategyId, const TickerId, const double, const double)), MainWindow::mainWindow()->getPositionView(), SLOT(updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double)));

    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)), openOrderView, SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)), openOrderView, SLOT(removeOrder(const OrderId)));
    QObject::connect(this, SIGNAL(orderUpdated(const OrderId,const long,const long, const double, const double)), openOrderView, SLOT(onExecutionUpdate(const OrderId, const long, const long, const double, const double)));
    QObject::connect(this, SIGNAL(orderStatusUpdated(const OrderId, const String)), openOrderView, SLOT(onStatusUpdate(const OrderId, const String)));
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

void OutputInterface::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const Position* position)
{
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

}

void OutputInterface::addOrder(const OpenOrder* openOrder)
{
    OrderId orderId = openOrder->getOrderId();
    Order order =     openOrder->getOrder();
    Contract contract = openOrder->getContract();
    //emit sigmal to order view
    emit orderPlaced(orderId, order, contract, "");
}

void OutputInterface::removeOrder(const OrderId orderId)
{
    //emit sigmal to order view
    emit orderDeleted(orderId);
}

void OutputInterface::updateOrderStatus(const OpenOrder* openOrder)
{
    OrderId orderId = openOrder->getOrderId();
    String orderStatus =  openOrder->getOrderStatusString();
   //emit sigmal to order view
    emit orderStatusUpdated(orderId, orderStatus);
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


