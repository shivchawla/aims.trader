#ifndef RISKMANAGER_H
#define RISKMANAGER_H

#include "AimsTraderDefs/typedefs.h"
#include "Platform/Position/Position.h"
#include <QReadWriteLock>
#include "Platform/Utils/DataSubscriber.h"

typedef QHash<TickerId, Position*> PositionMap;

class RiskManager : public DataSubscriber
{
    Q_OBJECT
    PositionMap _positionMap;
    QMutex _mutex;

    public:
        RiskManager();

    private:
        void addPosition(const TickerId);

    public:
        void updatePosition(const OrderId, const TickerId, const OrderDetail&);
        void onTickPriceUpdate(const TickerId, const TickType, const double);
        bool canPlaceOrder(const TickerId, const Order&);

    private slots:
        void updatePositionForExecution(const OrderId, const TickerId, const OrderDetail&);
};

#endif // RISKMANAGER_H
