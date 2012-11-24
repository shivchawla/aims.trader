#ifndef STRATEGYPOSITIONVIEW3_H
#define STRATEGYPOSITIONVIEW3_H

class OrderEntryDialog;

#include "Platform/View/TableView3.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/StrategyPositionViewItem2.h"

class StrategyPositionView3 : public TableView3<StrategyPositionView3, StrategyPositionViewItem2, StrategyPositionModel>//, StrategyPositionModelColumn>
{
    Q_OBJECT
    public:
        StrategyPositionView3(QWidget* parent = 0);

    private slots:
        void placeOrderfromDialog();

    public:
        void contextMenuEvent(QContextMenuEvent*);

    private:
        QMenu* positionMenu;
        QMenu* splitMenu;
        QAction* closePositionAction;
        QAction* tradeAction;
        QAction* buyAction;
        QAction* sellAction;
        OrderEntryDialog* _orderEntryDialog;
        QHash<StrategyId, QHash<TickerId, int> > _strategyTickerIdToRow;

    private:
        void setupView();
        void setupActions();
        int addRow(const StrategyId, const TickerId);

    private slots:
        void closePosition();
        void buyPosition();
        void sellPosition();

    public slots:
        void updatePositionForLastPrice(const StrategyId, const TickerId, const double runningPnl, const double pnl);
        void removePosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&);
        void addPosition(const StrategyId, const TickerId);
        void updatePositionForLastPrice(const StrategyId, const TickerId, const PositionDetail&);
};


#endif // STRATEGYPOSITIONVIEW3_H
