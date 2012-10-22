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
#include "Platform/View/StrategyPositionView2.h"
#include "Platform/View/StrategyView2.h"

IOInterface::IOInterface():QObject()
{
    init();
}

void IOInterface::setupConnections()
{
    QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const TickerId)), MainWindow::mainWindow().getPositionView(), SLOT(addPosition(const StrategyId, const TickerId)), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(positionUpdatedForExecution(const StrategyId, const TickerId, const PositionDetail&)), MainWindow::mainWindow().getPositionView(), SLOT(updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&)), Qt::UniqueConnection);

    //for DB
    QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const TickerId)), &(IODatabase::ioDatabase()), SLOT(addPosition(const StrategyId, const TickerId)));


    //QObject::connect(this, SIGNAL(positionUpdatedForExecutionGUI(const StrategyId, const TickerId, const PositionDetail&)), MainWindow::mainWindow().getPositionView(), SLOT(updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&)), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(positionUpdatedForExecution(const StrategyId, const TickerId, const PositionDetail&)), &IODatabase::ioDatabase(), SLOT(updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&)), Qt::UniqueConnection);


    QObject::connect(this, SIGNAL(positionUpdatedForLastPrice(const StrategyId, const TickerId, const PositionDetail&)), MainWindow::mainWindow().getPositionView(), SLOT(updatePositionForLastPrice(const StrategyId, const TickerId, const PositionDetail&)), Qt::UniqueConnection);


    OpenOrderWidget* openOrderView = MainWindow::mainWindow().getOpenOrderView();

//    QObject::connect(this, SIGNAL(orderPlacedGUI(const OpenOrder&, const QString&)), openOrderView, SLOT(addOrder(const OpenOrder&, const QString&)));
//    QObject::connect(this, SIGNAL(orderPlacedDB(const OpenOrder&, const QString&)), &IODatabase::ioDatabase(), SLOT(addOrder(const OpenOrder&, const QString&)));

    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const OrderDetail&)), openOrderView, SLOT(addOrder(const OrderId, const OrderDetail&)));
    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const OrderDetail&)), &IODatabase::ioDatabase(), SLOT(addOrder(const OrderId, const OrderDetail&)));


    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)), openOrderView, SLOT(removeOrder(const OrderId)));

//    QObject::connect(this, SIGNAL(orderUpdatedDB(const OpenOrder&)), &IODatabase::ioDatabase(), SLOT(updateOrder(const OpenOrder&)));
//    QObject::connect(this, SIGNAL(orderUpdatedGUI(const OpenOrder&)), openOrderView, SLOT(updateOrder(const OpenOrder&)));

    QObject::connect(this, SIGNAL(orderUpdated(const OrderId, const OrderDetail&)), &IODatabase::ioDatabase(), SLOT(updateOrder(const OrderId, const OrderDetail&)));
    QObject::connect(this, SIGNAL(orderUpdated(const OrderId, const OrderDetail&)), openOrderView, SLOT(updateOrder(const OrderId, const OrderDetail&)));


    StrategyView2* strategyView = MainWindow::mainWindow().getStrategyView();
    QObject::connect(this, SIGNAL(strategyUpdated(const StrategyId, const PerformanceStats&)), strategyView, SLOT(updatePerformance(const StrategyId, const PerformanceStats&)));

    MessageView* messageView = MainWindow::mainWindow().getMessageView();
    connect(this, SIGNAL(eventReported(const QDateTime&, const String&, const String&, const MessageType)), messageView, SLOT(reportEvent(const QDateTime&, const String&, const String&, const MessageType)));

    InstrumentView* instrumentView = MainWindow::mainWindow().getInstrumentView();
    connect(this, SIGNAL(instrumentAdded(const TickerId)), instrumentView, SLOT(addInstrument(const TickerId)));
}

void IOInterface::init()
{
    //_eventReportSPtr = new EventReport();
    //_guiWindow = new MainWindow();
    //_dbInputOutput = new IODatabase();
     setupConnections();
}

InstrumentView* IOInterface::getInstrumentView()
{
    return MainWindow::mainWindow().getInstrumentView();
}

void IOInterface::setupMainwindow(const QMap<QString, QSize> &customSizeHints)
{
     //mainWindow()->setup(customSizeHints);
     //setupConnections();
}

//IOInterface::~IOInterface()
//{

//}

void IOInterface::addPosition(const StrategyId strategyId, const TickerId tickerId,  const OutputType type)
{
        emit positionCreated(strategyId, tickerId);
    /*switch(type)
    {
        emit positionCreated(strategyId, tickerId); break;
        //case DB:  emit positionCreatedDB(strategyId, tickerId); break;
        //case ALL: emit positionCreatedDB(strategyId, tickerId);
         //         emit positionCreatedGUI(strategyId, tickerId);break;
    }*/
}

