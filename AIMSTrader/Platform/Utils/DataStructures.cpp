#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/Position.h"

StrategyInstrumentContainer::StrategyInstrumentContainer(){}
StrategyInstrumentContainer::~StrategyInstrumentContainer()
{
    StrategyInsrumentDataVector::iterator iter;
    StrategyInsrumentDataVector::iterator end = _strategyInstrumentDataVector.end();
    for(iter = _strategyInstrumentDataVector.begin(); iter!=end; ++iter)
    {
        StrategyInstrumentData* data = *iter;
        delete data;
    }
}

void StrategyInstrumentContainer::addDiagnostic(const StrategyId strategyId, const TickerId tickerId)
{
    //form a par;
    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(!_strategyInstrumentIdToPairId.contains(pair))
    {
        StrategyInstrumentData* data = new StrategyInstrumentData();
        _strategyInstrumentDataVector.push_back(data);

        int pos = _strategyInstrumentDataVector.size()-1;
        _strategyInstrumentIdToPairId[pair] = pos;

        if(!_instrumentStrategiesForInstrumentId.contains(tickerId))
        {
            _instrumentStrategiesForInstrumentId.insert(tickerId, InstrumentStrategyVector());
        }

        if(!_instrumentStrategiesForStrategyId.contains(strategyId))
        {
            _instrumentStrategiesForStrategyId.insert(strategyId, InstrumentStrategyVector());
        }

        _instrumentStrategiesForInstrumentId[tickerId].push_back(pos);
        _instrumentStrategiesForStrategyId[strategyId].push_back(pos);
    }
}


const StrategyInstrumentData* StrategyInstrumentContainer::getStrategyInstrumentData(const StrategyId strategyId, const TickerId tickerId)
{
    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(_strategyInstrumentIdToPairId.contains(pair))
    {
        return _strategyInstrumentDataVector[_strategyInstrumentIdToPairId[pair]];
    }
    return NULL;
}

/*void StrategyInstrumentContainer::updatePosition(const TickerId tickerId, const double lastPrice)
{}*/

void StrategyInstrumentContainer::updateDiagnostic(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(_strategyInstrumentIdToPairId.contains(pair))
    {
        long pos = _strategyInstrumentIdToPairId[pair];
        StrategyInstrumentData* data = _strategyInstrumentDataVector[pos];
        data->setRunningPnl(runningPnl);
        data->setPnl(pnl);
     }
}

void StrategyInstrumentContainer::updateDiagnostic(const Position& position)
{
    StrategyId strategyId = position.getStrategyId();
    TickerId tickerId = position.getTickerId();

    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(_strategyInstrumentIdToPairId.contains(pair))
    {
        long pos = _strategyInstrumentIdToPairId[pair];
        StrategyInstrumentData* data = _strategyInstrumentDataVector[pos];
        data->setAvgBought(position.getAvgBought());
        data->setAvgSold(position.getAvgSold());
        data->setNetShares(position.getNetShares());
        data->setSharesBought(position.getSharesBought());
        data->setSharesSold(position.getSharesSold());
        data->setRealizedPnl(position.getRealizedPnl());
        data->setRunningPnl(position.getRunningPnl());
        //data->setPnl(position.getPnL());
        data->setTotalCommision(position.getTotalCommission());
        data->setTotalValueSold(position.getTotalValueSold());
        data->setTotalValueBought(position.getTotalValueBought());
     }
}

/*void StrategyInstrumentContainer::updatePosition(const StrategyId strategyId, const TickerId tickerId, const double lastPrice)
{
    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(_strategyInstrumentIdToPairId.contains(pair))
    {
        long pos = _strategyInstrumentIdToPairId[pair];
        StrategyInstrumentData* data = _strategyInstrumentDataVector[pos];
        data->update(lastPrice);
     }
}*/

/*void StrategyInstrumentContainer::updatePosition(const StrategyId strategyId, const TickerId tickerId, const ExecutionStatus& status)
{
    int quantity = status.execution.shares;
    double avgFillPrice  = status.execution.avgPrice;
    double commission = 0;
    QPair<StrategyId, TickerId> pair(strategyId, tickerId);
    if(_strategyInstrumentIdToPairId.contains(pair))
    {
        long pos = _strategyInstrumentIdToPairId[pair];
        StrategyInstrumentData* data = _strategyInstrumentDataVector[pos];
        data->update(quantity, avgFillPrice, commission);
     }
}*/

StrategyInstrumentData::StrategyInstrumentData()
{
    _strategyId = 0;
    _tickerId = 0;
    _sharesBought = 0;
    _sharesSold = 0;
    _netShares = 0;
    _avgBought = 0;
    _avgSold = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommision = 0;
    _PnL=0;
    _realizedPnl = 0;
    _runningPnl = 0;
}

StrategyInstrumentData::~StrategyInstrumentData()
{}

void StrategyInstrumentData::update(const double lastPrice)
{
    _totalValueBought = _sharesBought*lastPrice;
    _totalValueSold = _sharesSold*lastPrice;

    if(_netShares>0)
    {
        _runningPnl = _netShares * (lastPrice - _avgBought);
    }
    else
    {
        _runningPnl = _netShares * (_avgSold -lastPrice);
    }
    //runningPnl = (totalValueBought - avgBought*sharesBought) + (avgSold*sharesSold - totalValueSold) - totalCommision;
}


void StrategyInstrumentData::update(const long quantity, const double avgFillPrice, const double commission)
{
    if(quantity>0)
    {
       _avgBought = (quantity*avgFillPrice +_sharesBought*_avgBought)/(_sharesBought += quantity);
       //sharesBought += quantity;
    }
    else
    {
        _avgSold = ((-quantity)*avgFillPrice + _sharesSold*_avgSold)/(_sharesSold += (-quantity));
    }

    _netShares = _sharesBought - _sharesSold;
    _totalCommision += commission;
}
