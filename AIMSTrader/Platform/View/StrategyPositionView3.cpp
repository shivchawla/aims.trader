#include "Platform/View/StrategyPositionView3.h"
#include "Platform/Model/StrategyPositionModel.h"
#include <QTableView>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QAction>
#include <Qmenu>
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

StrategyPositionView3::StrategyPositionView3(QWidget* parent):TableView3<StrategyPositionView3, StrategyPositionViewItem2, StrategyPositionModel>(parent)//, StrategyPositionModelColumn>(parent)
{
   setupActions();
}

void StrategyPositionView3::placeOrderfromDialog()
{
    StrategyId strategyId = _clickedItem->getStrategyId();
    TickerId tickerId = _clickedItem->getTickerId();

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

void StrategyPositionView3::contextMenuEvent(QContextMenuEvent* event)
{
    _clickedItem = static_cast<StrategyPositionViewItem2*>(itemAt(QPoint(event->x(), event->y())));

    if(_clickedItem != NULL)
    {
        bool isActive = true;// IsPositionActive();

        closePositionAction->setDisabled(!isActive);
        splitMenu->setDisabled(!isActive);

        event->accept();
        positionMenu->exec(event->globalPos());
    }
}

void StrategyPositionView3::setupActions()
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

    //connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    //connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));
    connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(int)));

    connect(_orderEntryDialog, SIGNAL(accepted()), this, SLOT(placeOrderfromDialog()));
    //connect(_removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

void StrategyPositionView3::closePosition()
{
    StrategyId strategyId = _clickedItem->getStrategyId();
    TickerId tickerId = _clickedItem->getTickerId();
    //Tell Strategy Manager to close this position for this particular strategy
    StrategyManager::strategyManager().closePosition(strategyId, tickerId);
}

void StrategyPositionView3::buyPosition()
{
    StrategyId strategyId = _clickedItem->getStrategyId();
    TickerId tickerId = _clickedItem->getTickerId();

    Order order;
    order.action = "BUY";
    _orderEntryDialog->setupDialog(tickerId, order);
}

void StrategyPositionView3::sellPosition()
{
    StrategyId strategyId = _clickedItem->getStrategyId();
    TickerId tickerId = _clickedItem->getTickerId();

    Order order;
    order.action = "SELL";
    _orderEntryDialog->setupDialog(tickerId, order);
}

void StrategyPositionView3::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    int row = _strategyTickerIdToRow.value(strategyId, QHash<TickerId,int>()).value(tickerId, -1);
    if(row != -1)
    {
        updateItemSpecial(row, StrategyPositionModelPL, pnl);
        updateItemSpecial(row, StrategyPositionModelRunningPL, runningPnl);
    }
}

void StrategyPositionView3::removePosition(const StrategyId strategyId, const TickerId tickerId)
{

}

void StrategyPositionView3::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    //setActive(positionDetail.getNetShares()!=0);
    int row = _strategyTickerIdToRow.value(strategyId, QHash<TickerId,int>()).value(tickerId, -1);
    if(row != -1)
    {
        updateItem(row, StrategyPositionModelBuys, (uint)positionDetail.getSharesBought());
        updateItem(row, StrategyPositionModelSells, (uint)positionDetail.getSharesSold());
        updateItem(row, StrategyPositionModelNet, (uint)positionDetail.getNetShares());
        updateItem(row, StrategyPositionModelAvgBT, positionDetail.getAvgBought());
        updateItem(row, StrategyPositionModelAvgSLD, positionDetail.getAvgSold());
        updateItem(row, StrategyPositionModelTotalBT, positionDetail.getTotalValueBought());
        updateItem(row, StrategyPositionModelTotalSLD, positionDetail.getTotalValueSold());
        updateItem(row, StrategyPositionModelNetTotal, positionDetail.getNetValue());
        updateItemSpecial(row, StrategyPositionModelRealizedPL, positionDetail.getRealizedPnl());
        updateItemSpecial(row, StrategyPositionModelRunningPL, positionDetail.getRunningPnl());
        updateItem(row, StrategyPositionModelNetInclCommission, positionDetail.getNetTotalIncCommission());
        updateItem(row, StrategyPositionModelLastPrice, positionDetail.getMarkedPrice());
        updateItem(row, StrategyPositionModelCommission, positionDetail.getTotalCommission());
    }
}

void StrategyPositionView3::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    int row = _strategyTickerIdToRow.value(strategyId, QHash<TickerId,int>()).value(tickerId, -1);
    if(row != -1)
    {
        updateItemSpecial(row, StrategyPositionModelPL, positionDetail.getNetPnL());
        updateItemSpecial(row, StrategyPositionModelRunningPL, positionDetail.getRunningPnl());
    }
}

void StrategyPositionView3::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    int row = _strategyTickerIdToRow.value(strategyId, QHash<TickerId,int>()).value(tickerId, -1);

    if(row == -1)
    {
        row = addRow(strategyId,tickerId);
        //get startegy name from strategyManager
        updateItem(row, StrategyPositionModelInstrumentName, Service::service().getInstrumentManager()->getSymbol(tickerId));
        updateItem(row, StrategyPositionModelStrategy, StrategyManager::strategyManager().getStrategyName(strategyId));
    }
}

int StrategyPositionView3::addRow(const StrategyId strategyId, const TickerId tickerId)
{
    int row = _numRows++;
    _strategyTickerIdToRow[strategyId][tickerId] = row;

    insertRow(row);
    for(int i=0;i<_numCols;++i)
    {
        insertItem(row, i, new StrategyPositionViewItem2(strategyId, tickerId));
    }

    return row;
}

