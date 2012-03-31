#ifndef STRATEGYVIEWITEM_H
#define STRATEGYVIEWITEM_H

#include "Platform/View/TableItem.h"
#include "Platform/typedefs.h"

enum StrategyModelColumn{
        StrategyID=0,
        StrategyName,
        TotalTrades,
        WinningTrades,
        LosingTrades,
        ProfitFactor,
        NetProfit,
        Investment
};


class StrategyViewItem
{
    private:
        const static int _numItems = 8;
        TableItem _items[_numItems];

    public:
        StrategyViewItem();
        ~StrategyViewItem();

    public:
        void updateStrategyID(const StrategyId);
        void updateStrategyName(std::string&);
        void updateTotalTrades(const int);
        void updateWinningTrades(const int);
        void updateLosingTrades(const int);
        void updateProfitFactor(const double);
        void updateNetProfit(const double);
        void updateInvestment(const double);

    public:
        const static int getNumItems()
        {
            return _numItems;
        }
};

#endif // STRATEGYVIEWITEM_H
