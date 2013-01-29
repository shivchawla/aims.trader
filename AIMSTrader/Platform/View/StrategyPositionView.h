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
         std::map<StrategyId, std::map<TickerId, StrategyPositionViewItem*> > _positionMap;
         OrderEntryDialog* _orderEntryDialog;

   public:
        StrategyPositionView(QWidget* parent);

    public slots:
        void update();
        void addPosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&);

        void updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double);
        void removePosition(const StrategyId, const TickerId);
        void contextMenuEvent(QContextMenuEvent *event);

    private slots:
        void onRemoveHeader();
        void onCustomizeHeader();
        void placeOrderfromDialog();
        void modifyHeaders(const int);

    private:
        StrategyPositionViewItem* getPositionViewItem(const StrategyId, const TickerId);

    private:
        QMenu* positionMenu;
        QMenu* splitMenu;
        QAction* closePositionAction;
        QAction* tradeAction;
        QAction* buyAction;
        QAction* sellAction;

    private:
        void setupActions();
        bool canModifyPosition(const StrategyId, const TickerId);

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
