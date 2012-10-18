#ifndef OPENORDERMODEL2_H
#define OPENORDERMODEL2_H

#include <Platform/View/GeneralModel2.h>
#include "Platform/Position/Position.h"
#include "Platform/Model/OpenOrderModel.h"

class OpenOrderModelData
{
    private:
        OrderId _orderId;
        OrderDetail _orderDetail;

    public:
        OpenOrderModelData(const OrderId orderId, const OrderDetail orderDetail):_orderId(orderId), _orderDetail(orderDetail)
        {}

        OpenOrderModelData(const OrderId orderId):_orderId(orderId)
        {}

        OpenOrderModelData()
        {}

    public:
        const OrderDetail& getOrderDetail() const {return _orderDetail;}
        const OrderId getOrderId() const {return _orderId;}

        void setOrderDetail(const OrderDetail& detail)
        {
            _orderDetail = detail;
        }
};

class OpenOrderModel2 : public GeneralModel2<OpenOrderModel>
{
    //Q_OBJECT
    public:
        OpenOrderModel2(QObject *parent=0);

    public:
        QVariant data(const QModelIndex &index, int role) const;

    public:
        bool IsOrderActive(const QModelIndex&);
        bool getOrderId(const QModelIndex&);
        const OrderDetail& getOrderDetail(const QModelIndex&);
        const OrderDetail& getOrderDetail(const int row);

    public:
        void onStatusUpdate(const OrderId, const OrderStatus, const int tab);
        void updateOrder(const OrderId, const OrderDetail&, const int tab);
        void removeOrder(const OrderId);
        void addOrder(const OrderId, const OrderDetail&);

    public:
        void showAllOrders();
        void showOpenOrders();
        void showCanceledOrders();

    private:
        QList<OpenOrderModelData>  _orders;
        QHash<OrderId, int> _indices;
};

#endif // OPENORDERMODEL2_H
