#include "Platform/View/StrategyView.h"
#include "Platform/View/StrategyViewItem.h"

StrategyView::StrategyView(QWidget* parent = 0):TableView<StrategyView>(parent)
{
    _numCols=StrategyViewItem::getNumItems();
    setStrategyView();
}

void StrategyView::setStrategyView()
{
    setMinimumSize(StrategyViewItem::getNumItems()*80, 200);
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();

    //resize based on header
}

void StrategyView::setHeaders()
{
    _header<<"StrategyID"<<"StrategyName"<<"TotalTrades"<<"WinningTrades"<<"LosingTrades"<<"ProfitFactor"<<"NetProfit"<<"Investment" ;
    setHorizontalHeaderLabels(_header);
}

StrategyViewItem* StrategyView::getStrategyViewItem(const StrategyId strategyId)
{
    if(_strategyIdToItemMap.count(strategyId)!=0)
    {
        return _strategyIdToItemMap[strategyId];
    }
    return NULL;
}

StrategyView::~StrategyView()
{}

void StrategyView::update()
{}

void StrategyView::onPerformanceUpdate()
{

}




