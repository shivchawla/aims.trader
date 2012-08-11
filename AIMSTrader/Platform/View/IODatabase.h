#ifndef IODATABASE_H
#define IODATABASE_H

#include <QObject>
#include "framework.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Position/Position.h"
#include "PLatform/Position/OpenOrder.h"

class IODatabase: public QObject
{
    Q_OBJECT
    DatabaseSession* _session;

    public:
        IODatabase():QObject()
        {
           QThread* thread = ThreadManager::threadManager().requestThread();
           moveToThread(thread);
           //connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
           _session = new DatabaseSession();
        }

        ~IODatabase()
        {
            delete _session;
        }

        static IODatabase& ioDatabase()
        {
            static IODatabase ioDb;
            return ioDb;
        }

    public:
        QList<StrategyData*> getStrategies();
        StrategyCompositeData* getCompositeStrategy(const QString& strategyName);
        QList<StrategyLinkedPositionData*> getStrategyLinkedPositions();
        //QList<StrategyLinkedPositionData*> getPositionsForStrategy(const QString& strategyName);
        QList<ATContract*> getATContractsForStrategy(const QString& strategyName);
        QList<InstrumentData*> getStrategyBuyList(const QString &strategyName);
        QList<StrategyLinkedPositionData*> getOpenStrategyLinkedPositions(const uint strategyId);
        //QList<StrategyLinkedPositionData*> getStrategyLinkedPositionsById(const uint strategyId);

        QList<OrderData*> getOrdersByStrategyName(const QString& strategyName);

    public slots:
       void addPosition(const StrategyId, const TickerId);
       void updatePositionForExecution(const Position& position);
       void updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double);
       //void addOrder(const OrderId, const Order&, const Contract&, const String&);
       int addOrder(const OpenOrder&, const QString&);
       int updateOrder(const OpenOrder& order);
       void onExecutionUpdate(const OrderId, const long, const long, const double, const double);
       void onStatusUpdate(const OrderId, const OrderStatus);
       void updatePerformance(const StrategyId, const PerformanceStats&);

    public:

};

//static IODatabase& ioDatabase()
//{
//    static IODatabase ioDb;
//    return ioDb;
//}


#endif // IODATABASE_H
