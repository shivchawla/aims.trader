#include "Platform/Strategy/SpreadStrategy.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Position/SpreadManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/View/IODatabase.h"

SpreadStrategy::SpreadStrategy(): StrategyImpl<SpreadStrategy>()
{
    _spreadManager = new SpreadManager(this);
    _strategyType = Spread_StrategyType;
}

SpreadStrategy::SpreadStrategy(const String& string): StrategyImpl<SpreadStrategy>(string)
{
    _spreadManager = new SpreadManager(this);
}

//void SpreadStrategy::placeSpreadOrder(const TickerId tickerId1, const TickerId tickerId2, const Order& order)
//{
//   OrderId orderId1 = Service::service().getOrderManager()->placeOrder(tickerId1, _strategyId, order);

//   order.action = "SELL";
//   OrderId orderId2 = Service::service().getOrderManager()->placeOrder(tickerId2, _strategyId, order);

//    subscribeMarketData(tickerId1);
//    subscribeMarketData(tickerId2);
//}

void SpreadStrategy::placeSpreadOrder(const SpreadId spreadId, const Order& order, const Contract& contract)
{
    OrderManager* om = Service::service().getOrderManager();
    OrderId orderId = om->requestOrderId();
    _orderIdToSpreadId[orderId] = spreadId;
    //Service::service().getOrderManager()->placeOrder(spreadId, _strategyId, order);
}

void SpreadStrategy::placeSpreadOrder(const SpreadId spreadId, const  TickerId tickerId, const Order& order)
{
    //Get Permission from Risk-Manager
    if(!canPlaceOrder(tickerId, order))
    {
        return;
    }

    OrderManager* om = Service::service().getOrderManager();
    OrderId orderId = om->requestOrderId();
    _orderIdToSpreadId[orderId] = spreadId;
    _orderIdToTickerId[orderId] = tickerId;
    subscribeMarketData(tickerId);
    om->placeOrder(orderId, tickerId, _strategyId, order);
}

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

void SpreadStrategy::loadSpreads(const DbStrategyId dbStrategyId)
{
    //QList<SpreadData> strategySpreadBuyList = IODatabase::ioDatabase().getStrategySpreadList(dbStrategyId);
    //setSpreadBuyList(strategyBuyList);
}

void SpreadStrategy::closeAllSpreads()
{
    _spreadManager->closeAllSpreads();
}

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
        }
    }
}

void SpreadStrategy::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double price)
{
    _spreadManager->updateSpread(tickerId, tickType, price);
    _positionManager->updatePosition(tickerId, tickType, price, false);
}

void SpreadStrategy::loadStrategyDataFromDb(const StrategyData& strategyData)
{
    setName(strategyData.name);
    DbStrategyId id = strategyData.strategyId;
    _strategyParams = IODatabase::ioDatabase().getStrategyConfigurations(id);
    populateGeneralStrategyPreferences();
    loadSpreadList(id);
    loadSpreadPositions(id);
}

void SpreadStrategy::loadSpreadList(const DbStrategyId strategyId)
{
    //here problem in assignment
    _spreadBuyList = IODatabase::ioDatabase().getStrategySpreadList(strategyId);

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

void SpreadStrategy::loadSpreadPositions(const DbStrategyId strategyId)
{

}

void SpreadStrategy::registerSpreadList()
{
    foreach(SpreadData spreadData, _spreadBuyList)
    {
        Service::service().getInstrumentManager()->registerSpread(spreadData);
    }
}

void SpreadStrategy::startStrategy()
{
    populateStrategySpecificPreferences();
    registerBuyList();
    registerSpreadList();
    setupIndicator();
    testStrategy();
    //setTimeout();
}

void SpreadStrategy::testStrategy()
{
    foreach(SpreadData d, _spreadBuyList)
    {
        TickerId t1 = Service::service().getInstrumentManager()->getTickerId(d.instrumentId1);
        TickerId t2 = Service::service().getInstrumentManager()->getTickerId(d.instrumentId2);
        _spreadManager->placeSpreadOrder(t1,t2);
    }
}






