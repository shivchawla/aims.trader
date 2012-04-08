#ifndef STRATEGYVIEWITEM_H
#define STRATEGYVIEWITEM_H

#include "Platform/View/TableItem.h"
#include "Platform/typedefs.h"
#include "Platform/Model/StrategyModel.h"

class StrategyViewItem
{
    private:
        const static int _numItems = 10;
        TableItem* _items[_numItems];

    public:
        StrategyViewItem();
        ~StrategyViewItem();

    public:
        void updateStrategyID(const StrategyId);
        void updateStrategyName(const String&);
        void updateTotalTrades(const int);
        void updateWinningTrades(const int);
        void updateLosingTrades(const int);
        void updateProfitFactor(const double);
        void updateNetProfit(const double);
        void updateInvestment(const double);
        void updateTotalBought(const double);
        void updateTotalSold(const double);
        void update(const String&, const StrategyModelColumn);
        static const String getHeaderName(const StrategyModelColumn);

    public:
        const static int getNumItems()
        {
            return _numItems;
        }
        TableItem* getTableItem(const int col);

};

#endif // STRATEGYVIEWITEM_H
