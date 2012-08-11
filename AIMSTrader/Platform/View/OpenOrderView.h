#pragma once
#ifndef OPENORDERVIEW_H
#define OPENORDERVIEW_H

#include "Platform/View/TableView.h"
#include "AimsTraderDefs/typedefs.h"
#include "Platform/Model/OpenOrderModel.h"
#include <map>
#include "Platform/Position/OpenOrder.h"

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

     private slots:
        void cancelOrder();
        void cancelReplaceOrder();
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);

    public:
        OpenOrderView(QWidget* parent);
        //~OpenOrderView();

    private:
        void setupActions();
        void showHideOrder(OpenOrderViewItem*, const int tab);

    public slots:

        void updateOrder(const OpenOrder&, const int);
        void addOrder(const OpenOrder&, const QString&, const int);

        void removeOrder(const OrderId);
        void onStatusUpdate(const OrderId, const OrderStatus, const int);
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
