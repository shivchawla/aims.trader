#include "Platform/View/OpenOrderView.h"
#include <QTableWidgetItem>
#include "Platform/View/OpenOrderViewItem.h"
#include <iostream>
//#include "Platform/Enumerations/OrderStatus.h"
#include "Platform/Model/OpenOrderModel.h"
#include "AimsTraderDefs/typedefs.h"
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/StrategyManager.h"
#include "QDebug"

OpenOrderView::OpenOrderView(QWidget* parent = 0):TableView<OpenOrderView, OpenOrderViewItem, OpenOrderModel, OpenOrderModelColumn>(parent)
{
    setupActions();
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

OpenOrderViewItem* OpenOrderView::getOpenOrderViewItem(const OrderId orderId)
{
    if(_orderIdToItemMap.count(orderId)!=0)
    {
        return _orderIdToItemMap[orderId];
    }
    return NULL;
}

//OpenOrderView::~OpenOrderView()
//{}

//void OpenOrderView::onExecutionUpdate(const OrderId orderId, const long filledQuantity, const long pendingQuantity, const double avgFillPrice, const double lastFillPrice)
//{
//    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
//    if(openOrderViewItem)
//    {
//        openOrderViewItem->update(QString::number(filledQuantity), OpenOrderModelFilledQuantity));
//        openOrderViewItem->update(QString::number(pendingQuantity), OpenOrderModelRemainingQuantity));
//        openOrderViewItem->update(QString::number(avgFillPrice), OpenOrderModelAvgFillPrice));
//        openOrderViewItem->update(QString::number(lastFillPrice), OpenOrderModelLastFillPrice));
//    }
//}

void OpenOrderView::updateOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{
    //setSortingEnabled(false);
    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
    if(openOrderViewItem)
    {
        openOrderViewItem->update(uint(orderDetail.getFilledShares()), OpenOrderModelFilledQuantity);
        openOrderViewItem->update(uint(orderDetail.getPendingShares()), OpenOrderModelRemainingQuantity);
        openOrderViewItem->update(orderDetail.getAvgFillPrice(), OpenOrderModelAvgFillPrice);
        openOrderViewItem->update(orderDetail.getLastFillPrice(), OpenOrderModelLastFillPrice);

        openOrderViewItem->setOrderStatus(orderDetail.getOrderStatus());
        openOrderViewItem->update(getOrderStatusString(orderDetail.getOrderStatus()), OpenOrderModelOrderStatus);
        openOrderViewItem->update(orderDetail.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
        openOrderViewItem->update(orderDetail.getCommission(), OpenOrderModelCommission);
        showHideOrder(openOrderViewItem, tab);
    }

    //setSortingEnabled(true);
}


//void OpenOrderView::onStatusUpdate(const OrderId orderId, const String status)
//{
//    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
//    if(openOrderViewItem)
//    {
//        openOrderViewItem->update(status, OpenOrderModelOrderStatus));
//    }
//}

void OpenOrderView::onStatusUpdate(const OrderId orderId, const OrderStatus status, const int tab)
{
    //setSortingEnabled(false);
    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
    if(openOrderViewItem)
    {
        openOrderViewItem->setOrderStatus(status);
        openOrderViewItem->update(getOrderStatusString(status), OpenOrderModelOrderStatus);
        //openOrderViewItem->update(openOrder.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
        showHideOrder(openOrderViewItem, tab);
    }

    //setSortingEnabled(true);
}

//void OpenOrderView::addOrder(const OrderId orderId, const Order& order, const Contract& contract, const String& strategyName)
//{
//    OpenOrderViewItem* newItem  = addItemInView();
//    _orderIdToItemMap[orderId] = newItem;
//    newItem->setOrderId(orderId);

//    newItem->update(QString::number(orderId), OpenOrderModelOrderId));
//    newItem->update(QString::number(order.totalQuantity), OpenOrderModelTotalQuantity));
//    newItem->update(QString::number(order.totalQuantity), OpenOrderModelRemainingQuantity));
//    newItem->update("0", OpenOrderModelFilledQuantity));
//    newItem->update(QString::fromStdString(contract.secType), OpenOrderModelInstrumentType));
//    newItem->update(QString::fromStdString(contract.symbol), OpenOrderModelInstrumentSymbol));
//    newItem->update(QString::fromStdString(order.orderType), OpenOrderModelOrderType));
//    newItem->update(QString::fromStdString(order.action), OpenOrderModelAction));
//    newItem->update(strategyName, OpenOrderModelStrategy));
//}

void OpenOrderView::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{
    //setSortingEnabled(false);
    //OrderId orderId = openOrder.getOrderId();
    OpenOrderViewItem* newItem  = addItemInView();
    _orderIdToItemMap[orderId] = newItem;
    newItem->setOrderId(orderId);

    //TickerId tickerId = openOrder.getTickerId();
    QString strategyName = StrategyManager::strategyManager().getStrategyName(orderDetail.getStrategyId());

    //Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);

    newItem->update(uint(orderId), OpenOrderModelOrderId);
    newItem->update(0, OpenOrderModelTotalQuantity);
    newItem->update(uint(orderDetail.getOrder().totalQuantity), OpenOrderModelRemainingQuantity);
    //newItem->update("0", OpenOrderModelFilledQuantity));
    newItem->update(orderDetail.getContract().secType, OpenOrderModelInstrumentType);
    newItem->update(orderDetail.getContract().symbol, OpenOrderModelInstrumentSymbol);
    newItem->update(orderDetail.getOrder().orderType, OpenOrderModelOrderType);
    newItem->update(orderDetail.getOrder().action, OpenOrderModelAction);
    newItem->update(strategyName, OpenOrderModelStrategy);
    newItem->update(orderDetail.getPlacedTime(), OpenOrderModelPlacedDate);
    newItem->update(orderDetail.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
    newItem->update(orderDetail.getOrder().goodTillDate, OpenOrderModelGoodTillDate);
    newItem->update(orderDetail.getCommission(), OpenOrderModelCommission);

    QString instrumentType("EQ");
    newItem->update(instrumentType, OpenOrderModelInstrumentType);
    newItem->update(orderDetail.getContract().exchange, OpenOrderModelExchange);

    showHideOrder(newItem, tab);
}

void OpenOrderView::removeOrder(const OrderId orderId)
{
    if(_orderIdToItemMap.count(orderId))
    {
        OpenOrderViewItem* item  = _orderIdToItemMap[orderId];
        int rowNum = row(item->getTableItem(0));
         _orderIdToItemMap.erase(orderId);
        removeRow(rowNum);
        _numRows--;
    }
}

void OpenOrderView::setupActions()
{
    _openOrderMenu = new QMenu("OpenOrderA", this);
    _cancel = new QAction(tr("Cancel Order"), this);
    connect(_cancel, SIGNAL(triggered()), this, SLOT(cancelOrder()));
    _cancelReplace = new QAction(tr("Cancel/Replace Order"), this);
    connect(_cancelReplace, SIGNAL(triggered()), this, SLOT(cancelReplaceOrder()));

    _openOrderMenu->addAction(_cancel);
    _openOrderMenu->addAction(_cancelReplace);

    _orderEntryDialog = new OrderEntryDialog(this);

    connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));
}

void OpenOrderView::cancelOrder()
{
    OrderId orderId = _clickedItem->parent()->getOrderId();
    Service::service().getOrderManager()->cancelOrder(orderId);
}

void OpenOrderView::cancelReplaceOrder()
{
    OrderId orderId = _clickedItem->parent()->getOrderId();
    Order order = Service::service().getOrderManager()->getOrder(orderId);
    TickerId tickerId = -1;
    _orderEntryDialog->setupDialog(tickerId, order);
}

void OpenOrderView::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<TableCellItem<OpenOrderViewItem> *>(itemAt(event->x(), event->y()));
    if(_clickedItem)
    {
        //String orderStatus = _clickedItem->parent()->getColumnText(OpenOrderModelOrderStatus));
        OrderStatus orderStatus = _clickedItem->parent()->getOrderStatus();
        if(orderStatus == FullyFilled || orderStatus == Canceled)
        {
            _cancel->setDisabled(true);
            _cancelReplace->setDisabled(true);
        }
        event->accept();
        _openOrderMenu->exec(event->globalPos());
    }
}

