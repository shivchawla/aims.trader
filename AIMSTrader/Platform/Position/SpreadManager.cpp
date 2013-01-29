#include "Platform/Position/SpreadManager.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Strategy/SpreadStrategy.h"
#include "Platform/Strategy/StrategyOutput.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

SpreadPosition::SpreadPosition(Spread* parentSpread):Position()
{
    _parentSpread = parentSpread;
}

SpreadPosition::SpreadPosition(const TickerId tickerId, const StrategyId strategyId, Spread* parentSpread):Position(tickerId, strategyId)
{
    _parentSpread = parentSpread;
}

Spread::Spread(const TickerId firstTickerId, const TickerId secondTickerId, const StrategyId strategyId)
{
    _firstPosition = new SpreadPosition(firstTickerId, strategyId, this);
    _secondPosition = new SpreadPosition(secondTickerId, strategyId, this);
    initialize();
}

Spread::Spread()
{
    //_spreadId = 0;
    _strategyId = 0;
    _firstPosition = new SpreadPosition(this);
    _secondPosition = new SpreadPosition(this);
    initialize();
}

void Spread::initialize()
{}

Spread::~Spread()
{
    delete _firstPosition;
    delete _secondPosition;
}

void Spread::update(const OrderId orderId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    //TickerId tickerId = orderDetail.tickerId;

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

SpreadPosition* Spread::getFirstPosition() const
{
    return _firstPosition;
}

SpreadPosition* Spread::getSecondPosition() const
{
    return _secondPosition;
}

SpreadManager::SpreadManager(SpreadStrategy* strategy): _spreadStrategy(strategy)
{
    //_spreadId = -1;
    _strategyId = strategy->getStrategyId();
    _positionManager = strategy->getPositionManager();
}

void SpreadManager::addSpread(const TickerId tickerId1, const TickerId tickerId2)
{
    SpreadId spreadId = Service::service().getInstrumentManager()->getSpreadId(tickerId1, tickerId2);
    Spread* spread = _spreads.value(spreadId, NULL);
    if(!spread)
    {
         Spread* spread = new Spread(tickerId1, tickerId2, _strategyId);
        _spreads[spreadId] = spread;

        _tickerIdsToListOfSpreadId[tickerId1].append(spreadId);
        _tickerIdsToListOfSpreadId[tickerId2].append(spreadId);
        //_tickerIdsToSpreadId[p] = spreadId;

        //adding spreadPositions to PositionManager
        updateMainPosition(spread->getFirstPosition());
        updateMainPosition(spread->getSecondPosition());
    }
    //return spreadId;
}

void SpreadManager::addSpread(const SpreadId spreadId)
{
    Spread* spread = _spreads.value(spreadId, NULL);
    if(!spread)
    {
        QPair<TickerId, TickerId> tickerIds = Service::service().getInstrumentManager()->getTickerIds(spreadId);
        TickerId tickerId1 = tickerIds.first;
        TickerId tickerId2 = tickerIds.second;

        Spread* spread = new Spread(tickerId1, tickerId2, _strategyId);
        _spreads[spreadId] = spread;

        _tickerIdsToListOfSpreadId[tickerId1].append(spreadId);
        _tickerIdsToListOfSpreadId[tickerId2].append(spreadId);
        //_tickerIdsToSpreadId[p] = spreadId;

        //adding spreadPositions to PositionManager
        updateMainPosition(spread->getFirstPosition());
        updateMainPosition(spread->getSecondPosition());
    }
    //return spreadId;
}


void SpreadManager::updateMainPosition(const Position* subPosition)
{
    _positionManager->addPosition(subPosition);
}

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

        //_spreadStrategy->placeSpreadOrder(tickerId1, tickerId2, order);
    }
}

void SpreadManager::closeAllSpreads()
{
    foreach(SpreadId spreadId, _spreads.keys())
    {
        closeSpread(spreadId);
    }
}

void SpreadManager::updateSpread(const OrderId orderId, const SpreadId spreadId, const OrderDetail& orderDetail)
{
    addSpread(spreadId);
    if(Spread* spread = _spreads.value(spreadId, NULL))
    {
        spread->update(orderId, orderDetail);
        updateOutput(spread);
    }
}

void SpreadManager::updateSpread(const OrderId orderId, const SpreadId spreadId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    addSpread(spreadId);
    if(Spread* spread = _spreads.value(spreadId, NULL))
    {
        spread->update(orderId, tickerId, orderDetail);
        updateOutput(spread);
    }
}

void SpreadManager::updateSpread(const TickerId tickerId, const TickType tickType, const double price)
{
    foreach(SpreadId spreadId,_tickerIdsToListOfSpreadId.value(tickerId, QList<SpreadId>()))
    {
        if(Spread* spread = _spreads.value(spreadId, NULL))
        {
            //once spread is updated...send it to the different outputs (GUI and DB)
            //Also, check the performance of the spread and close it based on conditions
            spread->update(tickerId, tickType, price);
            testExitConditions(spread);
            updateOutput(spread);
        }
    }
}

void SpreadManager::testExitConditions(const Spread* spread)
{
    //1.Return
    //2 Holding Period
    //3 Drawdown
    //4 ....
}

void SpreadManager::updateOutput(const Spread* spread)
{
     StrategyOutput::strategyOutput().updateSpread(spread);
}

void SpreadManager::placeSpreadOrder(const TickerId tickerId1, const TickerId tickerId2)
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
        _spreadStrategy->placeSpreadOrder(spreadId, tickerId1, order1);
        _spreadStrategy->placeSpreadOrder(spreadId, tickerId2, order2);
    }
}

//How to open a spread
//How to close a spread
//When to open a spread

//An example of spread based strategy is pairs trading.
//Pairs Trading will keep a track of spread betwwen two stocks like A- alpha*B = C
//Spreadbased Indicator will let the spreadManager know to open a spread.











