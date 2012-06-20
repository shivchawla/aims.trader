#pragma once
#ifndef OPENORDERVIEWITEM_H
#define OPENORDERVIEWITEM_H

#include "Platform/Model/OpenOrderModel.h"
#include "Platform/View/TableViewItem.h"

class OpenOrderViewItem : public TableViewItem<OpenOrderViewItem>
{
    OrderStatus _status;
    OrderId _orderId;

    public:
        OpenOrderViewItem(const int numItems);
        ~OpenOrderViewItem();

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

#endif // OPENORDERVIEWITEM_H