void OpenOrderView::onRemoveHeader()
{
    removeHeader();
}

void OpenOrderView::onCustomizeHeader()
{
     //_dialog->show();
}

void OpenOrderView::modifyHeaders(const int column)
{
    modifyHeader(column);
    for (int i=0;i<_numRows;++i)
    {
        qDebug()<<_viewItems[i]->getColumnText(column);
    }
}

void OpenOrderView::showAllOrders()
{
    setSortingEnabled(false);
    OpenOrderMapIterator end = _orderIdToItemMap.end();
    OpenOrderMapIterator it;

    for(it = _orderIdToItemMap.begin() ; it!=end ; ++it)
    {
        OpenOrderViewItem* item = it->second;
        int rowNum = item->getRow();
        showRow(rowNum);
    }
    setSortingEnabled(true);
}

void OpenOrderView::showOpenOrders()
{
    OpenOrderMapIterator end = _orderIdToItemMap.end();
    OpenOrderMapIterator it;

    for(it = _orderIdToItemMap.begin() ; it!=end ; ++it)
    {
        OpenOrderViewItem* item = it->second;
        int rowNum = item->getRow();

        if(item->getOrderStatus() == Canceled | item->getOrderStatus() == FullyFilled)
        {
            hideRow(rowNum);
        }
        else
        {
            showRow(rowNum);
        }
    }
}

void OpenOrderView::showCanceledOrders()
{
    OpenOrderMapIterator end = _orderIdToItemMap.end();
    OpenOrderMapIterator it;

    for(it = _orderIdToItemMap.begin() ; it!=end ; ++it)
    {
        OpenOrderViewItem* item = it->second;
        int rowNum = item->getRow();

        if(item->getOrderStatus()==Canceled)
        {
            showRow(rowNum);
        }
        else
        {
             hideRow(rowNum);
        }
    }
}

void OpenOrderView::showHideOrder(OpenOrderViewItem* item, const int tab)
{
    int rowNum = item->getRow();
    switch(tab)
    {
        case 1:
        {
            if( rowNum!=-1 && item->getOrderStatus() != Canceled )
            {
                hideRow(rowNum);
            }

            break;
        }

        case 0:
        {
             if(rowNum!=-1 && item->getOrderStatus() != Canceled && item->getOrderStatus() != FullyFilled )
             {
                hideRow(rowNum);
             }

             break;
        }
    }
}


