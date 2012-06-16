#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include<QObject>
#include "Platform/Position/Position.h"
#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/Singleton.h"

class EventReport;
class OutputInterface : public QObject, public Singleton<OutputInterface>
{
    Q_OBJECT
    private:
        EventReport* _eventReportSPtr;
        //StrategyInstrumentContainer _strategyInstrumentDiagnostics;
        //OrderContainer _orderDiagnostics;

    public:
        OutputInterface();
        ~OutputInterface();

    private:
        void setEventReporter();

    public:
        void addPosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const Position*);
        void updatePositionForLastPrice(const Position*);

        void updateOrderExecution(const OpenOrder*);
        void addOrder(const OpenOrder*);
        void removeOrder(const OrderId);
        void updateOrderStatus(const OpenOrder*);

        void updateStrategy(const StrategyId, const PerformanceStats&);
        void reportEvent(const String& reporter, const String& report, const int type = 0);

    signals:
        void positionCreated(const StrategyId, const TickerId);
        //void positionRemoved(const StrategyId, const PositionId);
        void positionUpdatedForExecution(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void positionUpdatedForLastPrice(const StrategyId, const TickerId, const double runningPnl, const double pnl);

        void orderPlaced(const OrderId, const Order&, const Contract&, const String&);
        void orderDeleted(const OrderId);
        void orderUpdated(const OrderId, const long, const long, const double, const double);
        //void orderStatusUpdated(const OrderId, const String);
        void orderStatusUpdated(const OrderId, const OrderStatus);
        void strategyUpdated(const StrategyId, const PerformanceStats&);
        void eventReported(const String, const String, const String, const int);
};

#endif // OUTPUTINTERFACE_H
