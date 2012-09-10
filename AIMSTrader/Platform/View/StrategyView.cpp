#include "Platform/View/StrategyView.h"
#include "Platform/View/StrategyViewItem.h"
#include "Platform/Model/StrategyModel.h"
#include "Platform/Strategy/StrategyManager.h"
#include <iostream>
#include <QAction>
#include <QMenu>
#include <QDebug>

StrategyView::StrategyView(QWidget* parent = 0):TableView<StrategyView, StrategyViewItem, StrategyModel, StrategyModelColumn>(parent)
{
    _strategyIdClicked = -1;
     //setStrategyView();
     setupActions();
     connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
     //connect(customizeActions, SIGNAL(triggered()), this, SLOT(onCustomizeHeader()));
}

StrategyViewItem* StrategyView::getStrategyViewItem(const StrategyId strategyId)
{
    if(_strategyIdToItemMap.count(strategyId)!=0)
    {
        return _strategyIdToItemMap[strategyId];
    }
    return NULL;
}

void StrategyView::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    //qDebug()<<"StrategyId ="<<strategyId;
    StrategyViewItem* newItem;
    if(!(newItem = _strategyIdToItemMap.value(strategyId, NULL)))
    {
        newItem = addItemInView();
        newItem->setStrategyId(strategyId);
        _strategyIdToItemMap[strategyId] = newItem;
        newItem->update(StrategyManager::strategyManager().getStrategyName(strategyId), StrategyModelStrategyName);
    }

    newItem->update(performanceStats.trades, StrategyModelTotalTrades);
    newItem->update(performanceStats.totalBought, StrategyModelTotalBought);
    newItem->update(performanceStats.totalSold, StrategyModelTotalSold);
    newItem->update(performanceStats.profitableTrades, StrategyModelWinningTrades);
    newItem->update(performanceStats.trades - performanceStats.profitableTrades, StrategyModelLosingTrades);
    newItem->update(performanceStats.profitFactor, StrategyModelProfitFactor);
    newItem->updateSpecial(performanceStats.runningGrossPnL, StrategyModelRunningGrossPnL);
    newItem->updateSpecial(performanceStats.realizedGrossPnL, StrategyModelRealizedGrossPnL);
    newItem->updateSpecial(performanceStats.netPnL, StrategyModelNetPnL);
    newItem->update(performanceStats.drawDown, StrategyModelDrawDown);
    newItem->update(performanceStats.maxDrawdown, StrategyModelMaxDrawDown);
    newItem->update(performanceStats.totalCommission, StrategyModelTotalCommission);
    newItem->update(0, StrategyModelInvestment);

}

//void StrategyView::onExecutionUpdate(const StrategyId strategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(profitableTrades), StrategyModelWinningTrades));
//        _strategyIdToItemMap[strategyId]->update(QString::number(totalBought), StrategyModelTotalBought));
//        _strategyIdToItemMap[strategyId]->update(QString::number(totalSold), StrategyModelTotalSold));
//        //_strategyIdToItemMap[strategyId]->updateTotalCommission()
//    }
//    else
//    {
//        StrategyViewItem* newItem = addItemInView();
//         newItem->setStrategyId(strategyId);
//        _strategyIdToItemMap[strategyId] = newItem;
//        _strategyIdToItemMap[strategyId]->update(StrategyManager::Instance()->getStrategyName(strategyId), StrategyModelStrategyName));
//        //_strategyIdToItemMap[strategyId]->updateStrategyID(strategyId);

//    }
//}

//void StrategyView::onTradeUpdate(const StrategyId strategyId, const int profitableTrades, const double netProfit)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(profitableTrades), StrategyModelWinningTrades));
//        _strategyIdToItemMap[strategyId]->update(QString::number(netProfit), StrategyModelNetProfit));
//    }
//}

//void StrategyView::updateTrades(const StrategyId strategyId, const int trades, const int longTrades, const int shortTrades)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(trades), StrategyModelTotalTrades));
//    }
//}

void StrategyView::setupActions()
{   
      strategyMenu =  new QMenu("Strategy", this);

      stopStrategyAction = new QAction(tr("Stop Strategy"), this);
      connect(stopStrategyAction, SIGNAL(triggered()), this, SLOT(stopStrategy()));

      closePositions = new QAction(tr("Close All Positions"), this);
      connect(closePositions, SIGNAL(triggered()), this, SLOT(closeAllPositions()));

      strategyMenu->addAction(closePositions);
      strategyMenu->addAction(stopStrategyAction);

      connect(strategyMenu, SIGNAL(aboutToShow()), this, SLOT(updateContextMenu()));

      connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
      connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));

}

void StrategyView::stopStrategy()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    StrategyManager::strategyManager().stopStrategy(strategyId);
}

void StrategyView::closeAllPositions()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    StrategyManager::strategyManager().closeAllPositionsInStrategy(strategyId);
}

void StrategyView::updateContextMenu()
{

}

void StrategyView::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<TableCellItem<StrategyViewItem>* >(itemAt(event->x(), event->y()));

     if(_clickedItem)
     {
         event->accept();
         strategyMenu->exec(event->globalPos());
     }
}

void StrategyView::onRemoveHeader()
{
    removeHeader();
}

void StrategyView::onCustomizeHeader()
{
    //_dialog->show();
}

void StrategyView::modifyHeaders(const int column)
{
    modifyHeader(column);
}
