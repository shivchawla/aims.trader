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

//StrategyView::~StrategyView()
//{}

//void StrategyView::update()
//{}

//void StrategyView::onPerformanceUpdate()
//{

//}

void StrategyView::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    //qDebug()<<"StrategyId ="<<strategyId;
    if(_strategyIdToItemMap.count(strategyId)==0)
    {
        StrategyViewItem* newItem = addItemInView();
        newItem->setStrategyId(strategyId);
        _strategyIdToItemMap[strategyId] = newItem;
        _strategyIdToItemMap[strategyId]->update(StrategyManager::strategyManager().getStrategyName(strategyId), getViewColumn(StrategyModelStrategyName));
    }

    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.trades), getViewColumn(StrategyModelWinningTrades));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.totalBought), getViewColumn(StrategyModelTotalBought));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.totalSold), getViewColumn(StrategyModelTotalSold));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.profitableTrades), getViewColumn(StrategyModelWinningTrades));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.trades - performanceStats.profitableTrades), getViewColumn(StrategyModelLosingTrades));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.profitFactor), getViewColumn(StrategyModelProfitFactor));
    _strategyIdToItemMap[strategyId]->updateSpecial(performanceStats.unRealizedGrossPnL, getViewColumn(StrategyModelUnRealizedGrossPnL));
    _strategyIdToItemMap[strategyId]->updateSpecial(performanceStats.realizedGrossPnL, getViewColumn(StrategyModelRealizedGrossPnL));
    _strategyIdToItemMap[strategyId]->updateSpecial(performanceStats.netPnL, getViewColumn(StrategyModelNetPnL));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.drawDown), getViewColumn(StrategyModelDrawDown));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.maxDrawdown), getViewColumn(StrategyModelMaxDrawDown));
    _strategyIdToItemMap[strategyId]->update(QString::number(performanceStats.totalCommission), getViewColumn(StrategyModelTotalCommission));
    _strategyIdToItemMap[strategyId]->update(QString::number(0), getViewColumn(StrategyModelInvestment));

}

//void StrategyView::onExecutionUpdate(const StrategyId strategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(profitableTrades), getViewColumn(StrategyModelWinningTrades));
//        _strategyIdToItemMap[strategyId]->update(QString::number(totalBought), getViewColumn(StrategyModelTotalBought));
//        _strategyIdToItemMap[strategyId]->update(QString::number(totalSold), getViewColumn(StrategyModelTotalSold));
//        //_strategyIdToItemMap[strategyId]->updateTotalCommission()
//    }
//    else
//    {
//        StrategyViewItem* newItem = addItemInView();
//         newItem->setStrategyId(strategyId);
//        _strategyIdToItemMap[strategyId] = newItem;
//        _strategyIdToItemMap[strategyId]->update(StrategyManager::Instance()->getStrategyName(strategyId), getViewColumn(StrategyModelStrategyName));
//        //_strategyIdToItemMap[strategyId]->updateStrategyID(strategyId);

//    }
//}

//void StrategyView::onTradeUpdate(const StrategyId strategyId, const int profitableTrades, const double netProfit)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(profitableTrades), getViewColumn(StrategyModelWinningTrades));
//        _strategyIdToItemMap[strategyId]->update(QString::number(netProfit), getViewColumn(StrategyModelNetProfit));
//    }
//}

//void StrategyView::updateTrades(const StrategyId strategyId, const int trades, const int longTrades, const int shortTrades)
//{
//    if(_strategyIdToItemMap.count(strategyId)!=0)
//    {
//        _strategyIdToItemMap[strategyId]->update(QString::number(trades), getViewColumn(StrategyModelTotalTrades));
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
