#ifndef OPENORDERVIEW_H
#define OPENORDERVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include <map>
class QTableWidgetItem;


class OpenOrderView: public TableView<OpenOrderView>
{
    private:
        std::map<OrderId, QTableWidgetItem*> _orderIdToItemMap;
        enum OpenOrderModelColumn{
               OpenOrderId = 0,
               Strategy,
               SecurityType,
               Symbol,
               OrderStatus,
               FilledQuantity,
               RemainingQuantity,
               TotalQuantity,
               AvgFillPrice,
               LastFillPrice,
               DateTime
        };


    public:
        OpenOrderView();
        ~OpenOrderView();
        void init();
    public:
        void addOpenOrder(const OrderId, const Order&, const String&);
        void cancelOpenOrder(const OrderId);
        void removeOpenOrder(const OrderId);
        void updateOpenOrder(const OrderId, const Contract&, const Execution&);
        void updateOpenOrder(const OrderId, const String& status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const String& whyHeld);
};

#endif // OPENORDERVIEW_H
