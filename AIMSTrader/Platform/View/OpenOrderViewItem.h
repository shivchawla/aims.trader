#ifndef OPENORDERVIEWITEM_H
#define OPENORDERVIEWITEM_H

#include "Platform/Model/OpenOrderModel.h"
#include "Platform/View/TableViewItem.h"

class OpenOrderViewItem : public TableViewItem<OpenOrderViewItem>
{
    OrderStatus _status;

    public:
        OpenOrderViewItem(const int numItems);
        ~OpenOrderViewItem();

    public:
        void setOrderStatus(const OrderStatus status)
        {
            _status=status;
        }
        const OrderStatus getOrderStatus()
        {
            return _status;
        }

};

#endif // OPENORDERVIEWITEM_H

