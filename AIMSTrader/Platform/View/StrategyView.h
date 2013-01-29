#pragma once
#ifndef STRATEGYVIEW_H
#define STRATEGYVIEW_H

#include "Platform/View/TableView.h"
#include "AimsTraderDefs/typedefs.h"
#include "Platform/View/StrategyViewItem.h"
#include "Platform/Model/StrategyModel.h"

class StrategyView : public TableView<StrategyView, StrategyViewItem, StrategyModel, StrategyModelColumn>
{
    Q_OBJECT
    private:
        QHash<StrategyId, StrategyViewItem*> _strategyIdToItemMap;

    public:
        StrategyView(QWidget* parent);
        //~StrategyView();

    private:
        //void setStrategyView();
        //void setHeaders();

    public slots:
//        void update();
//        void onPerformanceUpdate();
        void updatePerformance(const StrategyId, const PerformanceStats&);
//        void onExecutionUpdate(const StrategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission);
//        void onTradeUpdate(const StrategyId, const int profitableTrades, const double netProfit);
//        //void addStrategy(const StrategyId, const String& strategyName);
//        void updateTrades(const StrategyId strategyId, const int trades, const int longTrades, const int shortTrades);
        void contextMenuEvent(QContextMenuEvent *event);

    private:
        StrategyViewItem* getStrategyViewItem(const StrategyId);

    private:
        QAction* stopStrategyAction;
        QAction* closePositions;
        QAction* terminate;
        QMenu* strategyMenu;
        QMenu* headerMenu;
        StrategyId _strategyIdClicked;

    private:
        void setupActions();

    private slots:
        void stopStrategy();
        void closeAllPositions();
        void updateContextMenu();
        void onRemoveHeader();
        void modifyHeaders(const int);

    signals:
        void closed();
        void modifyHeadersClicked(const int);
};

#endif // STRATEGYVIEW_H
