#include "Platform/View/OpenOrderView.h"
#include <QTableWidgetItem>
#include "Platform/View/OpenOrderViewItem.h"
#include <iostream>
#include "Platform/Enumerations/OrderStatus.h"

OpenOrderView::OpenOrderView(QWidget* parent = 0):TableView<OpenOrderView>(parent)
{
    _numRows=0;
    _numCols = OpenOrderViewItem::getNumItems();
    setOpenOrderView();
}

void OpenOrderView::setHeaders()
{
    _header<< "OpenOrderId"
            <<"Strategy"
            <<"SecurityType"
            <<"Symbol"
            <<"OrderStatus"
            <<"OrderType"
            <<"FilledQuantity"
            <<"RemainingQuantity"
            <<"TotalQuantity"
            <<"AvgFillPrice"
            <<"LastFillPrice"
            <<"DateTime";
     setHorizontalHeaderLabels(_header);
}

void OpenOrderView::setOpenOrderView()
{
    setMinimumSize(OpenOrderViewItem::getNumItems()*80, 200);
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();
    //resize based on header
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
        openOrderViewItem->update(QString::number(filledQuantity), FilledQuantity);
        openOrderViewItem->update(QString::number(avgFillPrice), AvgFillPrice);
        openOrderViewItem->update(QString::number(lastFillPrice), LastFillPrice);
        //openOrderViewItem->update(QString::fromStdString(executionStatus.execution.time), DateTime);
        //openOrderViewItem->update(QString::fromLatin1(executionStatus.orderStatusToStr()), OrderStatus);
    }
}

void OpenOrderView::onStatusUpdate(const OrderId orderId, const String status)
{
    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
    if(openOrderViewItem)
    {
        openOrderViewItem->update(status, OrderStatus);
    }
}

void OpenOrderView::addOrder(const OrderId orderId, const Order& order, const Contract& contract, const String& strategyName)
{
    OpenOrderViewItem* newItem  = new OpenOrderViewItem();
    _orderIdToItemMap[orderId] = newItem;

    int currentRow = _numRows++;
    insertRow(currentRow);
    int numItems = OpenOrderViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        setItem(currentRow,i,newItem->getTableItem(i));
    }

    newItem->update(QString::number(orderId),OpenOrderID);
    newItem->update(QString::number(order.totalQuantity), TotalQuantity);
    newItem->update("0", FilledQuantity);
    newItem->update(QString::fromStdString(contract.secType), SecurityType);
    newItem->update(QString::fromStdString(contract.symbol), Symbol);
    newItem->update(QString::fromStdString(order.orderType), OrderType);
    newItem->update(strategyName, Strategy);
}

void OpenOrderView::removeOrder(const OrderId orderId)
{
    if(_orderIdToItemMap.count(orderId))
    {
       // int numRows = rowCount();
        OpenOrderViewItem* item  = _orderIdToItemMap[orderId];
        int rowNum = row(item->getTableItem(0));
         _orderIdToItemMap.erase(orderId);
        removeRow(rowNum);
        _numRows--;
    }
}
