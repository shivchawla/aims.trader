#include "Platform/View/StrategyView3.h"
#include "Platform/Strategy/StrategyManager.h"

StrategyView3::StrategyView3(QWidget* parent) : TableView3<StrategyView3, StrategyViewItem2, StrategyModel>(parent)
{
    setupActions();
}

void StrategyView3::setupActions()
{
      strategyMenu =  new QMenu("Strategy", this);

      stopStrategyAction = new QAction(tr("Stop Strategy"), this);
      connect(stopStrategyAction, SIGNAL(triggered()), this, SLOT(stopStrategy()));

      closePositions = new QAction(tr("Close All Positions"), this);
      connect(closePositions, SIGNAL(triggered()), this, SLOT(closeAllPositions()));

      strategyMenu->addAction(closePositions);
      strategyMenu->addAction(stopStrategyAction);

      connect(strategyMenu, SIGNAL(aboutToShow()), this, SLOT(updateContextMenu()));
      connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(int)));
}

void StrategyView3::stopStrategy()
{
    StrategyId strategyId = static_cast<StrategyViewItem2*>(_clickedItem)->getStrategyId();
    StrategyManager::strategyManager().stopStrategy(strategyId);
}

void StrategyView3::closeAllPositions()
{
    StrategyId strategyId = static_cast<StrategyViewItem2*>(_clickedItem)->getStrategyId();
    StrategyManager::strategyManager().closeAllPositionsInStrategy(strategyId);
}

void StrategyView3::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<StrategyViewItem2*>(itemAt(QPoint(event->x(), event->y())));
    if(_clickedItem != NULL)
    {
         event->accept();
         strategyMenu->exec(event->globalPos());
    }
}

void StrategyView3::onRemoveHeader()
{
    removeHeader();
}

void StrategyView3::onCustomizeHeader()
{}

void StrategyView3::modifyHeaders(const int column)
{
    //modifyHeader(column);
}

void StrategyView3::updateContextMenu()
{}

int StrategyView3::addRow(const StrategyId strategyId)
{
    int row = _numRows++;
    _strategyIdToRow[strategyId] = row;

    insertRow(row);
    for(int i=0;i<_numCols;++i)
    {
        insertItem(row, i, new StrategyViewItem2(strategyId));
    }

    return row;
}

void StrategyView3::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
     int row = _strategyIdToRow.value(strategyId, -1);

    if(row == -1)
    {
        row = addRow(strategyId);
        updateItem(row, StrategyModelStrategyName, StrategyManager::strategyManager().getStrategyName(strategyId));
    }

    updateItem(row, StrategyModelStrategyName, StrategyManager::strategyManager().getStrategyName(strategyId));
    updateItem(row, StrategyModelStrategyName, StrategyManager::strategyManager().getStrategyName(strategyId));
    updateItem(row, StrategyModelProfit, performanceStats.totalProfit);
    updateItem(row,  StrategyModelLoss, performanceStats.totalLoss);
    updateItem(row, StrategyModelTotalTrades, performanceStats.trades);
    updateItem(row, StrategyModelTotalBought, performanceStats.totalBought);
    updateItem(row, StrategyModelTotalSold, performanceStats.totalSold);
    updateItem(row, StrategyModelWinningTrades, performanceStats.profitableTrades);
    updateItem(row, StrategyModelLosingTrades, performanceStats.trades - performanceStats.profitableTrades);
    updateItem(row, StrategyModelProfitFactor, performanceStats.profitFactor);
    updateItemSpecial(row, StrategyModelRunningGrossPnL, performanceStats.runningGrossPnL);
    updateItemSpecial(row, StrategyModelRealizedGrossPnL, performanceStats.realizedGrossPnL);
    updateItemSpecial(row, StrategyModelNetPnL, performanceStats.netPnL);
    updateItem(row, StrategyModelDrawDown, performanceStats.drawDown);
    updateItem(row, StrategyModelMaxDrawDown, performanceStats.maxDrawdown);
    updateItem(row, StrategyModelTotalCommission, performanceStats.totalCommission);
    updateItem(row, StrategyModelInvestment, 0);

}
