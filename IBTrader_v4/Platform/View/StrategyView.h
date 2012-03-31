#ifndef STRATEGYVIEW_H
#define STRATEGYVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/typedefs.h"
#include "Platform/View/StrategyViewItem.h"

class StrategyView : public TableView
{
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

    private:
        StrategyViewItem* getStrategyViewItem(const StrategyId);
};

#endif // STRATEGYVIEW_H