void IOInterface::updatePositionForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    StrategyId strategyId = currentPosition->getStrategyId();
    TickerId tickerId = currentPosition->getTickerId();
    emit positionUpdatedForExecution(strategyId, tickerId, cumulativePosition->getNetPositionDetail());
    /*switch(type)
    {
        case GUI: emit positionUpdatedForExecutionGUI(strategyId, tickerId, cumulativePosition->getNetPositionDetail()); break;
        //case DB:  emit positionUpdatedForExecutionDB(strategyId, tickerId,currentPosition->getCurrentPositionDetail()); break;
        case ALL: emit positionUpdatedForExecutionGUI(strategyId, tickerId,cumulativePosition->getNetPositionDetail());
                  //emit positionUpdatedForExecutionDB(strategyId, tickerId,currentPosition->getCurrentPositionDetail()); break;
    }*/
}

void IOInterface::updatePositionForLastPrice(const Position* position,  const OutputType type)
{
    StrategyId strategyId = position->getStrategyId();
    TickerId tickerId = position->getTickerId();
    //double runningPnl = position->getNetPositionDetail().getRunningPnl();
    //double PnL = position->getNetPositionDetail().getPnL();
     //now emit signals
    //emit positionUpdatedForLastPrice(strategyId, tickerId, runningPnl, PnL);
    emit positionUpdatedForLastPrice(strategyId, tickerId, position->getNetPositionDetail());
}

void IOInterface::updateOrderExecution(const OpenOrder* openOrder, const OutputType type)
{
//    switch(type)
//    {
//        case GUI: emit orderUpdatedGUI(*openOrder);break;
//        case DB:  emit orderUpdatedDB(*openOrder);break;
//        case ALL: emit orderUpdatedDB(*openOrder);
//                  emit orderUpdatedGUI(*openOrder);break;
//    }
}

void IOInterface::updateOrderExecution(const OrderId orderId, const OrderDetail& orderDetail, const OutputType type)
{
    emit orderUpdated(orderId, orderDetail);
    /*switch(type)
    {
        case GUI: emit orderUpdatedGUI(orderId, orderDetail);break;
        case DB:  emit orderUpdatedDB(orderId, orderDetail);break;
        case ALL: emit orderUpdatedDB(orderId, orderDetail);
                  emit orderUpdatedGUI(orderId, orderDetail);break;
    }*/
}


void IOInterface::addOrder(const OpenOrder* openOrder, const String& strategyName, const OutputType type)
{
//    switch(type)
//    {
//        case GUI:  emit orderPlacedGUI(*openOrder, strategyName);break;
//        case DB:  emit orderPlacedDB(*openOrder, strategyName);break;
//        case ALL:  emit orderPlacedGUI(*openOrder, strategyName);
//                     emit orderPlacedDB(*openOrder, strategyName);break;
//    }
}

void IOInterface::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const OutputType type)
{
    emit orderPlaced(orderId, orderDetail);
    /*switch(type)
    {
        case GUI:  emit orderPlacedGUI(orderId, orderDetail);break;
        case DB:  emit orderPlacedDB(orderId, orderDetail);break;
        case ALL:  emit orderPlacedGUI(orderId, orderDetail);
                     emit orderPlacedDB(orderId, orderDetail);break;
    }*/
}


void IOInterface::removeOrder(const OrderId orderId, const OutputType type)
{
    emit orderDeleted(orderId);
}

void IOInterface::updateOrderStatus(const OpenOrder* openOrder, const OutputType type)
{
    OrderId orderId = openOrder->getOrderId();
    //String orderStatus =  openOrder->getOrderStatusString();
    OrderStatus status = openOrder->getOrderDetail().getOrderStatus();
   //emit sigmal to order view
    emit orderStatusUpdated(orderId, status);
    /*switch(type)
    {
        case GUI: emit orderStatusUpdatedGUI(orderId, status); break;
        case DB:  emit orderStatusUpdatedDB(orderId, status); break;
        case ALL: emit orderStatusUpdatedGUI(orderId, status);
                  emit orderStatusUpdatedDB(orderId, status);break;
    }*/
}

void IOInterface::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats, const OutputType type)
{
    emit strategyUpdated(strategyId, performanceStats);
    /*switch(type)
    {
        case GUI: emit strategyUpdatedGUI(strategyId, performanceStats);break;
        case DB:  emit strategyUpdatedDB(strategyId, performanceStats);break;
        case ALL: emit strategyUpdatedGUI(strategyId, performanceStats);
                  emit strategyUpdatedDB(strategyId, performanceStats); break;
    }*/
}

void IOInterface::reportEvent(const String& reporter, const String& report, const MessageType type)
{
      emit eventReported(QDateTime::currentDateTime(), reporter, report, type);
}

void IOInterface::addInstrument(const TickerId instrumentId, const InstrumentContract& instrumentContract)
{
     //emit instrumentAdded(instrumentId, instrumentContract);
}

void IOInterface::addInstrument(const TickerId tickerId)
{
     emit instrumentAdded(tickerId);
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

//IOInterface::~IOInterface()
//{

//}

IOInterface* IOInterface::_instance = NULL;


