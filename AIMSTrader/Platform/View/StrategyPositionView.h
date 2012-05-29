#ifndef POSITIONVIEW_H
#define POSITIONVIEW_H
#include "Platform/View/TableView.h"
#include "Platform/typedefs.h"

class StrategyPositionViewItem;
class StrategyPositionView : public TableView<StrategyPositionView>
{
    Q_OBJECT
    private:
        //std::map<StrategyId, std::map<PositionId, PositionViewItem*> > _positionMap;
         std::map<StrategyId, std::map<TickerId, StrategyPositionViewItem*> > _positionMap;

   public:
        StrategyPositionView(QWidget* parent);
        ~StrategyPositionView();

   private:
        void setPositionView();
        void setHeaders();

    public slots:
        void update();
        //void onPerformanceUpdate();
        //void addPosition(const StrategyId, const PositionId, const TickerId);
        void addPosition(const StrategyId, const TickerId);
        //void onExecutionUpdate(const StrategyId, const TickerId, const int quantity, const double avgFillPrice, const double positionValue, const double commission);
        //void onTradeUpdate(const StrategyId, const TickerId, const double positionValue, const double netProfit);
        void updatePositionForExecution(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void updatePositionForLastPrice(const StrategyId, const TickerId, const double, const double);
        void removePosition(const StrategyId, const TickerId);

    private:
        StrategyPositionViewItem* getPositionViewItem(const StrategyId, const TickerId);

    signals:
        void closed();

};

#endif // POSITIONVIEW_H
