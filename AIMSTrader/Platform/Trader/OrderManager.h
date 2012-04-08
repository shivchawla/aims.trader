#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include <map>
#include <QObject>
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/Order.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/OrderStatus.h"
#include <QReadWriteLock>

class OpenOrder;
class Strategy;

typedef std::map<OrderId, OpenOrder*> OpenOrderMap;

class OrderManager : public QObject
{
    Q_OBJECT
    private:
        OpenOrderMap _openOrders;
        OrderId _orderId;
    public:
        OrderManager();
        ~OrderManager();

    private:
        QReadWriteLock* lockOpenOrderMap;

    public:
        void updateOrderStatus(const OrderId, const OrderStatus);
        void placeOrder(const Order&, const Contract&, Strategy*);
        void placeOrder(const Order& , const TickerId, Strategy*);
        void removeOpenOrder(const OrderId);
        void addOpenOrder(const Contract&, const Order&);
        void updateOpenOrderOnExecution(const OrderId, const Contract&, const Execution&);

    public slots:
        void printThreadId();

    signals:
        void requestPlaceOrdertoTA(const OrderId, const Order&, const Contract&);
        void requestTickerId(const Contract& contract);
        void orderPlaced(const OrderId, const Order&, const Contract&, const String&);
        void orderDeleted(const OrderId);
};

#endif // ORDERMANAGER_H
