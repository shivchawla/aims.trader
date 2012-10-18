#include "Platform/View/StrategyView2.h"
#include "Platform/Strategy/StrategyManager.h"

StrategyView2::StrategyView2(QWidget* parent):TableView2<StrategyView2, StrategyModel2>(parent)
{
    setupActions();
}

void StrategyView2::setupActions()
{
      strategyMenu =  new QMenu("Strategy", this);

      stopStrategyAction = new QAction(tr("Stop Strategy"), this);
      connect(stopStrategyAction, SIGNAL(triggered()), this, SLOT(stopStrategy()));

      closePositions = new QAction(tr("Close All Positions"), this);
      connect(closePositions, SIGNAL(triggered()), this, SLOT(closeAllPositions()));

      strategyMenu->addAction(closePositions);
      strategyMenu->addAction(stopStrategyAction);

      connect(strategyMenu, SIGNAL(aboutToShow()), this, SLOT(updateContextMenu()));

      //connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
      //connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));
      connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(int)));

}

void StrategyView2::stopStrategy()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    StrategyManager::strategyManager().stopStrategy(strategyId);
}

void StrategyView2::closeAllPositions()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    StrategyManager::strategyManager().closeAllPositionsInStrategy(strategyId);
}

void StrategyView2::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedIdx = _proxyModel->mapToSource(indexAt(QPoint(event->x(), event->y())));

    if(_clickedIdx != QModelIndex())
     {
         event->accept();
         strategyMenu->exec(event->globalPos());
     }
}

void StrategyView2::onRemoveHeader()
{
    removeHeader();
}

void StrategyView2::onCustomizeHeader()
{}

void StrategyView2::modifyHeaders(const int column)
{
    modifyHeader(column);
}


void StrategyView2::updateContextMenu()
{

}

void StrategyView2::updatePerformance(const StrategyId strategyId, const PerformanceStats& stats)
{
    _model->updatePerformance(strategyId, stats);
}
