#pragma once
#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QHash>
#include <QPair>
#include "AimsTraderDefs/typedefs.h"
#include <vector>
#include <QObject>
#include "Platform/Position/Position.h"

typedef std::vector<int> InstrumentStrategyVector;

//startegyInstrumentId 1(a1) 2(a2) 3(a3) 4(a4) 5(a5) 6(b1) 7(b2) 8(b3) 9(b4) 10(b5)

//strategy a b
//instrument 1 2 3 4 5


class StrategyInstrumentData
{
    private:
        long _id;
        StrategyId _strategyId;
        TickerId _tickerId;
        long _sharesBought;
        long _sharesSold;
        long _netShares;
        double _avgBought;
        double _avgSold;
        double _totalValueBought;
        double _totalValueSold;
        double _netTotalValue;
        double _totalCommision;
        double _realizedPnl;
        double _runningPnl;
        double _PnL;
        double _netInclCommission;

    public:
        StrategyInstrumentData();
        ~StrategyInstrumentData();

    public:
        const long getStrategyInstrumentId() const{return _id;}
        const StrategyId getStrategyId() const{return _strategyId;}
        const TickerId getTickerId() const{return _tickerId;}
        const long getSharesBought() const{return _sharesBought;}
        const long getSharesSold() const{return _sharesSold;}
        const long getNetShares() const{return (_sharesBought-_sharesSold);}
        const double getAvgBought() const {return _avgBought; }
        const double getAvgSold() const{return _avgSold;}
        const double getTotalValueBought() const{return _totalValueBought;}
        const double getTotalValueSold() const{return _totalValueSold;}
        const double getNetTotal() const{return (_totalValueBought-_totalValueSold);}
        const double getTotalCommision() const{return _totalCommision;}
        const double getRealizedPnl() const{return _realizedPnl;}
        const double getRunningPnl() const{return _runningPnl;}
        const double getPnL() const{return _PnL;}
        const double getNetTotalIncCommission() const{return _netInclCommission;}

    public:
        void update(const double lastPrice);
        void update(const long quantity, const double avgFillPrice, const double commission);

     public:
        void setId(const long id){_id=id;}
        void setStrategyId(const StrategyId strategyId){_strategyId=strategyId;}
        void setTickerId(const TickerId tickerId){_tickerId=tickerId;}
        void setSharesBought(const long sharesBought){_sharesBought = sharesBought;}
        void setSharesSold(const long sharesSold){_sharesSold = sharesSold;}
        void setNetShares(const long netShares){_netShares = netShares;}
        void setAvgBought(const double avgBought){_avgBought = avgBought;}
        void setAvgSold(const double avgSold){_avgSold = avgSold;}
        void setTotalValueBought(const double totalValueBought){_totalValueBought = totalValueBought;}
        void setTotalValueSold(const double totalValueSold){_totalValueSold = totalValueSold;}
        void setTotalCommision(const double totalCommision){_totalCommision = totalCommision;}
        void setRealizedPnl(const double realizedPnl){_realizedPnl = realizedPnl;}
        void setRunningPnl(const double runningPnl){_runningPnl=runningPnl;}
        void setPnl(const double pnl){_PnL=pnl;}
};

typedef std::vector<StrategyInstrumentData*> StrategyInsrumentDataVector;
typedef QHash<StrategyId, QHash<TickerId, StrategyInstrumentData*> > StrategyInstrumentMap;
typedef QHash<QPair<StrategyId, TickerId>, int> StrategyInstrumentIdToPairId;

class StrategyInstrumentContainer
{
    private:
        StrategyInsrumentDataVector _strategyInstrumentDataVector;
        QHash<TickerId, InstrumentStrategyVector> _instrumentStrategiesForInstrumentId;
        QHash<StrategyId, InstrumentStrategyVector> _instrumentStrategiesForStrategyId;
        StrategyInstrumentIdToPairId _strategyInstrumentIdToPairId;

    public:
        StrategyInstrumentContainer();
        ~StrategyInstrumentContainer();

    public:
        void addDiagnostic(const StrategyId, const TickerId);
        void updateDiagnostic(const Position&);
        void updateDiagnostic(const StrategyId, const TickerId, const double, const double);
        //void updatePosition(const StrategyId, const TickerId, const double lastprice);
        //void updatePosition(const TickerId, const double lastprice);
        void removeDiagnostic(const StrategyId, const TickerId);
        //void updatePosition(const StrategyId, const TickerId, const ExecutionStatus& status);// const int quantity, const double avgFillPrice, const double commission);
        const StrategyInstrumentData* getStrategyInstrumentData(const StrategyId, const TickerId);

};

#endif // DATASTRUCTURES_H
