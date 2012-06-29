#pragma once
#ifndef OPENORDERVIEW_H
#define OPENORDERVIEW_H

#include "Platform/View/TableView.h"
#include "AimsTraderDefs/typedefs.h"
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Order.h"
#include "Platform/Model/OpenOrderModel.h"
#include <map>

class OpenOrderViewItem;
class OrderEntryDialog;
class OpenOrderView: public TableView<OpenOrderView, OpenOrderViewItem, OpenOrderModel, OpenOrderModelColumn>
{   
    Q_OBJECT

    typedef std::map<OrderId, OpenOrderViewItem*> OpenOrderMap;
    typedef OpenOrderMap::iterator OpenOrderMapIterator;

    private:
        OpenOrderMap _orderIdToItemMap;

    private:
        QMenu* _openOrderMenu;
        QMenu* _openOrderMenuB;
        QAction* _cancel;
        QAction* _cancelReplace;
        OrderEntryDialog* _orderEntryDialog;

        //QAction*
     private slots:
        void cancelOrder();
        void cancelReplaceOrder();
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);

    public:
        OpenOrderView(QWidget* parent);
        ~OpenOrderView();

    private:
        void setupActions();
        //void setOpenOrderView();
        //void setHeaders();

    public:
       void onExecutionUpdate(const OrderId orderId, const long filledQuantity, const long pendingQuantity, const double avgFillPrice, const double lastFillPrice);
       void addOrder(const OrderId, const Order&, const Contract&, const String&);
       void removeOrder(const OrderId);
       //void onStatusUpdate(const OrderId, const String);
       void onStatusUpdate(const OrderId, const OrderStatus);
       void contextMenuEvent(QContextMenuEvent *event);

    private:
        OpenOrderViewItem* getOpenOrderViewItem(const OrderId);
        const char *orderStatusToStr(const OrderStatus orderStatus);

    signals:
        void closed();
        void modifyHeadersClicked(const int);

    public:
        void showAllOrders();
        void showOpenOrders();
        void showCanceledOrders();
};

#endif // OPENORDERVIEW_H
