#ifndef SPREADSTRATEGY_H
#define SPREADSTRATEGY_H

#include "Strategy/StrategyImpl.h"

class SpreadManager;

class SpreadStrategy: public Strategy
{
    friend class SpreadManager;
    private:
        QList<SpreadData> _spreadBuyList;

    private:
        SpreadManager* _spreadManager;
        QHash<OrderId, SpreadId> _orderIdToSpreadId;

    public:
        SpreadStrategy();
        SpreadStrategy(const String&);

    private:
        void loadSpreadList(const DbStrategyId);
        void loadSpreadPositions(const DbStrategyId);
        void registerSpreadList();
        void testStrategy();
        void updatePendingQuantity(const SpreadId, const Order&, const Contract&);
        void createIndicator();
        void closeAll();

    public:
        void startStrategy();
        void stopStrategy();
        void placeSpreadOrder(const SpreadId, const Order&, const Contract&);
        void placeSpreadOrder(const SpreadId, const  TickerId, const Order&, const bool isClosingOrder = false);
        void loadSpreads(const DbStrategyId);
        void closeAllSpreads();
        void loadStrategyDataFromDb(const StrategyData&);
        void requestStrategyUpdateForExecution(const OrderId,  const OrderDetail&);
        void placeOrder(const TickerId, const Order &);

    protected:
        void updatePositionOnExecution(const OrderId, const OrderDetail&);

    public:
        void onTickPriceUpdate(const TickerId, const TickType, const double);
};

#endif // SPREADSTRATEGY_H
