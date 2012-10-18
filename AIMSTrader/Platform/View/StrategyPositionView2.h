#ifndef STRATEGYPOSITIONVIEW2_H
#define STRATEGYPOSITIONVIEW2_H

//class QTableView;
class StrategyPositionModel2;
class QWidget;
class OrderEntryDialog;
#include "Platform/View/TableView2.h"
#include "Platform/Model/StrategyPositionModel.h"

class StrategyPositionView2 : public TableView2<StrategyPositionView2, StrategyPositionModel2>//, StrategyPositionModelColumn>
{
    Q_OBJECT
    public:
        StrategyPositionView2(QWidget* parent = 0);

    private slots:
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);
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

    private:
        void setupView();
        void setupActions();

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

#endif // STRATEGYPOSITIONVIEW2_H
