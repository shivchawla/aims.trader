#ifndef OPENORDERVIEWITEM2_H
#define OPENORDERVIEWITEM2_H

#include "Platform/View/TableViewItem2.h"
#include "AimsTraderDefs/typedefs.h"

class OpenOrderViewItem2 : public TableViewItem2
{
    OrderStatus _status;
    OrderId _orderId;

    public:
        OpenOrderViewItem2();
        //~OpenOrderViewItem2();

    public:
        void setOrderStatus(const OrderStatus status)
        {
            _status=status;
        }

        const OrderStatus getOrderStatus() const
        {
            return _status;
        }

        void setOrderId(const OrderId orderId)
        {
            _orderId = orderId;
        }

        const OrderId getOrderId() const
        {
            return _orderId;
        }

};

#endif // OPENORDERVIEWITEM2_H
