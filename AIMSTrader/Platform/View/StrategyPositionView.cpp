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

StrategyPositionViewItem* StrategyPositionView::getPositionViewItem(const StrategyId strategyId, const InstrumentId instrumentId)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(instrumentId)!=0)
        {
            return _positionMap[strategyId][instrumentId];
        }
    }
    return NULL;
}

//StrategyPositionView::~StrategyPositionView()
//{}

void StrategyPositionView::update()
{}

void StrategyPositionView::addPosition(const StrategyId strategyId, const InstrumentId instrumentId)
{
      StrategyPositionViewItem* newItem = addItemInView();

      newItem->setStrategyId(strategyId);
      newItem->setInstrumentId(instrumentId);
      _positionMap[strategyId][instrumentId] = newItem;
      newItem->update(QString::number(strategyId), getViewColumn(StrategyPositionModelStrategy));
      //newItem->update(QString::number(positionId), PositionID);

      //get startegy name from strategyManager
      newItem->update(Service::service().getInstrumentManager()->getInstrumentContract(instrumentId).symbol, getViewColumn(StrategyPositionModelInstrumentName));
      newItem->update(StrategyManager::strategyManager().getStrategyName(strategyId), getViewColumn(StrategyPositionModelStrategy));
}

//void StrategyPositionView::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission)
//{
//    if(_positionMap.count(strategyId)!=0)
//    {
//        if(_positionMap[strategyId].count(tickerId)!=0)
//        {
//           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

//           item->update(QString::number(sharesBought), getViewColumn(StrategyPositionModelBuys));
//           item->update(QString::number(sharesSold), getViewColumn(StrategyPositionModelSells));
//           item->update(QString::number(netShares), getViewColumn(StrategyPositionModelNet));
//           item->update(QString::number(avgBought), getViewColumn(StrategyPositionModelAvgBT));
//           item->update(QString::number(avgSold), getViewColumn(StrategyPositionModelAvgSLD));
//           item->update(QString::number(totalValueBought), getViewColumn(StrategyPositionModelTotalBT));
//           item->update(QString::number(totalValueSold), getViewColumn(StrategyPositionModelTotalSLD));
//           item->update(QString::number(totalValueBought-totalValueSold), getViewColumn(StrategyPositionModelNetTotal));
//           item->updateSpecial(realizedPnl, getViewColumn(StrategyPositionModelRealizedPL));
//           item->updateSpecial(runningPnl, getViewColumn(StrategyPositionModelUnRealizedPL));
//           item->updateSpecial(realizedPnl+runningPnl, getViewColumn(StrategyPositionModelPL));
//           item->update(QString::number(-totalValueBought+totalValueSold-totalCommision), getViewColumn(StrategyPositionModelNetInclCommission));
//        }
//   }
//}

void StrategyPositionView::updatePositionForExecution(const Position& position)
{
    StrategyId strategyId  = position.getStrategyId();
    InstrumentId instrumentId = position.getInstrumentId();

    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(instrumentId)!=0)
        {
           StrategyPositionViewItem* item = _positionMap[strategyId][instrumentId];

           item->update(QString::number(position.getSharesBought()), getViewColumn(StrategyPositionModelBuys));
           item->update(QString::number(position.getSharesSold()), getViewColumn(StrategyPositionModelSells));
           item->update(QString::number(position.getNetShares()), getViewColumn(StrategyPositionModelNet));
           item->update(QString::number(position.getAvgBought()), getViewColumn(StrategyPositionModelAvgBT));
           item->update(QString::number(position.getAvgSold()), getViewColumn(StrategyPositionModelAvgSLD));
           item->update(QString::number(position.getTotalValueBought()), getViewColumn(StrategyPositionModelTotalBT));
           item->update(QString::number(position.getTotalValueSold()), getViewColumn(StrategyPositionModelTotalSLD));
           item->update(QString::number(position.getTotalValueBought() - position.getTotalValueSold()), getViewColumn(StrategyPositionModelNetTotal));
           item->updateSpecial(position.getRealizedPnl(), getViewColumn(StrategyPositionModelRealizedPL));
           item->updateSpecial(position.getRunningPnl(), getViewColumn(StrategyPositionModelUnRealizedPL));
           item->updateSpecial(position.getRealizedPnl() + position.getRunningPnl(), getViewColumn(StrategyPositionModelPL));
           item->update(QString::number(-position.getTotalValueBought() + position.getTotalValueSold() - position.getTotalCommission()), getViewColumn(StrategyPositionModelNetInclCommission));
        }
   }
}


void StrategyPositionView::updatePositionForLastPrice(const StrategyId strategyId, const InstrumentId instrumentId, const double runningPnl, const double pnl)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(instrumentId)!=0)
        {
            StrategyPositionViewItem* item = _positionMap[strategyId][instrumentId];
            item->updateSpecial(pnl, getViewColumn(StrategyPositionModelPL));
            item->updateSpecial(runningPnl, getViewColumn(StrategyPositionModelUnRealizedPL));
        }
    }
}

void StrategyPositionView::removePosition(const StrategyId strategyId, const InstrumentId instrumentId)
{
    if(_positionMap.count(strategyId))
    {
        if(_positionMap[strategyId].count(instrumentId))
        {
            StrategyPositionViewItem* item  = _positionMap[strategyId][instrumentId];
            int rowNum = row(item->getTableItem(0));
            _positionMap[strategyId].erase(instrumentId);
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
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();
    //Tell Strategy Manager to close this position for this particular strategy
    StrategyManager::strategyManager().closePosition(strategyId, instrumentId);
}

void StrategyPositionView::buyPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();

    Order order;
    order.action = "BUY";
    _orderEntryDialog->setupDialog(instrumentId, order);
}

void StrategyPositionView::sellPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();
    Order order;
    order.action = "SELL";
    _orderEntryDialog->setupDialog(instrumentId, order);
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
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();

    Order o;
    int quantity = _orderEntryDialog->getQuantity();
    o.totalQuantity = std::abs(quantity);

    OrderType orderType = _orderEntryDialog->getOrderType();
    if(orderType == MKT)
    {
        o.orderType = "MKT";
    }
    else if(orderType == LMT)
    {
        o.orderType = "LMT";
    }

    OrderSide orderSide = _orderEntryDialog->getOrderSide();
    if(orderSide == BUY)
    {
        o.action = "BUY";
    }
    else if(orderSide == SELL)
    {
        o.action = "SELL";
    }

    o.lmtPrice = _orderEntryDialog->getLimitPrice();
    o.referencePriceType=0;
    StrategyManager::strategyManager().adjustPosition(strategyId, instrumentId, o);
}


