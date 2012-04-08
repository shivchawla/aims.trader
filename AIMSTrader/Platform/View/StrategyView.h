#ifndef STRATEGYVIEW_H
#define STRATEGYVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/typedefs.h"
#include "Platform/View/StrategyViewItem.h"

class StrategyView : public TableView<StrategyView>
{
    Q_OBJECT
    private:
        std::map<StrategyId, StrategyViewItem*> _strategyIdToItemMap;

    public:
        StrategyView(QWidget* parent);
        ~StrategyView();

    private:
        void setStrategyView();
        void setHeaders();

    public slots:
        void update();
        void onPerformanceUpdate();
        void onExecutionUpdate(const StrategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission);
        void onTradeUpdate(const StrategyId, const int profitableTrades, const double netProfit);
        void addStrategy(const StrategyId, const String& strategyName);
        void updateTrades(const StrategyId strategyId, const int trades, const int longTrades, const int shortTrades);


    private:
        StrategyViewItem* getStrategyViewItem(const StrategyId);

    signals:
        void closed();
};

#endif // STRATEGYVIEW_H
