#include "Platform/Position/SpreadManager.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Strategy/SpreadStrategy.h"
#include "Platform/Strategy/StrategyOutput.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

/*
 * SpreadPosition Constructor
 */
SpreadPosition::SpreadPosition(Spread* parentSpread):Position()
{
    _parentSpread = parentSpread;
}

/*
 * SpreadPosition Constructor
 */
SpreadPosition::SpreadPosition(const TickerId tickerId, const StrategyId strategyId, Spread* parentSpread):Position(tickerId, strategyId)
{
    _parentSpread = parentSpread;
}

/*
 * Spread constructor
 */
Spread::Spread(const TickerId firstTickerId, const TickerId secondTickerId, const StrategyId strategyId)
{
    _strategyId = strategyId;
    _firstPosition = new SpreadPosition(firstTickerId, strategyId, this);
    _secondPosition = new SpreadPosition(secondTickerId, strategyId, this);
    initialize();
}

Spread::Spread(const SpreadId spreadId,const TickerId tickerId1, const TickerId tickerId2, const StrategyId strategyId)
{
    _spreadId = spreadId;
    _strategyId = strategyId;
    _firstPosition = new SpreadPosition(tickerId1, strategyId, this);
    _secondPosition = new SpreadPosition(tickerId2, strategyId, this);
    initialize();
}

/*
 * Spread constructor
 */
Spread::Spread()
{
    //_spreadId = 0;
    _strategyId = 0;
    _firstPosition = new SpreadPosition(this);
    _secondPosition = new SpreadPosition(this);
    initialize();
}

/*
 *Initialize spread
 */
void Spread::initialize()
{}

/*
 *
 */
Spread::~Spread()
{
    delete _firstPosition;
    delete _secondPosition;
}

/*
 * Update spread for execution
 */
void Spread::update(const OrderId orderId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    if(_firstPosition->getTickerId() == tickerId)
    {
        _firstPosition->update(orderId, orderDetail);
    }
    else if(_secondPosition->getTickerId() == tickerId)
    {
        _secondPosition->update(orderId, orderDetail);
    }

    _spreadDetail.netValue = _firstPosition->getNetValue() + _secondPosition->getNetValue();
    _spreadDetail.totalValueBought = _firstPosition->getTotalValueBought() + _secondPosition->getTotalValueBought();
    _spreadDetail.totalValueSold = _firstPosition->getTotalValueSold() + _secondPosition->getTotalValueSold();
    _spreadDetail.totalCommission = _firstPosition->getTotalCommission() + _secondPosition->getTotalCommission();
    _spreadDetail.realizedProfit = _firstPosition->getRealizedProfit() + _secondPosition->getRealizedProfit();
    _spreadDetail.realizedLoss = _firstPosition->getRealizedLoss() + _secondPosition->getRealizedLoss();
    _spreadDetail.runningProfit = _firstPosition->getRunningProfit() + _secondPosition->getRunningProfit() ;
    _spreadDetail.runningLoss = _firstPosition->getRunningLoss() + _secondPosition->getRunningLoss();
    _spreadDetail.peakValue = _spreadDetail.netValue > _spreadDetail.peakValue ?  _spreadDetail.netValue : _spreadDetail.peakValue;
    _spreadDetail.drawDown = _spreadDetail.netValue >= _spreadDetail.peakValue
                                ? 0 : 100 * (_spreadDetail.peakValue - _spreadDetail.netValue)/_spreadDetail.peakValue;
}

/*
 * Update spread for combo-order execution
 */
void Spread::update(const OrderId orderId, const OrderDetail& orderDetail)
{
    //TickerId tickerId = orderDetail.tickerId;

//    if(_firstPosition->getTickerId() == tickerId)
//    {
//        _firstPosition->update(orderId, orderDetail);
//    }
//    else if(_secondPosition->getTickerId() == tickerId)
//    {
//        _secondPosition->update(orderId, orderDetail);
//    }

//    _spreadDetail.netValue = _firstPosition->getNetValue() + _secondPosition->getNetValue();
//    _spreadDetail.totalValueBought = _firstPosition->getTotalValueBought() + _secondPosition->getTotalValueBought();
//    _spreadDetail.totalValueSold = _firstPosition->getTotalValueSold() + _secondPosition->getTotalValueSold();
//    _spreadDetail.totalCommission = _firstPosition->getTotalCommission() + _secondPosition->getTotalCommission();
//    _spreadDetail.realizedProfit = _firstPosition->getRealizedProfit() + _secondPosition->getRealizedProfit();
//    _spreadDetail.realizedLoss = _firstPosition->getRealizedLoss() + _secondPosition->getRealizedLoss();
//    _spreadDetail.runningProfit = _firstPosition->getRunningProfit() + _secondPosition->getRunningProfit() ;
//    _spreadDetail.runningLoss = _firstPosition->getRunningLoss() + _secondPosition->getRunningLoss();
//    _spreadDetail.peakValue = _spreadDetail.netValue > _spreadDetail.peakValue ?  _spreadDetail.netValue : _spreadDetail.peakValue;
//    _spreadDetail.drawDown = _spreadDetail.netValue >= _spreadDetail.peakValue
//                                ? 0 : 100 * (_spreadDetail.peakValue - _spreadDetail.netValue)/_spreadDetail.peakValue;
}

