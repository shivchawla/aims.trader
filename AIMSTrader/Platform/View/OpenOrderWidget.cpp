#include "Platform/View/OpenOrderWidget.h"
#include "Platform/View/OpenOrderView.h"
#include <QAction>

OpenOrderWidget::OpenOrderWidget(QWidget* parent):QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    _openOrderView = new OpenOrderView(this);
    _tabBar = new QTabBar(this);
    _tabBar->addTab("Working Orders");
    _tabBar->addTab("Canceled Orders");
    _tabBar->addTab("All Orders");
    _tabBar->setCurrentIndex(2);

    layout->setSpacing(0);
    layout->setMargin(0);

    connect(_tabBar, SIGNAL(currentChanged(int)), this, SLOT(showOrders(int)));

    layout->addWidget(_tabBar);
    layout->addWidget(_openOrderView);
}


//void OpenOrderWidget::onExecutionUpdate(const OrderId orderId, const long filledQuantity, const long pendingQuantity, const double avgFillPrice, const double lastFillPrice)
//{
//    _openOrderView->onExecutionUpdate(orderId, filledQuantity, pendingQuantity, avgFillPrice, lastFillPrice);
//}

void OpenOrderWidget::updateOrder(const OpenOrder& openOrder)
{
    _openOrderView->updateOrder(openOrder);
}


//void OpenOrderWidget::addOrder(const OrderId orderId, const Order& order, const Contract& contract, const String& strategyName)
//{
//    _openOrderView->addOrder(orderId, order, contract, strategyName);
//}

void OpenOrderWidget::addOrder(const OpenOrder& openOrder, const String& strategyName)
{
    _openOrderView->addOrder(openOrder, strategyName);
}


void OpenOrderWidget::removeOrder(const OrderId orderId)
{
    _openOrderView->removeOrder(orderId);
}

//void OpenOrderWidget::onStatusUpdate(const OrderId orderId, const String status)
//{
//    _openOrderView->onStatusUpdate(orderId, status);
//}

void OpenOrderWidget::onStatusUpdate(const OrderId orderId, const OrderStatus status)
{
    _openOrderView->onStatusUpdate(orderId, status);
}

void OpenOrderWidget::showOrders(int x)
{
    switch(x)
    {
        case 0: _openOrderView->showOpenOrders(); break;
        case 1: _openOrderView->showCanceledOrders(); break;
        case 2: _openOrderView->showAllOrders(); break;
    }
}
