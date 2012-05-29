#include "Platform/View/StrategyView.h"
#include "Platform/View/StrategyViewItem.h"
#include "Platform/Model/StrategyModel.h"
#include "Platform/Strategy/StrategyManager.h"
#include <iostream>

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
    int numItems = StrategyViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        StrategyModelColumn column = static_cast<StrategyModelColumn>(i);

        _header<<StrategyViewItem::getHeaderName(column); //"StrategyID"<<"StrategyName"<<"TotalTrades"<<"WinningTrades"<<"LosingTrades"<<"ProfitFactor"<<"NetProfit"<<"TotalBought"<<"TotalSold"<<"Investment" ;
        //String s  = StrategyViewItem::getHeaderName(column);
        //std::cout<<s.toStdString();
    }
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

void StrategyView::onExecutionUpdate(const StrategyId strategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission)
{
    if(_strategyIdToItemMap.count(strategyId)!=0)
    {
        _strategyIdToItemMap[strategyId]->updateWinningTrades(profitableTrades);
        _strategyIdToItemMap[strategyId]->updateTotalBought(totalBought);
        _strategyIdToItemMap[strategyId]->updateTotalBought(totalSold);
        //_strategyIdToItemMap[strategyId]->updateTotalCommission()
    }
    else
    {
        StrategyViewItem* newItem = new StrategyViewItem();
        _strategyIdToItemMap[strategyId] = newItem;
        int currentRow= _numRows++;
        insertRow(currentRow);
        int numItems = StrategyViewItem::getNumItems();

        for(int i=0;i<numItems;++i)
        {
            setItem(currentRow,i,newItem->getTableItem(i));
        }

        _strategyIdToItemMap[strategyId]->updateStrategyName(StrategyManager::manager()->getStrategyName(strategyId));
        _strategyIdToItemMap[strategyId]->updateStrategyID(strategyId);

    }
}

void StrategyView::onTradeUpdate(const StrategyId strategyId, const int profitableTrades, const double netProfit)
{
    if(_strategyIdToItemMap.count(strategyId)!=0)
    {
        _strategyIdToItemMap[strategyId]->updateWinningTrades(profitableTrades);
        _strategyIdToItemMap[strategyId]->updateNetProfit(netProfit);
    }
}

void StrategyView::addStrategy(const StrategyId strategyId, const String& strategyName)
{
    if(_strategyIdToItemMap.count(strategyId)==0)
    {
        _strategyIdToItemMap[strategyId] = new StrategyViewItem();
        _strategyIdToItemMap[strategyId]->updateStrategyName(strategyName);
        _strategyIdToItemMap[strategyId]->updateStrategyID(strategyId);
    }
}

void StrategyView::updateTrades(const StrategyId strategyId, const int trades, const int longTrades, const int shortTrades)
{
    if(_strategyIdToItemMap.count(strategyId)!=0)
    {
        _strategyIdToItemMap[strategyId]->updateTotalTrades(trades);
    }
}



