#pragma once
#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include<QObject>
#include "Platform/Position/Position.h"
#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/Singleton.h"
#include <QSize>

class EventReport;
class MainWindow;
class InstrumentView;
class DatabaseSession;

class OutputInterface : public QObject, public Singleton<OutputInterface>
{
    Q_OBJECT
    private:
        EventReport* _eventReportSPtr;
        MainWindow* _guiWindow;
        //DatabaseSession* _dbSession;

    public:
        OutputInterface();
        ~OutputInterface();

    private:
        void init();
        void setEventReporter();
        void setupConnections();

    public:
        void setupMainwindow(const QMap<QString, QSize> &customSizeHints);

    public:
        void addPosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const Position*);
        void updatePositionForLastPrice(const Position*);

        void updateOrderExecution(const OpenOrder*);
        void addOrder(const OpenOrder*, const String&);
        void removeOrder(const OrderId);
        void updateOrderStatus(const OpenOrder*);

        void updateStrategy(const StrategyId, const PerformanceStats&);
        void reportEvent(const String& reporter, const String& report, const MessageType type = INFO);
        void addInstrument(const TickerId, const Contract&);


    public:
        InstrumentView* getInstrumentView();
//        DatabaseSession* getDatabaseSession();

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
        void eventReported(const String, const String, const String, const MessageType);
        void instrumentAdded(const TickerId, const Contract&);
};

#endif // OUTPUTINTERFACE_H
