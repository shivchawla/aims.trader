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
        //QHash<QPair<TickerId,TickerId>, SpreadId>  _tickerIdsToSpreadId;
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
        void addSpread(const TickerId, const TickerId);
        void addSpread(const SpreadId);

        void closeSpread(const SpreadId);
        void closeAllSpreads();
        void adjustSpread(const SpreadId, const TickerId, const Order&);
        void adjustSpread(const SpreadId, const Order&);
        void placeSpreadOrder(const TickerId, const TickerId);

     private:
        void updateMainPosition(const Position* subPosition);
        void testExitConditions(const Spread* spread);
        void updateOutput(const Spread* spread);

};

#endif // SPREADMANAGER_H
