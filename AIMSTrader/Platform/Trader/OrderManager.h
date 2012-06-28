#pragma once
#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H
#include <map>
#include <QObject>
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Execution.h"
#include "InteractiveBroker/Shared/Order.h"
#include "AimsTraderDefs/typedefs.h"
//#include "Platform/Enumerations/OrderStatus.h"
#include <QReadWriteLock>
#include "Platform/Model/Mode.h"

class OpenOrder;
class Strategy;
class QSignalMapper;

typedef std::map<OrderId, OpenOrder*> OpenOrderMap;

class OutputInterface;
class OrderManager //: public QObject
{
    //Q_OBJECT
    private:
        OpenOrderMap _openOrders;
        OrderId _orderId;
        Mode _mode;
        OutputInterface* _outputInterface;
        std::map<OrderId, Strategy*> _orderIdToStrategy;

    public:
        OrderManager();
        ~OrderManager();

    private:
        QReadWriteLock* _lockOpenOrderMap;

    public:
        void updateOrderStatus(const OrderId, const OrderStatus);
        void placeOrder(const Order&, const ATContract&, Strategy*);
        void placeOrder(const Order& , const TickerId, Strategy*);
        void removeOpenOrder(const OrderId);

        void updateOpenOrderOnExecution(const OrderId, const Execution&);
        void reportEvent(const String& message, const MessageType mType = INFO);
        bool IsClosingOrder(const OrderId orderId);
        void setMode(const Mode);
        const Order getOrder(const OrderId);
        void cancelOrder(const OrderId);

    private:
        void removeOrderFromOutputs(const OrderId);
        void addOrderInOutputs(const OpenOrder*, const String&);
        void updateStrategyForExecution(const OpenOrder*);
        void updateOrderExecutionInOutputs(const OpenOrder*);
        void updateOrderStatusInOutputs(const OpenOrder*);
        const OrderId addOpenOrder(const TickerId, const Order&, const Contract&, Strategy*);

//    signals:
//        void requestPlaceOrdertoTA(const OrderId, const Order&, const Contract&);
//        void requestTickerId(const Contract& contract);

};

#endif // ORDERMANAGER_H
