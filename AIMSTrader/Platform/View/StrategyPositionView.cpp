#include "Platform/View/StrategyPositionView.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/StrategyPositionViewItem.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include <QAction>
#include <Qmenu>
#include "Platform/View/OrderEntryDialog.h"
#include <QMessageBox>

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

void StrategyPositionView::update()
{}

void StrategyPositionView::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
      StrategyPositionViewItem* newItem =  _positionMap[strategyId][tickerId];

      if(!newItem)
         newItem = addItemInView();

      newItem->setStrategyId(strategyId);
      newItem->setTickerId(tickerId);

      _positionMap[strategyId][tickerId] = newItem;
      newItem->update(uint(strategyId), StrategyPositionModelStrategy);

      //get startegy name from strategyManager
      newItem->update(Service::service().getInstrumentManager()->getSymbol(tickerId), StrategyPositionModelInstrumentName);
      newItem->update(StrategyManager::strategyManager().getStrategyName(strategyId), StrategyPositionModelStrategy);
}

void StrategyPositionView::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

           item->setActive(positionDetail.getNetShares()!=0);

           item->update((int)positionDetail.sharesBought, StrategyPositionModelBuys);
           item->update((int)positionDetail.sharesSold, StrategyPositionModelSells);
           //long netShares = positionDetail.getNetShares();
           item->update((int)positionDetail.getNetShares(), StrategyPositionModelNet);
           item->update(positionDetail.avgBuyPrice, StrategyPositionModelAvgBT);
           item->update(positionDetail.avgSellPrice, StrategyPositionModelAvgSLD);
           item->update(positionDetail.totalValueBought, StrategyPositionModelTotalBT);
           item->update(positionDetail.totalValueSold, StrategyPositionModelTotalSLD);
           item->update(positionDetail.netValue, StrategyPositionModelNetTotal);
           item->updateSpecial(positionDetail.getRealizedPnl(), StrategyPositionModelRealizedPL);
           item->updateSpecial(positionDetail.getRunningPnl(), StrategyPositionModelRunningPL);
           //item->updateSpecial(positionDetail.getPnL(), StrategyPositionModelPL);
           item->update(positionDetail.getNetTotalIncCommission(), StrategyPositionModelNetInclCommission);
           item->update(positionDetail.getMarkedPrice(), StrategyPositionModelLastPrice);
           item->update(positionDetail.totalCommission, StrategyPositionModelCommission);
        }
   }
}

//void StrategyPositionView::updatePositionForExecution(const TickerId tickerId, const StrategyId strategyId, const PositionDetail& positionDetail)
//{
//    if(_positionMap.count(strategyId)!=0)
//    {
//        if(_positionMap[strategyId].count(tickerId)!=0)
//        {
//           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

//           item->setActive(positionDetail.getNetShares()!=0);

//           item->update(positionDetail.getSharesBought(), StrategyPositionModelBuys);
//           item->update(positionDetail.getSharesSold(), StrategyPositionModelSells);
//           item->update(positionDetail.getNetShares(), StrategyPositionModelNet);
//           item->update(positionDetail.getAvgBought(), StrategyPositionModelAvgBT);
//           item->update(positionDetail.getAvgSold(), StrategyPositionModelAvgSLD);
//           item->update(positionDetail.getTotalValueBought(), StrategyPositionModelTotalBT);
//           item->update(positionDetail.getTotalValueSold(), StrategyPositionModelTotalSLD);
//           item->update(positionDetail.getNetValue(), StrategyPositionModelNetTotal);
//           item->updateSpecial(positionDeatil.getRealizedPnl(), StrategyPositionModelRealizedPL);
//           item->updateSpecial(positionDetail.getRunningPnl(), StrategyPositionModelUnRealizedPL);
//           item->updateSpecial(positionDetail.getPnL(), StrategyPositionModelPL);
//           item->update(-positionDetail.getNetTotalIncCommission(), StrategyPositionModelNetInclCommission);
//           item->update(positionDetail.getMarkedPrice(), StrategyPositionModelLastPrice);
//           item->update(positionDetail.getTotalCommission(), StrategyPositionModelCommission);
//        }
//   }
//}


void StrategyPositionView::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
            StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];
            item->updateSpecial(pnl, StrategyPositionModelPL);
            item->updateSpecial(runningPnl, StrategyPositionModelRunningPL);
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
            int rowNum = row(item->getCellItem(0));
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
    if(!canModifyPosition(strategyId, tickerId))
    {
        QMessageBox messageBox;
        messageBox.setText("This position is a part of a Spread/Portfolio.\nCannot be closed");
        messageBox.setIcon(QMessageBox::Information);
        messageBox.exec();
        return;
    }
    //Tell Strategy Manager to close this position for this particular strategy
    StrategyManager::strategyManager().closePosition(strategyId, tickerId);
}

void StrategyPositionView::buyPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId(); 
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    if(!canModifyPosition(strategyId, tickerId))
    {
        QMessageBox messageBox;
        messageBox.setText("This position is a part of a Spread/Portfolio.\nCannot be bought");
        messageBox.setIcon(QMessageBox::Information);
        messageBox.exec();
        return;
    }

    Order order;
    order.action = "BUY";
    _orderEntryDialog->setupDialog(tickerId, order);
}

void StrategyPositionView::sellPosition()
{
    StrategyId strategyId = _clickedItem->parent()->getStrategyId();
    TickerId tickerId = _clickedItem->parent()->getTickerId();

    if(!canModifyPosition(strategyId, tickerId))
    {
        QMessageBox messageBox;
        messageBox.setText("This position is a part of a Spread/Portfolio.\nCannot be sold");
        messageBox.setIcon(QMessageBox::Information);
        messageBox.exec();
        return;
    }

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
        bool isActive = _clickedItem->parent()->IsActive();

        closePositionAction->setDisabled(!isActive);
        splitMenu->setDisabled(!isActive);

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

void StrategyPositionView::modifyHeaders(const int column)
{
    modifyHeader(column);
}

bool StrategyPositionView::canModifyPosition(const StrategyId strategyId, const TickerId tickerId)
{
    return StrategyManager::strategyManager().getStrategyType(strategyId) == SingleStock_StrategyType;
}
