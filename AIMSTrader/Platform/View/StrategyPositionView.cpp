#include "Platform/View/StrategyPositionView.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/StrategyPositionViewItem.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include <QAction>
#include <Qmenu>
#include "Platform/View/OrderEntryDialog.h"

StrategyPositionView::StrategyPositionView(QWidget* parent=0):TableView<StrategyPositionView, StrategyPositionViewItem, StrategyPositionModel, StrategyPositionModelColumn>(parent)
{
    setupActions();
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

StrategyPositionViewItem* StrategyPositionView::getPositionViewItem(const StrategyId strategyId, const PositionId positionId)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(positionId)!=0)
        {
            return _positionMap[strategyId][positionId];
        }
    }
    return NULL;
}

StrategyPositionView::~StrategyPositionView()
{}

void StrategyPositionView::update()
{}

void StrategyPositionView::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
      StrategyPositionViewItem* newItem = addItemInView();

      newItem->setStrategyId(strategyId);
      newItem->setTickerId(tickerId);
      _positionMap[strategyId][tickerId] = newItem;
      newItem->update(QString::number(strategyId), getViewColumn(StrategyPositionModelStrategy));
      //newItem->update(QString::number(positionId), PositionID);

      //get startegy name from strategyManager
      newItem->update(Service::Instance()->getInstrumentManager()->getInstrumentId(tickerId), getViewColumn(StrategyPositionModelInstrumentName));
      newItem->update(StrategyManager::Instance()->getStrategyName(strategyId), getViewColumn(StrategyPositionModelStrategy));
}

void StrategyPositionView::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

           item->update(QString::number(sharesBought), getViewColumn(StrategyPositionModelBuys));
           item->update(QString::number(sharesSold), getViewColumn(StrategyPositionModelSells));
           item->update(QString::number(netShares), getViewColumn(StrategyPositionModelNet));
           item->update(QString::number(avgBought), getViewColumn(StrategyPositionModelAvgBT));
           item->update(QString::number(avgSold), getViewColumn(StrategyPositionModelAvgSLD));
           item->update(QString::number(totalValueBought), getViewColumn(StrategyPositionModelTotalBT));
           item->update(QString::number(totalValueSold), getViewColumn(StrategyPositionModelTotalSLD));
           item->update(QString::number(totalValueBought-totalValueSold), getViewColumn(StrategyPositionModelNetTotal));
           item->update(QString::number(realizedPnl), getViewColumn(StrategyPositionModelRealizedPL));
           item->update(QString::number(runningPnl), getViewColumn(StrategyPositionModelUnRealizedPL));
           item->update(QString::number(realizedPnl+runningPnl), getViewColumn(StrategyPositionModelPL));
           item->update(QString::number(-totalValueBought+totalValueSold-totalCommision), getViewColumn(StrategyPositionModelNetInclCommission));
        }
   }
}

void StrategyPositionView::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
            StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];
            item->update(QString::number(pnl), getViewColumn(StrategyPositionModelPL));
            item->update(QString::number(runningPnl), getViewColumn(StrategyPositionModelUnRealizedPL));
        }
    }
}

void StrategyPositionView::removePosition(const StrategyId strategyId, const TickerId tickerId)
{
    if(_positionMap.count(strategyId))
    {
        if(_positionMap[strategyId].count(tickerId))
        {
            StrategyPositionViewItem* item  = _positionMap[strategyId][tickerId];
            int rowNum = row(item->getTableItem(0));
            _positionMap[strategyId].erase(tickerId);
            removeRow(rowNum);
            _numRows--;
        }
    }
}


void StrategyPositionView::setupActions()
{
    _orderEntryDialog = new OrderEntryDialog(this);

    closePositionAction = new QAction(tr("Close Position"), this);
    connect(closePositionAction, SIGNAL(triggered()), this, SLOT(closePosition()));

    positionMenu  = new QMenu("StrategyPosition", this);
    positionMenu->addAction(closePositionAction);
    splitMenu = new QMenu("Adjust Position", this);
    buyAction = new QAction(tr("Buy"), this);
    connect(buyAction, SIGNAL(triggered()), this, SLOT(buyPosition()));
    sellAction = new QAction(tr("Sell"), this);
    connect(sellAction, SIGNAL(triggered()), this, SLOT(sellPosition()));

    splitMenu->addAction(buyAction);
    splitMenu->addAction(sellAction);

    positionMenu->addAction(closePositionAction);
    positionMenu->addMenu(splitMenu);

    connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));

    connect(_orderEntryDialog, SIGNAL(accepted()), this, SLOT(placeOrderfromDialog()));

}

void StrategyPositionView::closePosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();
    //Tell Strategy Manager to close this position for this particular strategy
    StrategyManager::Instance()->closePosition(strategyId, tickerId);
}

void StrategyPositionView::buyPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    QString action("BUY");
    _orderEntryDialog->setupDialog(action, tickerId);
}

void StrategyPositionView::sellPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    QString action("SELL");
    _orderEntryDialog->setupDialog(action, tickerId);
}

void StrategyPositionView::updateContextMenu()
{

}

void StrategyPositionView::contextMenuEvent(QContextMenuEvent* event)
{
    _clickedItem = static_cast<TableCellItem<StrategyPositionViewItem> *>(itemAt(event->x(), event->y()));
    if(_clickedItem)
    {
        event->accept();
        positionMenu->exec(event->globalPos());
    }
}

void StrategyPositionView::onRemoveHeader()
{
    removeHeader();
}

void StrategyPositionView::onCustomizeHeader()
{
    // _dialog->show();
}

void StrategyPositionView::modifyHeaders(const int column)
{
    modifyHeader(column);
}

void StrategyPositionView::placeOrderfromDialog()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    Order o;
    int quantity = _orderEntryDialog->getQuantity();
    o.totalQuantity = std::abs(quantity);
    o.orderType = _orderEntryDialog->getOrderSide().toStdString();

    String orderSide = _orderEntryDialog->getOrderSide();
    if(orderSide == "BUY")
    {
        o.action = "BUY";
    }
    else if(orderSide == "SELL")
    {
        o.action = "SELL";
    }

    o.lmtPrice = _orderEntryDialog->getLimitPrice();
    o.referencePriceType=0;
    StrategyManager::Instance()->adjustPosition(strategyId, tickerId, o);
}


