#ifndef STRATEGYVIEW2_H
#define STRATEGYVIEW2_H
#include "Platform/View/TableView2.h"
#include "Platform/View/StrategyModel2.h"

class StrategyView2 : public TableView2<StrategyView2, StrategyModel2>
{
    Q_OBJECT
    public:
        StrategyView2(QWidget* parent);

    public slots:
        void contextMenuEvent(QContextMenuEvent *event);

    private:
        QAction* stopStrategyAction;
        QAction* closePositions;
        QAction* terminate;
        QMenu* strategyMenu;
        QMenu* headerMenu;

    private:
        void setupActions();

    private slots:
        void stopStrategy();
        void closeAllPositions();
        void updateContextMenu();
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);

    public slots:
        void updatePerformance(const StrategyId, const PerformanceStats&);

};

#endif // STRATEGYVIEW2_H