/*
 * Update spread for price update
 */
void Spread::update(const TickerId tickerId, const TickType type, const double price)
{
    if(_firstPosition->getTickerId() == tickerId)
    {
        _firstPosition->update(type, price);
    }
    else if(_secondPosition->getTickerId() == tickerId)
    {
        _secondPosition->update(type, price);
    }

    double oldPeakValue = _spreadDetail.peakValue;
    double newNetValue = _spreadDetail.netValue = _firstPosition->getNetValue() + _secondPosition->getNetValue();
    _spreadDetail.runningProfit = _firstPosition->getRunningProfit() + _secondPosition->getRunningProfit() ;
    _spreadDetail.runningLoss = _firstPosition->getRunningLoss() + _secondPosition->getRunningLoss();
    double newPeakValue = _spreadDetail.peakValue = newNetValue> oldPeakValue ?  newNetValue : oldPeakValue;
    _spreadDetail.drawDown = newNetValue >= newPeakValue
                                ? 0 : 100 * (oldPeakValue - newNetValue)/oldPeakValue;
}

/*
 * Get first spread-position
 */
SpreadPosition* Spread::getFirstPosition() const
{
    return _firstPosition;
}

/*
 * Get second spread-position
 */
SpreadPosition* Spread::getSecondPosition() const
{
    return _secondPosition;
}

/*
 * SpreadManager Constructor
 */
SpreadManager::SpreadManager(SpreadStrategy* strategy): _spreadStrategy(strategy)
{
    //_spreadId = -1;
    _strategyId = strategy->getStrategyId();
    _positionManager = strategy->getPositionManager();
}

/*
 * Add Spread for two tickerIds
 */
void SpreadManager::addSpread(const TickerId tickerId1, const TickerId tickerId2)
{
    SpreadId spreadId = Service::service().getInstrumentManager()->getSpreadId(tickerId1, tickerId2);
    Spread* spread = _spreads.value(spreadId, NULL);
    if(!spread)
    {
         Spread* spread = new Spread(spreadId, tickerId1, tickerId2, _strategyId);
        _spreads[spreadId] = spread;

        _tickerIdsToListOfSpreadId[tickerId1].append(spreadId);
        _tickerIdsToListOfSpreadId[tickerId2].append(spreadId);
        //_tickerIdsToSpreadId[p] = spreadId;

        //adding spreadPositions to PositionManager
        updateMainPosition(spread->getFirstPosition());
        updateMainPosition(spread->getSecondPosition());
    }
}

/*
 * Add Spread for a spreadId
 */
void SpreadManager::addSpread(const SpreadId spreadId)
{
    Spread* spread = _spreads.value(spreadId, NULL);
    if(!spread)
    {
        QPair<TickerId, TickerId> tickerIds = Service::service().getInstrumentManager()->getTickerIds(spreadId);
        TickerId tickerId1 = tickerIds.first;
        TickerId tickerId2 = tickerIds.second;

        Spread* spread = new Spread(spreadId, tickerId1, tickerId2, _strategyId);
        _spreads[spreadId] = spread;

        _tickerIdsToListOfSpreadId[tickerId1].append(spreadId);
        _tickerIdsToListOfSpreadId[tickerId2].append(spreadId);
        //_tickerIdsToSpreadId[p] = spreadId;

        //adding spreadPositions to PositionManager
        updateMainPosition(spread->getFirstPosition());
        updateMainPosition(spread->getSecondPosition());
    }
}

/*
 * Add Spread
 */
void SpreadManager::addSpread(const SpreadId spreadId, const TickerId tickerId1, const TickerId tickerId2)
{
    Spread* spread = _spreads.value(spreadId, NULL);
    if(!spread)
    {
        Spread* spread = new Spread(spreadId, tickerId1, tickerId2, _strategyId);
        _spreads[spreadId] = spread;

        _tickerIdsToListOfSpreadId[tickerId1].append(spreadId);
        _tickerIdsToListOfSpreadId[tickerId2].append(spreadId);
        //_tickerIdsToSpreadId[p] = spreadId;

        //adding spreadPositions to PositionManager
        updateMainPosition(spread->getFirstPosition());
        updateMainPosition(spread->getSecondPosition());
    }
}

/*
 * Update postion manager for child positions
 */
void SpreadManager::updateMainPosition(const Position* subPosition)
{
    _positionManager->addSubPosition(subPosition);
}

/*
 * Close Spread
 */
