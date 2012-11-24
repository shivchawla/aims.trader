#include "Platform/View/OpenOrderView3.h"
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"

OpenOrderView3::OpenOrderView3(QWidget* parent):TableView3<OpenOrderView3, OpenOrderViewItem2, OpenOrderModel>(parent)
{
   setupActions();
}

void OpenOrderView3::setupActions()
{
    _openOrderMenu = new QMenu("OpenOrderA", this);
    _cancel = new QAction(tr("Cancel Order"), this);
    connect(_cancel, SIGNAL(triggered()), this, SLOT(cancelOrder()));
    _cancelReplace = new QAction(tr("Cancel/Replace Order"), this);
    connect(_cancelReplace, SIGNAL(triggered()), this, SLOT(cancelReplaceOrder()));

    _openOrderMenu->addAction(_cancel);
    _openOrderMenu->addAction(_cancelReplace);

    _orderEntryDialog = new OrderEntryDialog(this);
    //connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(int)));

}

void OpenOrderView3::cancelOrder()
{
    OrderId orderId = _clickedItem->getOrderId();
    Service::service().getOrderManager()->cancelOrder(orderId);
}

void OpenOrderView3::cancelReplaceOrder()
{
    OrderId orderId = _clickedItem->getOrderId();
    //Order order = _model->getOrderDetail(_clickedIdx).getOrder();
    TickerId tickerId = -1;
    _orderEntryDialog->setupDialog(tickerId, Service::service().getOrderManager()->getOrder(orderId));
}

void OpenOrderView3::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenuHelper(event);
    if(_clickedItem)
    {
        OrderStatus orderStatus = _clickedItem->getOrderStatus();
        if(orderStatus == FullyFilled || orderStatus == Canceled)
        {
            _cancel->setDisabled(true);
            _cancelReplace->setDisabled(true);
        }

        event->accept();
        _openOrderMenu->exec(event->globalPos());
    }
}

void OpenOrderView3::onRemoveHeader()
{
    removeHeader();
}

void OpenOrderView3::modifyHeaders(const int column)
{
    //modifyHeader(column);
}

void OpenOrderView3::showAllOrders()
{
    //_proxyModel->setFilterRegExp("");
}

void OpenOrderView3::showOpenOrders()
{
    //_proxyModel->setFilterRegExp("");
}

void OpenOrderView3::showCanceledOrders()
{
    //_proxyModel->setFilterRegExp(getOrderStatusString(Canceled));
}

//void OpenOrderView3::showHideOrder(const int tab)
//{
//    //_proxyModel->setFilterRegExp();
//}

void OpenOrderView3::updateOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{
    int row = _orderIdToRow.value(orderId, -1);

    if(row != -1)
    {
        //row = addRow(orderId);
//        update(uint(orderDetail.getFilledShares()), OpenOrderModelFilledQuantity);
//        update(uint(orderDetail.getPendingShares()), OpenOrderModelRemainingQuantity);
//        update(orderDetail.getAvgFillPrice(), OpenOrderModelAvgFillPrice);
//        update(orderDetail.getLastFillPrice(), OpenOrderModelLastFillPrice);
//        setOrderStatus(orderDetail.getOrderStatus());
//        update(getOrderStatusString(orderDetail.getOrderStatus()), OpenOrderModelOrderStatus);
//        update(orderDetail.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
//        update(orderDetail.getCommission(), OpenOrderModelCommission);
        //showHideOrder(openOrderViewItem, tab);
    }

    //setSortingEnabled(true);
}


void OpenOrderView3::onStatusUpdate(const OrderId orderId, const OrderStatus status, const int tab)
{
//    //setSortingEnabled(false);
//    OpenOrderViewItem* openOrderViewItem = getOpenOrderViewItem(orderId);
//    if(openOrderViewItem)
//    {
//        openOrderViewItem->setOrderStatus(status);
//        openOrderViewItem->update(getOrderStatusString(status), OpenOrderModelOrderStatus);
//        //openOrderViewItem->update(openOrder.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
//        showHideOrder(openOrderViewItem, tab);
//    }

//    //setSortingEnabled(true);
}

void OpenOrderView3::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{

//    int row = _orderIdToRow.value(orderId, -1);

//    if(row != -1)
//    {
//        int row = addRow();

//    }

//    //TickerId tickerId = openOrder.getTickerId();
//    QString strategyName = StrategyManager::strategyManager().getStrategyName(orderDetail.getStrategyId());

//    //Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);

//    newItem->update(uint(orderId), OpenOrderModelOrderId);
//    newItem->update(0, OpenOrderModelTotalQuantity);
//    newItem->update(uint(orderDetail.getOrder().totalQuantity), OpenOrderModelRemainingQuantity);
//    //newItem->update("0", OpenOrderModelFilledQuantity));
//    newItem->update(orderDetail.getContract().secType, OpenOrderModelInstrumentType);
//    newItem->update(orderDetail.getContract().symbol, OpenOrderModelInstrumentSymbol);
//    newItem->update(orderDetail.getOrder().orderType, OpenOrderModelOrderType);
//    newItem->update(orderDetail.getOrder().action, OpenOrderModelAction);
//    newItem->update(strategyName, OpenOrderModelStrategy);
//    newItem->update(orderDetail.getPlacedTime(), OpenOrderModelPlacedDate);
//    newItem->update(orderDetail.getLastUpdatedTime(), OpenOrderModelUpdatedDate);
//    newItem->update(orderDetail.getOrder().goodTillDate, OpenOrderModelGoodTillDate);
//    newItem->update(orderDetail.getCommission(), OpenOrderModelCommission);

//    QString instrumentType("EQ");
//    newItem->update(instrumentType, OpenOrderModelInstrumentType);
//    newItem->update(orderDetail.getContract().exchange, OpenOrderModelExchange);

//    showHideOrder(newItem, tab);
}

void OpenOrderView3::removeOrder(const OrderId orderId)
{
//    if(_orderIdToItemMap.count(orderId))
//    {
//        OpenOrderViewItem* item  = _orderIdToItemMap[orderId];
//        int rowNum = row(item->getTableItem(0));
//         _orderIdToItemMap.erase(orderId);
//        removeRow(rowNum);
//        _numRows--;
//    }
}

