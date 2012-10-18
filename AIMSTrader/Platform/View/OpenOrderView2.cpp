#include "Platform/View/OpenOrderView2.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"

OpenOrderView2::OpenOrderView2(QWidget* parent):TableView2<OpenOrderView2, OpenOrderModel2>(parent)
{
    _proxyModel->setFilterKeyColumn((int)OpenOrderModelOrderStatus);
}

void OpenOrderView2::setupActions()
{
    _openOrderMenu = new QMenu("OpenOrderA", this);
    _cancel = new QAction(tr("Cancel Order"), this);
    connect(_cancel, SIGNAL(triggered()), this, SLOT(cancelOrder()));
    _cancelReplace = new QAction(tr("Cancel/Replace Order"), this);
    connect(_cancelReplace, SIGNAL(triggered()), this, SLOT(cancelReplaceOrder()));

    _openOrderMenu->addAction(_cancel);
    _openOrderMenu->addAction(_cancelReplace);

    _orderEntryDialog = new OrderEntryDialog(this);
    connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(int)));

}

void OpenOrderView2::cancelOrder()
{
    OrderId orderId = _model->getOrderId(_clickedIdx);
    Service::service().getOrderManager()->cancelOrder(orderId);
}

void OpenOrderView2::cancelReplaceOrder()
{
    OrderId orderId = _model->getOrderId(_clickedIdx);
    Order order = _model->getOrderDetail(_clickedIdx).getOrder();
    TickerId tickerId = -1;
    _orderEntryDialog->setupDialog(tickerId, order);
}

void OpenOrderView2::contextMenuEvent(QContextMenuEvent *event)
{
    //_clickedIdx = indexAt(QPoint(event->x(), event->y()));
    _clickedIdx = _proxyModel->mapToSource(indexAt(QPoint(event->x(), event->y())));

    if(_clickedIdx!=QModelIndex())
    {
        OrderStatus orderStatus = _model->getOrderDetail(_clickedIdx).getOrderStatus();
        if(orderStatus == FullyFilled || orderStatus == Canceled)
        {
            _cancel->setDisabled(true);
            _cancelReplace->setDisabled(true);
        }

        event->accept();
        _openOrderMenu->exec(event->globalPos());
    }
}

void OpenOrderView2::onRemoveHeader()
{
    removeHeader();
}

void OpenOrderView2::onCustomizeHeader()
{
}

void OpenOrderView2::modifyHeaders(const int column)
{
    modifyHeader(column);
}

void OpenOrderView2::showAllOrders()
{
    _proxyModel->setFilterRegExp("");
}

void OpenOrderView2::showOpenOrders()
{
    _proxyModel->setFilterRegExp("");
}

void OpenOrderView2::showCanceledOrders()
{
    _proxyModel->setFilterRegExp(getOrderStatusString(Canceled));
}

void OpenOrderView2::showHideOrder(const int tab)
{
    //_proxyModel->setFilterRegExp();
}
