#pragma once
#ifndef POSITIONVIEW_H
#define POSITIONVIEW_H
#include "Platform/View/TableView.h"
#include "AimsTraderDefs/typedefs.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/Position/Position.h"
#include "Data/InstrumentData.h"

class StrategyPositionViewItem;
class OrderEntryDialog;

class StrategyPositionView : public TableView<StrategyPositionView, StrategyPositionViewItem, StrategyPositionModel, StrategyPositionModelColumn>
{
    Q_OBJECT
    private:
        //std::map<StrategyId, std::map<PositionId, PositionViewItem*> > _positionMap;
         std::map<StrategyId, std::map<InstrumentId, StrategyPositionViewItem*> > _positionMap;
         //TableCellItem<StrategyPositionViewItem>* _clickedItem;
         OrderEntryDialog* _orderEntryDialog;

   public:
        StrategyPositionView(QWidget* parent);
        //~StrategyPositionView();

   private:
        //void setPositionView();
        //void setHeaders();

    public slots:
        void update();
        //void onPerformanceUpdate();
        //void addPosition(const StrategyId, const PositionId, const TickerId);
        void addPosition(const StrategyId, const InstrumentId);
        //void onExecutionUpdate(const StrategyId, const TickerId, const int quantity, const double avgFillPrice, const double positionValue, const double commission);
        //void onTradeUpdate(const StrategyId, const TickerId, const double positionValue, const double netProfit);
        //void updatePositionForExecution(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void updatePositionForExecution(const Position&);

        void updatePositionForLastPrice(const StrategyId, const InstrumentId, const double, const double);
        void removePosition(const StrategyId, const InstrumentId);
        void contextMenuEvent(QContextMenuEvent *event);

    private slots:
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);
        void placeOrderfromDialog();


    private:
        StrategyPositionViewItem* getPositionViewItem(const StrategyId, const InstrumentId);

    private:
        QMenu* positionMenu;
        QMenu* splitMenu;
        QAction* closePositionAction;
        QAction* tradeAction;
        QAction* buyAction;
        QAction* sellAction;

    private:
        void setupActions();

    private slots:
        void closePosition();
        void buyPosition();
        void sellPosition();

        void updateContextMenu();

    signals:
        void closed();
        void modifyHeadersClicked(const int);

};

#endif // POSITIONVIEW_H
