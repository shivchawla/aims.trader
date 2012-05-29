#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include<QObject>
#include "Platform/Position/Position.h"
#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/OpenOrder.h"

class OutputInterface :public QObject
{
    Q_OBJECT
    private:
        //StrategyInstrumentContainer _strategyInstrumentDiagnostics;
        //OrderContainer _orderDiagnostics;

    public:
        OutputInterface();
        ~OutputInterface();

    public:
        void addPosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const Position*);
        void updatePositionForLastPrice(const StrategyId, const TickerId, const Position*);

        void updateOrderExecution(const OpenOrder*);
        void addOrder(const OpenOrder*);
        void removeOrder(const OrderId);
        void updateOrderStatus(const OpenOrder*);


       // void updatePosition(const TickerId, const double lastprice);

//    public slots:
//        void addPosition(const StrategyId, const TickerId);
//        void removePosition(const StrategyId, const TickerId);
//        void onExecutionUpdate(const Position&);
//        void onLastPriceUpdate(const StrategyId, const TickerId, const double, const double);

        //void updatePosition(const StrategyId, const TickerId, const ExecutionStatus&);
        //void updatePosition(const StrategyId, const TickerId, const double lastprice);
        //void onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double value);

    signals:
        void positionCreated(const StrategyId, const TickerId);
        //void positionRemoved(const StrategyId, const PositionId);
        void positionUpdatedForExecution(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void positionUpdatedForLastPrice(const StrategyId, const TickerId, const double runningPnl, const double pnl);

        void orderPlaced(const OrderId, const Order&, const Contract&, const String&);
        void orderDeleted(const OrderId);
        void orderUpdated(const OrderId, const long, const long, const double, const double);
        void orderStatusUpdated(const OrderId, const String);
};

#endif // OUTPUTINTERFACE_H
