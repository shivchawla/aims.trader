#ifndef SPREADMANAGER_H
#define SPREADMANAGER_H

#include <QHash>
#include "Platform/Position/Position.h"

class Strategy;
class Position;
class PositionManager;
class Spread;
class SpreadStrategy;


class SpreadManager
{
    private:
        QHash<SpreadId, Spread*> _spreads;
        QHash<TickerId, QList<SpreadId> > _tickerIdsToListOfSpreadId;
        SpreadStrategy* _spreadStrategy;
        StrategyId _strategyId;
        PositionManager* _positionManager;

    public:
        SpreadManager(SpreadStrategy*);

    public:
        void updateSpread(const TickerId, const TickType, const double price);
        void updateSpread(const OrderId, const TickerId, const OrderDetail&);
        void updateSpread(const OrderId, const SpreadId, const TickerId, const OrderDetail&);
        void addSpreadPosition(const SpreadId, const TickerId, const SpreadPositionData&);


        void addSpread(const SpreadId, const TickerId, const TickerId);
        void addSpread(const TickerId, const TickerId);
        void addSpread(const SpreadId);

        void closeSpread(const SpreadId);
        void closeAllSpreads();
        void adjustSpread(const SpreadId, const TickerId, const Order&);
        void adjustSpread(const SpreadId, const Order&);
        void placeSpreadOrder(const TickerId, const TickerId, const double multiplier = 1);

     private:
        void updateMainPosition(const Position* subPosition);
        void testExitConditions(const Spread* spread);
        void updateOutputForPrice(const Spread* spread);
        void updateOutputForExecution(const Spread* spread);
        void loadSpreadPosition(const SpreadId, const TickerId, const SpreadPositionData&);
};

#endif // SPREADMANAGER_H
