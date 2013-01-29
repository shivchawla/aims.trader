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

void OpenOrderWidget::updateOrder(const OrderId orderId, const OrderDetail& orderDetail)
{
    _openOrderView->updateOrder(orderId, orderDetail, _tabBar->currentIndex());
}

//void OpenOrderWidget::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const String& strategyName)
//{
//     _openOrderView->addOrder(orderId, orderDetail, strategyName, _tabBar->currentIndex());
//}

void OpenOrderWidget::addOrder(const OrderId orderId, const OrderDetail& orderDetail)
{
    _openOrderView->addOrder(orderId, orderDetail, _tabBar->currentIndex());
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
    _openOrderView->onStatusUpdate(orderId, status, _tabBar->currentIndex());
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
