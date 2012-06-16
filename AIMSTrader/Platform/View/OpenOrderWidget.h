#ifndef OPENORDERWIDGET_H
#define OPENORDERWIDGET_H

class OpenOrderView;
class QTabBar;
#include <QWidget>
#include "Platform/typedefs.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Contract.h"

class OpenOrderWidget : public QWidget
{
    Q_OBJECT
    private:
        OpenOrderView* _openOrderView;
        QTabBar* _tabBar;
//        QAction* _showAllOrders;
//        QAction* _showCanceledOrders;
//        QAction* _showOpenOrders;

    public:
        OpenOrderWidget(QWidget* parent = 0);

    public slots:
       void onExecutionUpdate(const OrderId orderId, const long filledQuantity, const long pendingQuantity, const double avgFillPrice, const double lastFillPrice);
       void addOrder(const OrderId, const Order&, const Contract&, const String&);
       void removeOrder(const OrderId);
       //void onStatusUpdate(const OrderId, const String);
       void onStatusUpdate(const OrderId, const OrderStatus);

    private slots:
       void showOrders(int x);
};

#endif // OPENORDERWIDGET_H
