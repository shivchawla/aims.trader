#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include <map>
#include <QObject>
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/Order.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/OrderStatus.h"
class OpenOrder;

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

    public slots:
        void updateOpenOrderExecution(const OrderId, const Contract&, const Execution&);
        void updateOrderStatus(const OrderId, const OrderStatus);
        void placeOrder(const Order&, const Contract&); //this slot is called by strategy
        void removeOpenOrder(const OrderId);
        void addOpenOrder(const Contract&, const Order&);
        void printThreadId();

   signals:
        void requestPlaceOrdertoTA(const OrderId, const Order&, const Contract&);
        void requestTickerId(const Contract& contract);
        void orderPlaced(const OrderId, const Contract&);
};

#endif // ORDERMANAGER_H
