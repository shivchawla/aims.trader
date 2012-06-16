#include "Platform/View/OpenOrderView.h"
#include <QTableWidgetItem>
#include "Platform/View/OpenOrderViewItem.h"
#include <iostream>
//#include "Platform/Enumerations/OrderStatus.h"
#include "Platform/Model/OpenOrderModel.h"
#include "Platform/typedefs.h"

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

OpenOrderView::~OpenOrderView()
{}

void OpenOrderView::onExecutionUpdate(const OrderId orderId, const long filledQuantity, const long pendingQuantity, const double avgFillPrice, const double lastFillPrice)
{
    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
    if(openOrderViewItem)
    {
        openOrderViewItem->update(QString::number(filledQuantity), getViewColumn(OpenOrderModelFilledQuantity));
        openOrderViewItem->update(QString::number(pendingQuantity), getViewColumn(OpenOrderModelRemainingQuantity));
        openOrderViewItem->update(QString::number(avgFillPrice), getViewColumn(OpenOrderModelAvgFillPrice));
        openOrderViewItem->update(QString::number(lastFillPrice), getViewColumn(OpenOrderModelLastFillPrice));
    }
}

//void OpenOrderView::onStatusUpdate(const OrderId orderId, const String status)
//{
//    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
//    if(openOrderViewItem)
//    {
//        openOrderViewItem->update(status, getViewColumn(OpenOrderModelOrderStatus));
//    }
//}

void OpenOrderView::onStatusUpdate(const OrderId orderId, const OrderStatus status)
{
    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
    if(openOrderViewItem)
    {
        openOrderViewItem->setOrderStatus(status);
        openOrderViewItem->update(getOrderStatusString(status), getViewColumn(OpenOrderModelOrderStatus));
    }
}

void OpenOrderView::addOrder(const OrderId orderId, const Order& order, const Contract& contract, const String& strategyName)
{
    OpenOrderViewItem* newItem  = addItemInView();
    _orderIdToItemMap[orderId] = newItem;

    newItem->update(QString::number(orderId), getViewColumn(OpenOrderModelOrderId));
    newItem->update(QString::number(order.totalQuantity), getViewColumn(OpenOrderModelTotalQuantity));
    newItem->update(QString::number(order.totalQuantity), getViewColumn(OpenOrderModelRemainingQuantity));
    newItem->update("0", getViewColumn(OpenOrderModelFilledQuantity));
    newItem->update(QString::fromStdString(contract.secType), getViewColumn(OpenOrderModelInstrumentType));
    newItem->update(QString::fromStdString(contract.symbol), getViewColumn(OpenOrderModelInstrumentSymbol));
    newItem->update(QString::fromStdString(order.orderType), getViewColumn(OpenOrderModelOrderType));
    newItem->update(strategyName, getViewColumn(OpenOrderModelStrategy));
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

//    _openOrderMenuB = new QMenu("OpenOrderB", this);
//    _showAllOrders = new QAction("Show All Orders");
//    _showCanceledOrders = new QAction("Show");
//    QAction* _showOpenOrders;


    connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));
}

void OpenOrderView::cancelOrder()
{

}

void OpenOrderView::cancelReplaceOrder()
{

}

void OpenOrderView::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<TableCellItem<OpenOrderViewItem> *>(itemAt(event->x(), event->y()));
    if(_clickedItem)
    {
        String orderStatus = _clickedItem->parent()->getColumnText(getViewColumn(OpenOrderModelOrderStatus));
        if(orderStatus == "FullyFilled" || orderStatus =="Cancelled")
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
}

void OpenOrderView::showAllOrders()
{
    OpenOrderMapIterator end = _orderIdToItemMap.end();
    OpenOrderMapIterator it;

    for(it = _orderIdToItemMap.begin() ; it!=end ; ++it)
    {
        OpenOrderViewItem* item = it->second;
        int rowNum = row(item->getTableItem(0));
        showRow(rowNum);
    }
}

void OpenOrderView::showOpenOrders()
{
    OpenOrderMapIterator end = _orderIdToItemMap.end();
    OpenOrderMapIterator it;

    for(it = _orderIdToItemMap.begin() ; it!=end ; ++it)
    {
        OpenOrderViewItem* item = it->second;
        int rowNum = row(item->getTableItem(0));

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
        int rowNum = row(item->getTableItem(0));

        if(-item->getOrderStatus()==Canceled)
        {
            showRow(rowNum);
        }
        else
        {
             hideRow(rowNum);
        }
    }
}

