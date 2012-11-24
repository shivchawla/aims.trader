#pragma once
#ifndef OPENORDERWIDGET_H
#define OPENORDERWIDGET_H

class OpenOrderView;
class OpenOrderView2;
class QTabBar;
#include <QWidget>
#include "AimsTraderDefs/typedefs.h"
//#include "InteractiveBroker/Shared/Order.h"
//#include "InteractiveBroker/Shared/Contract.h"
#include "Platform/Position/OpenOrder.h"

class OpenOrderWidget : public QWidget
{
    Q_OBJECT
    private:
        OpenOrderView* _openOrderView;
        QTabBar* _tabBar;

    public:
        OpenOrderWidget(QWidget* parent = 0);


    public slots:
        void updateOrder(const OrderId, const OrderDetail&);

        //void addOrder(const OrderId, const Order&, const Contract&, const String&);
        void addOrder(const OrderId, const OrderDetail&, const QString&);
        void addOrder(const OrderId, const OrderDetail&);

        void removeOrder(const OrderId);
       //void onStatusUpdate(const OrderId, const String);
       void onStatusUpdate(const OrderId, const OrderStatus);

    private slots:
       void showOrders(int x);
};

#endif // OPENORDERWIDGET_H
