#ifndef STRATEGYVIEW3_H
#define STRATEGYVIEW3_H
#include "Platform/View/TableView3.h"
#include "Platform/View/StrategyViewItem2.h"
#include "Platform/Model/StrategyModel.h"

class StrategyView3 : public TableView3<StrategyView3, StrategyViewItem2, StrategyModel>
{
        Q_OBJECT
        private:
            QAction* stopStrategyAction;
            QAction* closePositions;
            QAction* terminate;
            QMenu* strategyMenu;
            QMenu* headerMenu;
            QHash<StrategyId, int> _strategyIdToRow;

        private:
            void setupActions();
            int addRow(const StrategyId);

        public:
            StrategyView3(QWidget* parent = 0);

        private slots:
            void stopStrategy();
            void closeAllPositions();
            void updateContextMenu();
            void onRemoveHeader();
            void onCustomizeHeader();
            void modifyHeaders(const int);

        public slots:
            void updatePerformance(const StrategyId, const PerformanceStats&);
            void contextMenuEvent(QContextMenuEvent *event);

};

#endif // STRATEGYVIEW3_H
