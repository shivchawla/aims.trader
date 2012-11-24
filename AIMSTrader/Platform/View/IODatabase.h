#ifndef IODATABASE_H
#define IODATABASE_H

#include <QObject>
#include "framework.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Position/Position.h"
#include "PLatform/Position/OpenOrder.h"
#include "Platform/Startup/Service.h"

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
        QList<StrategyData> getStrategies();
        StrategyCompositeData getCompositeStrategy(const QString& strategyName);
        QList<PositionData> getStrategyLinkedPositions();
        //QList<StrategyLinkedPositionData*> getPositionsForStrategy(const QString& strategyName);
        QList<ATContract> getATContractsForStrategy(const QString& strategyName);
        QList<InstrumentData> getStrategyBuyList(const StrategyId);
        QList<PositionData> getOpenStrategyLinkedPositions(const StrategyId strategyId);
        //QList<StrategyLinkedPositionData*> getStrategyLinkedPositionsById(const uint strategyId);
        QHash<QString, QString> getStrategyConfigurations(uint strategyId);

        QList<OrderData> getOrdersByStrategyName(const QString& strategyName);
        QList<InstrumentData> getInstrumentsWithSimilarSymbol(const QString& symbol);

        void setupDatabaseSession(const Mode);

    public slots:
       void addPosition(const StrategyId, const TickerId);
       void updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&);
       void updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double);
       //void addOrder(const OrderId, const Order&, const Contract&, const String&);
       int addOrder(const OrderId, const OrderDetail&, const QString&);
       int addOrder(const OrderId, const OrderDetail&);

       int updateOrder(const OrderId, const OrderDetail&);
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