void SpreadManager::closeSpread(const SpreadId spreadId)
{
    //find the tickerIds for thsi SpreadId
    if(Spread* spread  = _spreads.value(spreadId, NULL))
    {
        Position* pos1 = spread->getFirstPosition();
        Position* pos2 = spread->getSecondPosition();

        //Place Combo Order
        Order order;
        order.orderType = "MKT";

        TickerId tickerId1 = pos1->getTickerId();
        int quantity = pos1->getNetShares();

        order.totalQuantity = abs(quantity);

        if(quantity>0)
        {
            order.action = "SELL";
        }
        else
        {
            order.action = "BUY";
        }

        bool isClosingOrder = true;
        _spreadStrategy->placeSpreadOrder(spreadId, tickerId1, order, isClosingOrder);

        TickerId tickerId2 = pos2->getTickerId();
        quantity = pos2->getNetShares();

        order.totalQuantity = abs(quantity);

        if(quantity>0)
        {
            order.action = "SELL";
        }
        else
        {
            order.action = "BUY";
        }

        _spreadStrategy->placeSpreadOrder(spreadId, tickerId2, order, isClosingOrder);

    }
}

/*
 * Close all spreads
 */
void SpreadManager::closeAllSpreads()
{
    foreach(SpreadId spreadId, _spreads.keys())
    {
        closeSpread(spreadId);
    }
}

/*
 * Update spread for execution (Combo-Order Execution)
 */
void SpreadManager::updateSpread(const OrderId orderId, const SpreadId spreadId, const OrderDetail& orderDetail)
{
    //this is inefficient
    //two calls to hash table
    addSpread(spreadId);
    if(Spread* spread = _spreads.value(spreadId, NULL))
    {
        spread->update(orderId, orderDetail);
        updateOutputForExecution(spread);
    }
}

/*
 * Update Spread for single leg execution
 */
void SpreadManager::updateSpread(const OrderId orderId, const SpreadId spreadId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    addSpread(spreadId);
    if(Spread* spread = _spreads.value(spreadId, NULL))
    {
        spread->update(orderId, tickerId, orderDetail);
        updateOutputForExecution(spread);
    }
}

/*
 * Update spread for price update
 */
void SpreadManager::updateSpread(const TickerId tickerId, const TickType tickType, const double price)
{
    foreach(SpreadId spreadId, _tickerIdsToListOfSpreadId.value(tickerId, QList<SpreadId>()))
    {
        if(Spread* spread = _spreads.value(spreadId, NULL))
        {
            //once spread is updated...send it to the different outputs (GUI and DB)
            //Also, check the performance of the spread and close it based on conditions
            spread->update(tickerId, tickType, price);
            testExitConditions(spread);
            updateOutputForPrice(spread);
        }
    }
}

/*
 * Test Exit conditions for a spread
 */
void SpreadManager::testExitConditions(const Spread* spread)
{
    //1.Return
    //2 Holding Period
    //3 Drawdown
    //4 ....
}

/*
 * Update outputs for new found information
 */
void SpreadManager::updateOutputForPrice(const Spread* spread)
{
     StrategyOutput::strategyOutput().updateSpread(spread, GUI);
}

void SpreadManager::updateOutputForExecution(const Spread* spread)
{
     StrategyOutput::strategyOutput().updateSpread(spread);
}


/*
 * Place Spread Order (Two single-legged orders)
 */
void SpreadManager::placeSpreadOrder(const TickerId tickerId1, const TickerId tickerId2, const double multiplier)
{
    Order order1, order2;
    order1.orderType = "MKT";
    order1.totalQuantity = 10;
    order1.action = "BUY";

    order2.orderType = "MKT";
    order2.totalQuantity = 10;
    order2.action = "SELL";

    if(_spreadStrategy->canPlaceOrder(tickerId1, order1) && _spreadStrategy->canPlaceOrder(tickerId2, order2))
    {
        SpreadId spreadId = Service::service().getInstrumentManager()->getSpreadId(tickerId1, tickerId2);
        addSpread(spreadId, tickerId1, tickerId2);
        _spreadStrategy->placeSpreadOrder(spreadId, tickerId1, order1);
        _spreadStrategy->placeSpreadOrder(spreadId, tickerId2, order2);
    }
}

/*
 * Place Spread Order (Two single-legged orders)
 */
//void SpreadManager::placeSpreadOrder(const SpreadId spreadId)
//{
//    Contract contract;
//    contract.comboLegs
//    Order order1;
//    order1.orderType = "MKT";
//    order1.totalQuantity = 10;
//    order1.action = "BUY";

//    order2.orderType = "MKT";
//    order2.totalQuantity = 10;
//    order2.action = "SELL";

//    if(_spreadStrategy->canPlaceOrder(tickerId1, order1) && _spreadStrategy->canPlaceOrder(tickerId2, order2))
//    {
//        SpreadId spreadId = Service::service().getInstrumentManager()->getSpreadId(tickerId1, tickerId2);
//        addSpread(spreadId, tickerId1, tickerId2);
//        _spreadStrategy->placeSpreadOrder(spreadId, tickerId1, order1);
//        _spreadStrategy->placeSpreadOrder(spreadId, tickerId2, order2);
//    }
//}

//How to open a spread
//How to close a spread
//When to open a spread

//An example of spread based strategy is pairs trading.
//Pairs Trading will keep a track of spread betwwen two stocks like A- alpha*B = C
//Spreadbased Indicator will let the spreadManager know to open a spread.











