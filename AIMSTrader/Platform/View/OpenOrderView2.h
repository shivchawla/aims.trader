#ifndef OPENORDERVIEW2_H
#define OPENORDERVIEW2_H
#include "Platform/View/TableView2.h"
#include "Platform/View/OpenOrderModel2.h"
#include "Platform/View/OrderEntryDialog.h"

class OpenOrderView2 : public TableView2<OpenOrderView2, OpenOrderModel2>
{
    public:
        OpenOrderView2(QWidget* parent = 0);

//    public:
//        OpenOrderModel2* getOpenOrderModel()
//        {
//            return _model;
//        }

    private slots:
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);
        void cancelOrder();
        void cancelReplaceOrder();

     public:
        void contextMenuEvent(QContextMenuEvent*);

    private:
        QMenu* _openOrderMenu;
        QMenu* _openOrderMenuB;
        QAction* _cancel;
        QAction* _cancelReplace;
        OrderEntryDialog* _orderEntryDialog;

    private:
        void setupView();
        void setupActions();

    public:
        void showAllOrders();
        void showOpenOrders();
        void showCanceledOrders();
        void showHideOrder(const int tab);
};


#endif // OPENORDERVIEW2_H
