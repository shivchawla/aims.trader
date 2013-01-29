#ifndef OPENORDERVIEW3_H
#define OPENORDERVIEW3_H

#include "Platform/View/TableView3.h"
#include "Platform/View/OpenOrderViewItem2.h"
#include "Platform/Model/OpenOrderModel.h"

class OrderEntryDialog;

class OpenOrderView3 : public TableView3<OpenOrderView3, OpenOrderViewItem2, OpenOrderModel>
{
        Q_OBJECT
        private:
            QHash<OrderId, uint> _orderIdToRow;

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
            //void onCustomizeHeader();
            void modifyHeaders(const int);

        public:
            OpenOrderView3(QWidget* parent = 0);

        private:
            void setupActions();
            void updateRow(const int, const OrderDetail&);
            int getRow(const OrderId orderId)
            {
                return _orderIdToRow.value(orderId, -1);
            }
            void addRow(const OrderId, const OrderDetail&);
            void showHideOrder(const int row, const int tab);

        public slots:
            void updateOrder(const OrderId, const OrderDetail&, const int);
            void addOrder(const OrderId, const OrderDetail&, const int tab);
            void removeOrder(const OrderId);
            void onStatusUpdate(const OrderId, const OrderStatus, const int);
            void contextMenuEvent(QContextMenuEvent *event);

        public:
            void showAllOrders();
            void showOpenOrders();
            void showCanceledOrders();
};

#endif // OPENORDERVIEW3_H
