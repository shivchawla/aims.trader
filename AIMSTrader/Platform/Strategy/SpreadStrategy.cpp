#include "Platform/Strategy/SpreadStrategy.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Position/SpreadManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/View/IODatabase.h"

/*
* Empty Constructor for SpreadStrategy
*/
SpreadStrategy::SpreadStrategy(): Strategy()
{
    _spreadManager = new SpreadManager(this);
    _strategyType = Spread_StrategyType;
}

/*
* Constructor for SpreadStrategy
*/
SpreadStrategy::SpreadStrategy(const String& string): Strategy(string)
{
    _spreadManager = new SpreadManager(this);
    _strategyType = Spread_StrategyType;
}

/*
* Place Spread Order (Combination Order)
*/
void SpreadStrategy::placeSpreadOrder(const SpreadId spreadId, const Order& order, const Contract& contract)
{
    OrderManager* om = Service::service().getOrderManager();
    OrderId orderId = om->requestOrderId();
    _orderIdToSpreadId[orderId] = spreadId;
    updatePendingQuantity(spreadId, order, contract);
}

/*
* Place One-Legged Spread Order (in one instrument of spread)
*/
void SpreadStrategy::placeSpreadOrder(const SpreadId spreadId, const  TickerId tickerId, const Order& order, bool isClosingOrder)
{
    if(!isClosingOrder)
    {
        //Get Permission from Risk-Manager
        if(!canPlaceOrder(tickerId, order))
        {
            return;
        }
    }

    OrderManager* om = Service::service().getOrderManager();
    OrderId orderId = om->requestOrderId();
    _orderIdToSpreadId[orderId] = spreadId;
    _orderIdToTickerId[orderId] = tickerId;
    subscribeMarketData(tickerId);
    om->placeOrder(orderId, tickerId, _strategyId, order);
    Strategy::updatePendingQuantity(tickerId, order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity);
}

/*
 *
 */
void SpreadStrategy::requestStrategyUpdateForExecution(const OrderId orderId,  const OrderDetail& orderDetail)
{
    if(SpreadId spreadId = _orderIdToSpreadId.value(orderId, 0))
    {
        if(orderDetail.isSpreadOrder)
        {
            _spreadManager->updateSpread(orderId, spreadId, orderDetail);
        }
        else if(TickerId tickerId = _orderIdToTickerId.value(orderId, 0))
        {
            _spreadManager->updateSpread(orderId, spreadId, tickerId, orderDetail);
            _positionManager->updatePosition(orderId, tickerId, orderDetail);
        }
    }
}

/*
 * Load spread position for the strategy
 */
void SpreadStrategy::loadSpreads()
{
    //QList<SpreadData> strategySpreadBuyList = IODatabase::ioDatabase().getStrategySpreadList(dbStrategyId);
    //setSpreadBuyList(strategyBuyList);
}

/*
 * Close all spreads for strategy
 */
void SpreadStrategy::closeAllSpreads()
{
    _spreadManager->closeAllSpreads();
}

/*
* Update Position on order execution
*/
void SpreadStrategy::updatePositionOnExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    if(SpreadId spreadId = _orderIdToSpreadId.value(orderId, 0))
    {
        if(orderDetail.isSpreadOrder)
        {
            _spreadManager->updateSpread(orderId, spreadId, orderDetail);
        }
        else if(TickerId tickerId = _orderIdToTickerId.value(orderId, 0))
        {
            _spreadManager->updateSpread(orderId, spreadId, tickerId, orderDetail);
            _positionManager->updatePosition(orderId, tickerId, orderDetail, false);
            Strategy::updatePendingQuantity(tickerId, orderDetail.order.action.compare("BUY") ? -orderDetail.filledShares : orderDetail.filledShares);
        }
    }
}

/*
* Update position of price update
*/
void SpreadStrategy::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double price)
{
    _spreadManager->updateSpread(tickerId, tickType, price);
    _positionManager->updatePosition(tickerId, tickType, price, false);
}

/*
* Load strategy data from database
*/
void SpreadStrategy::loadStrategyDataFromDb()
{
    loadStrategyParameters();
    populateGeneralStrategyPreferences();
    loadSpreadList();
    loadSpreadPositions();
}

/*
* Load spreads from database and create buylist from unique
* instruments
*/
void SpreadStrategy::loadSpreadList()
{
    //here problem in assignment
    _spreadBuyList = IODatabase::ioDatabase().getStrategySpreadList(_strategyName);

    int numSpreads = _spreadBuyList.count();
    QMap<InstrumentId,uint> instruments;
    for(int i = 0; i < numSpreads ; ++i)
    {
        //registerIns
        instruments[_spreadBuyList[i].instrumentId1] = 1;
        instruments[_spreadBuyList[i].instrumentId2] = 1;
    }

    //now we have a consolidated list of instruments for a spread-based strategy
    int numInstruments = instruments.count();
    QList<InstrumentId> l = instruments.keys();
    for(int i = 0 ; i< numInstruments ; ++i)
    {
        _buyList.append(IODatabase::ioDatabase().getInstrument(l[i]));
    }
}

/*
* Load Spread positions from database
*/
void SpreadStrategy::loadSpreadPositions()
{
    QList<SpreadPositionData> spreadPositions = IODatabase::ioDatabase().getOpenStrategyLinkedSpreadPositions(_strategyName);

    foreach(SpreadPositionData spreadPositionData, spreadPositions)
    {
        SpreadData spreadData = IODatabase::ioDatabase().getSpreadData(spreadPositionData.spreadId);
        SpreadId spreadId = Service::service().getInstrumentManager()->registerSpread(spreadData);
        TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(spreadPositionData.instrumentId);

        _spreadManager->addSpreadPosition(spreadId, tickerId, spreadPositionData);
        subscribeMarketData(tickerId);
    }
}

/*
* Register spread with instrument manager
*/
void SpreadStrategy::registerSpreadList()
{
    foreach(SpreadData spreadData, _spreadBuyList)
    {
        Service::service().getInstrumentManager()->registerSpread(spreadData);
    }
}

/*
* Start Spread Strategy
*/
void SpreadStrategy::startStrategy()
{
    populateStrategySpecificPreferences();
    registerBuyList();
    registerSpreadList();
    setupIndicator();
    setTimeout();
    testStrategy();
}

/*
* A test strategy
*/
void SpreadStrategy::testStrategy()
{
    foreach(SpreadData d, _spreadBuyList)
    {
        TickerId t1 = Service::service().getInstrumentManager()->getTickerId(d.instrumentId1);
        TickerId t2 = Service::service().getInstrumentManager()->getTickerId(d.instrumentId2);
        _spreadManager->placeSpreadOrder(t1,t2);
    }
}

void SpreadStrategy::updatePendingQuantity(const SpreadId spreadId, const Order& order, const Contract& contract)
{
    //Strategy::updatePendingQuantity(tickerId, quantity);
}

void SpreadStrategy::closeAll()
{
    closeAllSpreads();
}

void SpreadStrategy::stopStrategy()
{
    _canOpenNewPositions = false;
    stopIndicator();
    closeAllSpreads();
}





