#ifndef POSITIONVIEW_H
#define POSITIONVIEW_H
#include "Platform/View/TableView.h"
#include "Platform/typedefs.h"

class PositionViewItem;
class PositionView : public TableView<PositionView>
{
    Q_OBJECT
    private:
        std::map<StrategyId, std::map<PositionId, PositionViewItem*> > _positionMap;

   public:
        PositionView(QWidget* parent);
        ~PositionView();

   private:
        void setPositionView();
        void setHeaders();

    public slots:
        void update();
        //void onPerformanceUpdate();
        void addPosition(const StrategyId, const PositionId, const TickerId);
        void onExecutionUpdate(const StrategyId, const PositionId, const int quantity, const double avgFillPrice, const double positionValue, const double commission);
        void onTradeUpdate(const StrategyId, const PositionId, const double positionValue, const double netProfit);
        void removePosition(const StrategyId, const PositionId);

    private:
        PositionViewItem* getPositionViewItem(const StrategyId, const PositionId);

    signals:
        void closed();

};

#endif // POSITIONVIEW_H
