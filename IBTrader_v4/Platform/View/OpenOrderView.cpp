#include "Platform/View/OpenOrderView.h"
#include <QTableWidgetItem>
#include "Platform/View/OpenOrderViewItem.h"
#include <iostream>
OpenOrderView::OpenOrderView():TableView<OpenOrderView>()
{
    _numRows=0;
    _numCols = OpenOrderViewItem::getNumItems();

    init();
}

void OpenOrderView::init()
{
    setRowCount(_numRows);
    setColumnCount(_numCols);
    _header<< "OpenOrderId"
            <<"Strategy"
            <<"SecurityType"
            <<"Symbol"
            <<"OrderStatus"
            <<"FilledQuantity"
            <<"RemainingQuantity"
            <<"TotalQuantity"
            <<"AvgFillPrice"
            <<"LastFillPrice"
            <<"DateTime";
     setHorizontalHeaderLabels(_header);
     Order order;
     addOpenOrder(1,order,"Shiv");
     addOpenOrder(2,order,"A");
     addOpenOrder(3,order,"B");
     addOpenOrder(4,order,"C");
     addOpenOrder(5,order,"D");
}

OpenOrderView::~OpenOrderView(){}

void OpenOrderView::addOpenOrder(const OrderId orderId, const Order& order, const String& strategyName)
{
    OpenOrderViewItem* openOrderViewItem = new OpenOrderViewItem();
    QTableWidgetItem* firstItem =  openOrderViewItem->getItemPointer();

    #pragma omp critical (OpenOrderViewMap)
    {
        _orderIdToItemMap[orderId] = firstItem;

    }
    int numItems = openOrderViewItem->getNumItems();
    #pragma omp critical(OpenOrderViewItem)
    {
        int row = _numRows++;
        insertRow(row);
        for(int i=0;i<numItems;++i)
        {
            setItem(row,i,firstItem+i);
        }
        //QString::number()
        item(row, OpenOrderView::OpenOrderId)->setText(QString::number(orderId));
        item(row, OpenOrderView::Strategy)->setText(QString::fromStdString(strategyName));
        item(row, OpenOrderView::TotalQuantity)->setText(QString::number(order.totalQuantity));
    }
}

void OpenOrderView::cancelOpenOrder(const OrderId orderId)
{
    QTableWidgetItem* orderIdItem;
    #pragma omp critical (OpenOrderViewMap)
    {
        if(_orderIdToItemMap.count(orderId))
        {
            orderIdItem = _orderIdToItemMap[orderId];
        }
    }

    OpenOrderModelColumn status = OrderStatus;

    #pragma omp critical(OpenOrderViewItem)
    {
        if(!orderIdItem)
        {
            int row = orderIdItem->row();
            QTableWidgetItem* it = item(row,status);
            it->setText("PendingCancel");
            it->setForeground(Qt::yellow);
        }
    }
}

void OpenOrderView::removeOpenOrder(const OrderId orderId)
{
    QTableWidgetItem* orderIdItem;
    #pragma omp critical (OpenOrderViewMap)
    {
        if(_orderIdToItemMap.count(orderId))
        {
            orderIdItem = _orderIdToItemMap[orderId];
        }
    }

    #pragma omp critical(OpenOrderViewItem)
    {
        if(!orderIdItem)
        {
            int row = orderIdItem->row();
            removeRow(row);
        }
    }
}

void OpenOrderView::updateOpenOrder(const OrderId orderId, const Contract& contract, const Execution& execution)
{}

void OpenOrderView::updateOpenOrder(const OrderId orderId, const String& status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const String& whyHeld)
{
    QTableWidgetItem* orderIdItem;
    #pragma omp critical (OpenOrderViewMap)
    {
        if(_orderIdToItemMap.count(orderId))
        {
            orderIdItem = _orderIdToItemMap[orderId];
        }
    }

    #pragma omp critical(OpenOrderViewItem)
    {
        if(!orderIdItem)
        {
            int row = orderIdItem->row();
            item(row, OpenOrderView::OrderStatus)->setText(QString::fromStdString(status));
            item(row, OpenOrderView::FilledQuantity)->setText(QString::number(filled));
            item(row, OpenOrderView::AvgFillPrice)->setText(QString::number(avgFillPrice));
            item(row, OpenOrderView::LastFillPrice)->setText(QString::number(lastFillPrice));
            item(row, OpenOrderView::RemainingQuantity)->setText(QString::number(remaining));
        }
    }
}
