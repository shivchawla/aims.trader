#pragma once
#ifndef OUTPUTINTERFACE_H
#define OUTPUTINTERFACE_H

#include<QObject>
#include "Platform/Position/Position.h"
#include "Platform/Utils/DataStructures.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/Singleton.h"
#include <QSize>
#include "Utils/Log.h"

class StrategyData;
class EventReport;
class MainWindow;
class InstrumentView;
class InstrumentViewWidget;
class DatabaseSession;
class IODatabase;
class StrategyData;
class Spread;
class StrategyPositionView;
class OpenOrderWidget;
class StrategyView;
class MessageView;
class TradeView;

//#include "Platform/View/StrategyPositionModel2.h"

class IOInterface
{
    private:
        IODatabase* _ioDatabase;
        StrategyPositionView* _strategyPositionView;
        OpenOrderWidget* _openOrderView;
        StrategyView* _strategyView;
        MessageView* _messageView;
        InstrumentViewWidget* _instrumentViewWidget;
        TradeView* _tradeView;

    public:
        IOInterface();

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
        }

    public:
        void addPosition(const StrategyId, const TickerId, const OutputType type);
        void updatePositionForExecution(const Position*, const Position*, const OutputType type);
        void updatePositionForLastPrice(const Position*, const OutputType type);

        void updateOrderExecution(const OpenOrder*, const OutputType);
        void addOrder(const OpenOrder*, const String&, const OutputType);
        void addOrder(const OrderId, const OrderDetail&, const OutputType);
        void removeOrder(const OrderId, const OutputType );
        void updateOrderStatus(const OpenOrder*, const OutputType);
        void updateOrderExecution(const OrderId, const OrderDetail&, const OutputType);

        void updateSpread(const Spread*, const OutputType);


        void updatePerformance(const StrategyId, const PerformanceStats&, const OutputType type);
        void reportEvent(const String& reporter, const String& report, const MessageType type = INFO);
        void addInstrument(const TickerId, const InstrumentContract&);
        void addInstrument(const TickerId);

    public:
        InstrumentViewWidget* getInstrumentView();

//    signals:
//        void positionCreated(const StrategyId, const TickerId);

//        void positionUpdatedForExecution(const StrategyId, const TickerId, const PositionDetail&);

//        void positionUpdatedForLastPrice(const StrategyId, const TickerId, const PositionDetail&);

//        void orderPlaced(const OrderId, const OrderDetail&);

//        void orderDeleted(const OrderId);

//        void orderUpdated(const OrderId, const long, const long, const double, const double);

//        void orderUpdated(const OrderId, const OrderDetail&);

//        void orderStatusUpdated(const OrderId, const OrderStatus);

//        void strategyUpdated(const StrategyId, const PerformanceStats&);

//        void eventReported(const QDateTime&, const String, const String, const MessageType);

//        void instrumentAdded(const TickerId);

//        void spreadUpdated(const StrategyId, const SpreadId, const SpreadDetail&);
//        void spreadPositionUpdated(const StrategyId, const SpreadId, const PositionDetail&);
};

#endif // OUTPUTINTERFACE_H
