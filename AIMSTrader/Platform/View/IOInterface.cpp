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

IOInterface::IOInterface():QObject()//, Singleton<IOInterface>()
{
    init();
}

void IOInterface::setupConnections()
{
    //for GUI
    QObject::connect(this, SIGNAL(positionCreatedGUI(const StrategyId, const TickerId)), MainWindow::mainWindow().getPositionView(), SLOT(addPosition(const StrategyId, const TickerId)));

    //for DB
    //IODatabase ioDatabase = ioDatabase();
    QObject::connect(this, SIGNAL(positionCreatedDB(const StrategyId, const TickerId)), &(IODatabase::ioDatabase()), SLOT(addPosition(const StrategyId, const TickerId)));


    QObject::connect(this, SIGNAL(positionUpdatedForExecutionGUI(const Position&)), MainWindow::mainWindow().getPositionView(), SLOT(updatePositionForExecution(const Position&)), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(positionUpdatedForExecutionDB(const Position&)), &IODatabase::ioDatabase(), SLOT(updatePositionForExecution(const Position&)), Qt::UniqueConnection);


    QObject::connect(this, SIGNAL(positionUpdatedForLastPrice(const StrategyId, const TickerId, const double, const double)), MainWindow::mainWindow().getPositionView(), SLOT(updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double)));


    OpenOrderWidget* openOrderView = MainWindow::mainWindow().getOpenOrderView();

    QObject::connect(this, SIGNAL(orderPlacedGUI(const OpenOrder&, const QString&)), openOrderView, SLOT(addOrder(const OpenOrder&, const QString&)));
    QObject::connect(this, SIGNAL(orderPlacedDB(const OpenOrder&, const QString&)), &IODatabase::ioDatabase(), SLOT(addOrder(const OpenOrder&, const QString&)));

    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)), openOrderView, SLOT(removeOrder(const OrderId)));

    QObject::connect(this, SIGNAL(orderUpdatedDB(const OpenOrder&)), &IODatabase::ioDatabase(), SLOT(updateOrder(const OpenOrder&)));
    QObject::connect(this, SIGNAL(orderUpdatedGUI(const OpenOrder&)), openOrderView, SLOT(updateOrder(const OpenOrder&)));


    StrategyView* strategyView = MainWindow::mainWindow().getStrategyView();
    QObject::connect(this, SIGNAL(strategyUpdatedGUI(const StrategyId, const PerformanceStats&)), strategyView, SLOT(updatePerformance(const StrategyId, const PerformanceStats&)));
    QObject::connect(this, SIGNAL(strategyUpdatedDB(const StrategyId, const PerformanceStats&)), &IODatabase::ioDatabase(), SLOT(updatePerformance(const StrategyId, const PerformanceStats&)));

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

void IOInterface::addPosition(const StrategyId strategyId, const TickerId instrumentId,  const OutputType type)
{
    switch(type)
    {
        case GUI: emit positionCreatedGUI(strategyId, instrumentId); break;
        case DB:  emit positionCreatedDB(strategyId, instrumentId); break;
        case ALL: emit positionCreatedDB(strategyId, instrumentId);
                  emit positionCreatedGUI(strategyId, instrumentId);break;
    }
}

void IOInterface::updatePositionForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    switch(type)
    {
        case GUI: emit positionUpdatedForExecutionGUI(*cumulativePosition); break;
        case DB:  emit positionUpdatedForExecutionDB(*currentPosition); break;
        case ALL: emit positionUpdatedForExecutionGUI(*cumulativePosition);
                  emit positionUpdatedForExecutionDB(*currentPosition); break;
    }
}

void IOInterface::updatePositionForLastPrice(const Position* position,  const OutputType type)
{
    StrategyId strategyId = position->getStrategyId();
    TickerId instrumentId = position->getTickerId();
    double runningPnl = position->getRunningPnl();
    double PnL = position->getPnL();
     //now emit signals
    emit positionUpdatedForLastPrice(strategyId, instrumentId, runningPnl, PnL);
}

void IOInterface::updateOrderExecution(const OpenOrder* openOrder, const OutputType type)
{
    switch(type)
    {
        case GUI: emit orderUpdatedGUI(*openOrder);break;
        case DB:  emit orderUpdatedDB(*openOrder);break;
        case ALL: emit orderUpdatedDB(*openOrder);
                  emit orderUpdatedGUI(*openOrder);break;
    }
}

void IOInterface::addOrder(const OpenOrder* openOrder, const String& strategyName, const OutputType type)
{
    switch(type)
    {
        case GUI:  emit orderPlacedGUI(*openOrder, strategyName);break;
        case DB:  emit orderPlacedDB(*openOrder, strategyName);break;
        case ALL:  emit orderPlacedGUI(*openOrder, strategyName);
                     emit orderPlacedDB(*openOrder, strategyName);break;
    }
}

void IOInterface::removeOrder(const OrderId orderId, const OutputType type)
{
    emit orderDeleted(orderId);
}

void IOInterface::updateOrderStatus(const OpenOrder* openOrder, const OutputType type)
{
    OrderId orderId = openOrder->getOrderId();
    //String orderStatus =  openOrder->getOrderStatusString();
    OrderStatus status = openOrder->getOrderStatus();
   //emit sigmal to order view
    switch(type)
    {
        case GUI: emit orderStatusUpdatedGUI(orderId, status); break;
        case DB:  emit orderStatusUpdatedDB(orderId, status); break;
        case ALL: emit orderStatusUpdatedGUI(orderId, status);
                  emit orderStatusUpdatedDB(orderId, status);break;
    }
}

void IOInterface::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats, const OutputType type)
{
    switch(type)
    {
        case GUI: emit strategyUpdatedGUI(strategyId, performanceStats);break;
        case DB:  emit strategyUpdatedDB(strategyId, performanceStats);break;
        case ALL: emit strategyUpdatedGUI(strategyId, performanceStats);
                  emit strategyUpdatedDB(strategyId, performanceStats); break;
    }
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


