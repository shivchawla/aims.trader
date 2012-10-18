#include "Platform/View/StrategyPositionView2.h"
#include "Platform/View/StrategyPositionModel2.h"
#include <QTableView>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QAction>
#include <Qmenu>
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Strategy/StrategyManager.h"

StrategyPositionView2::StrategyPositionView2(QWidget* parent):TableView2<StrategyPositionView2, StrategyPositionModel2>(parent)//, StrategyPositionModelColumn>(parent)
{
   setupActions();
}

void StrategyPositionView2::setupView()
{
    connect(_removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

void StrategyPositionView2::onRemoveHeader()
{
    removeHeader();
}

void StrategyPositionView2::onCustomizeHeader()
{
    //_dialog->show();
}

void StrategyPositionView2::modifyHeaders(const int column)
{
    modifyHeader(column);
}

void StrategyPositionView2::placeOrderfromDialog()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    TickerId tickerId = _model->getTickerId(_clickedIdx);

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

void StrategyPositionView2::contextMenuEvent(QContextMenuEvent* event)
{
    _clickedIdx = _proxyModel->mapToSource(indexAt(QPoint(event->x(), event->y())));

    // StrategyPositionView2* view = static_cast<StrategyPositionView2*>(itemAt(event->x(), event->y()));
    if(_clickedIdx!=QModelIndex())
    {
        bool isActive = _model->IsPositionActive(_clickedIdx);

        closePositionAction->setDisabled(!isActive);
        splitMenu->setDisabled(!isActive);

        event->accept();
        positionMenu->exec(event->globalPos());
    }
}

void StrategyPositionView2::setupActions()
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
    connect(_removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

void StrategyPositionView2::closePosition()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    TickerId tickerId = _model->getTickerId(_clickedIdx);
    //Tell Strategy Manager to close this position for this particular strategy
    StrategyManager::strategyManager().closePosition(strategyId, tickerId);
}

void StrategyPositionView2::buyPosition()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    TickerId tickerId = _model->getTickerId(_clickedIdx);

    Order order;
    order.action = "BUY";
    _orderEntryDialog->setupDialog(tickerId, order);
}

void StrategyPositionView2::sellPosition()
{
    StrategyId strategyId = _model->getStrategyId(_clickedIdx);
    TickerId tickerId = _model->getTickerId(_clickedIdx);
    Order order;
    order.action = "SELL";
    _orderEntryDialog->setupDialog(tickerId, order);
}


void StrategyPositionView2::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    _model->updatePositionForLastPrice(strategyId, tickerId, runningPnl, pnl);
}

void StrategyPositionView2::removePosition(const StrategyId strategyId, const TickerId tickerId)
{
    _model->removePosition(strategyId, tickerId);
}

void StrategyPositionView2::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    _model->updatePositionForExecution(strategyId, tickerId, positionDetail);
}

void StrategyPositionView2::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    _model->updatePositionForLastPrice(strategyId, tickerId, positionDetail);
}


void StrategyPositionView2::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    _model->addPosition(strategyId, tickerId);
}

