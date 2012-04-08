#ifndef OPENORDERVIEW_H
#define OPENORDERVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/typedefs.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include <map>

class OpenOrderViewItem;

class OpenOrderView: public TableView<OpenOrderView>
{   
    Q_OBJECT
    private:
        std::map<OrderId, OpenOrderViewItem*> _orderIdToItemMap;

    public:
        OpenOrderView(QWidget* parent);
        ~OpenOrderView();

    private:
        void setOpenOrderView();
        void setHeaders();

    public slots:
        //void update();
       void onExecutionUpdate(const OrderId, const ExecutionStatus&);
       void addOrder(const OrderId, const Order&, const Contract&, const String&);
       void removeOrder(const OrderId);
       void onStatusUpdate(const OrderId, const ExecutionStatus& );


    private:
        OpenOrderViewItem* getOpenOrderViewItem(const OrderId);
        const char *orderStatusToStr(const OrderStatus orderStatus);

    signals:
        void closed();

};

#endif // OPENORDERVIEW_H
