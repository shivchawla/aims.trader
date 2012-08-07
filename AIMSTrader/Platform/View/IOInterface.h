#pragma once
#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include<QObject>
#include "Platform/Position/Position.h"
#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/Singleton.h"
#include <QSize>

class StrategyData;
class EventReport;
class MainWindow;
class InstrumentView;
class DatabaseSession;
class IODatabase;
class StrategyData;

class IOInterface : public QObject//, public Singleton<IOInterface>
{
    Q_OBJECT
//    private:
//        EventReport* _eventReportSPtr;
//        MainWindow* _guiWindow;
//        IODatabase* _ioDatabase;

    public:
        IOInterface();
        //~IOInterface();

    private:
        void init();
        void setEventReporter();
        void setupConnections();
        static IOInterface* _instance;

    public:
        void setupMainwindow(const QMap<QString, QSize> &customSizeHints);
        static IOInterface& ioInterface()
        {
            if(_instance == NULL)
            {
                _instance = new IOInterface();
            }
            return *_instance;
            //static IOInterface x;
            //return x;
        }

    public:
        void addPosition(const StrategyId, const InstrumentId, const OutputType type);
        void updatePositionForExecution(const Position*, const OutputType type);
        void updatePositionForLastPrice(const Position*, const OutputType type);

        void updateOrderExecution(const OpenOrder*, const OutputType type);
        void addOrder(const OpenOrder*, const String&, const OutputType type);
        void removeOrder(const OrderId, const OutputType type);
        void updateOrderStatus(const OpenOrder*, const OutputType type);

        void updatePerformance(const StrategyId, const PerformanceStats&, const OutputType type);
        void reportEvent(const String& reporter, const String& report, const MessageType type = INFO);
        void addInstrument(const InstrumentId, const InstrumentContract&, const OutputType type);

    public:
        InstrumentView* getInstrumentView();

    signals:
        void positionCreatedGUI(const StrategyId, const InstrumentId);
        void positionCreatedDB(const StrategyId, const InstrumentId);

        //void positionRemoved(const StrategyId, const PositionId);
        //void positionUpdatedForExecution(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void positionUpdatedForExecutionGUI(const Position&);
        void positionUpdatedForExecutionDB(const Position&);

        void positionUpdatedForLastPrice(const StrategyId, const InstrumentId, const double runningPnl, const double pnl);

        void orderPlacedGUI(const OpenOrder&, const QString&);
        void orderPlacedDB(const OpenOrder&, const QString&);

        void orderDeleted(const OrderId);

        void orderUpdatedGUI(const OrderId, const long, const long, const double, const double);
        void orderUpdatedDB(const OrderId, const long, const long, const double, const double);

        void orderUpdatedGUI(const OpenOrder&);
        void orderUpdatedDB(const OpenOrder&);

        void orderStatusUpdatedGUI(const OrderId, const OrderStatus);
        void orderStatusUpdatedDB(const OrderId, const OrderStatus);

        void strategyUpdatedGUI(const StrategyId, const PerformanceStats&);
        void strategyUpdatedDB(const StrategyId, const PerformanceStats&);

        void eventReported(const String, const String, const String, const MessageType);

        void instrumentAdded(const InstrumentId, const InstrumentContract&);
};

//static IOInterface& ioInterface()
//{
//    static IOInterface x;
//    return x;
//}


#endif // OUTPUTINTERFACE_H
