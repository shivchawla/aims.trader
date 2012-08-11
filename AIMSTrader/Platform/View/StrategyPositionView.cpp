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

StrategyPositionViewItem* StrategyPositionView::getPositionViewItem(const StrategyId strategyId, const TickerId tickerId)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
            return _positionMap[strategyId][tickerId];
        }
    }
    return NULL;
}

//StrategyPositionView::~StrategyPositionView()
//{}

void StrategyPositionView::update()
{}

void StrategyPositionView::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
      StrategyPositionViewItem* newItem = addItemInView();

      newItem->setStrategyId(strategyId);
      newItem->setTickerId(tickerId);
      _positionMap[strategyId][tickerId] = newItem;
      newItem->update(QString::number(strategyId), StrategyPositionModelStrategy);
      //newItem->update(QString::number(positionId), PositionID);

      //get startegy name from strategyManager
      newItem->update(Service::service().getInstrumentManager()->getSymbol(tickerId), StrategyPositionModelInstrumentName);
      newItem->update(StrategyManager::strategyManager().getStrategyName(strategyId), StrategyPositionModelStrategy);
}

//void StrategyPositionView::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission)
//{
//    if(_positionMap.count(strategyId)!=0)
//    {
//        if(_positionMap[strategyId].count(tickerId)!=0)
//        {
//           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

//           item->update(QString::number(sharesBought), StrategyPositionModelBuys));
//           item->update(QString::number(sharesSold), StrategyPositionModelSells));
//           item->update(QString::number(netShares), StrategyPositionModelNet));
//           item->update(QString::number(avgBought), StrategyPositionModelAvgBT));
//           item->update(QString::number(avgSold), StrategyPositionModelAvgSLD));
//           item->update(QString::number(totalValueBought), StrategyPositionModelTotalBT));
//           item->update(QString::number(totalValueSold), StrategyPositionModelTotalSLD));
//           item->update(QString::number(totalValueBought-totalValueSold), StrategyPositionModelNetTotal));
//           item->updateSpecial(realizedPnl, StrategyPositionModelRealizedPL));
//           item->updateSpecial(runningPnl, StrategyPositionModelUnRealizedPL));
//           item->updateSpecial(realizedPnl+runningPnl, StrategyPositionModelPL));
//           item->update(QString::number(-totalValueBought+totalValueSold-totalCommision), StrategyPositionModelNetInclCommission));
//        }
//   }
//}

void StrategyPositionView::updatePositionForExecution(const Position& position)
{
    StrategyId strategyId  = position.getStrategyId();
    TickerId tickerId = position.getTickerId();

    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

           item->update(QString::number(position.getSharesBought()), StrategyPositionModelBuys);
           item->update(QString::number(position.getSharesSold()), StrategyPositionModelSells);
           item->update(QString::number(position.getNetShares()), StrategyPositionModelNet);
           item->update(QString::number(position.getAvgBought()), StrategyPositionModelAvgBT);
           item->update(QString::number(position.getAvgSold()), StrategyPositionModelAvgSLD);
           item->update(QString::number(position.getTotalValueBought()), StrategyPositionModelTotalBT);
           item->update(QString::number(position.getTotalValueSold()), StrategyPositionModelTotalSLD);
           item->update(QString::number(position.getTotalValueBought() - position.getTotalValueSold()), StrategyPositionModelNetTotal);
           item->updateSpecial(position.getRealizedPnl(), StrategyPositionModelRealizedPL);
           item->updateSpecial(position.getRunningPnl(), StrategyPositionModelUnRealizedPL);
           item->updateSpecial(position.getRealizedPnl() + position.getRunningPnl(), StrategyPositionModelPL);
           item->update(QString::number(-position.getTotalValueBought() + position.getTotalValueSold() - position.getTotalCommission()), StrategyPositionModelNetInclCommission);
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
            item->updateSpecial(pnl, StrategyPositionModelPL);
            item->updateSpecial(runningPnl, StrategyPositionModelUnRealizedPL);
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
    StrategyManager::strategyManager().closePosition(strategyId, tickerId);
}

void StrategyPositionView::buyPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    Order order;
    order.action = "BUY";
    _orderEntryDialog->setupDialog(tickerId, order);
}

void StrategyPositionView::sellPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();
    Order order;
    order.action = "SELL";
    _orderEntryDialog->setupDialog(tickerId, order);
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
    StrategyManager::strategyManager().adjustPosition(strategyId, tickerId, o);
}


