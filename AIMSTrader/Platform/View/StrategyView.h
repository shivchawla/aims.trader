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

    private:
        StrategyViewItem* getStrategyViewItem(const StrategyId);

    signals:
        void closed();
};

#endif // STRATEGYVIEW_H
