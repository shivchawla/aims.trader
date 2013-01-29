/*
#ifndef STRATEGYSPREADPOSITIONVIEW_H
#define STRATEGYSPREADPOSITIONVIEW_H

#include "Platform/View/TreeView.h"
#include "Platform/Model/StrategyPositionModel.h"
#include

class StrategySpreadPositionView : public TreeView<StrategySpreadPositionView, StrategySpreadPositionViewItem, StrategyPositionModel, StrategyPositionModelColumn>
{
        Q_OBJECT
        private:
             std::map<StrategyId, std::map<SpreadId, StrategyPositionViewItem*> > _spreadMap;
             std::map<StrategyId, std::map<SpreadId, std::map<TickerId , StrategyPositionViewItem*> > > _positionMap;


             //OrderEntryDialog* _orderEntryDialog;

        public:
            StrategySpreadPositionView();

        public slots:
            void update();
            void addPosition(const StrategyId, const TickerId);
            void updatePositionForExecution(const StrategyId, const TickerId, const SpreadDetail&);
            void updateSpreadPositionForExecution(const StrategyId, const SpreadId, const TickerId, const PositionDetail&);

            void updatePositionForLastPrice(const StrategyId, const SpreadId, const TickerId, const double, const double);
            void updatePositionForLastPrice(const StrategyId, const SpreadId, const double, const double);
            void removePosition(const StrategyId, const TickerId);
            void contextMenuEvent(QContextMenuEvent *event);

        private slots:
            void onRemoveHeader();
            void onCustomizeHeader();
            void placeOrderfromDialog();
            void modifyHeaders(const int);

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

};

#endif // STRATEGYSPREADPOSITIONVIEW_H
*/
