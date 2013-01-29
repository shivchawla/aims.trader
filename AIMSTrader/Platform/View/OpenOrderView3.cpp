#include "Platform/View/OpenOrderView3.h"
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Strategy/StrategyManager.h"

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
    int row = getRow(orderId);
    if(row != -1)
    {
        updateRow(row, orderDetail);
        showHideOrder(row, tab);
    }
}

void OpenOrderView3::onStatusUpdate(const OrderId orderId, const OrderStatus status, const int tab)
{
    int row = getRow(orderId);
    if(row != -1)
    {
        for (int i=0;i <_numCols; ++i)
        {
            static_cast<OpenOrderViewItem2*>(item(row,i))->setOrderStatus(status);
        }

        //openOrderViewItem->setOrderStatus(status);
        updateItem(row, OpenOrderModelOrderStatus, getOrderStatusString(status)) ;
        showHideOrder(row, tab);
    }
}

void OpenOrderView3::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{
    int row = getRow(orderId);
    if(row == -1)
    {
        addRow(orderId, orderDetail);
        showHideOrder(row, tab);
    }
}

void OpenOrderView3::removeOrder(const OrderId orderId)
{
//    int row = getRow(orderId);
//    if(row == -1)
//    {
//        _orderIdToRow.erase(orderId);
//        removeRow(row);
//        _numRows--;
//    }
}

void OpenOrderView3::updateRow(const int row, const OrderDetail& orderDetail)
{
    updateItem(row, OpenOrderModelFilledQuantity, uint(orderDetail.getFilledShares()));
    updateItem(row, OpenOrderModelRemainingQuantity, uint(orderDetail.getPendingShares()));
    updateItem(row, OpenOrderModelAvgFillPrice, orderDetail.getAvgFillPrice());
    updateItem(row, OpenOrderModelLastFillPrice, orderDetail.getLastFillPrice());
    updateItem(row, OpenOrderModelOrderStatus, getOrderStatusString(orderDetail.getOrderStatus()));
    updateItem(row, OpenOrderModelUpdatedDate, orderDetail.getLastUpdatedTime());
    updateItem(row, OpenOrderModelCommission, orderDetail.getCommission());
}

void OpenOrderView3::addRow(const OrderId orderId, const OrderDetail& orderDetail)
{
    int row = _orderIdToRow[orderId] = _numRows++;
    QString strategyName = StrategyManager::strategyManager().getStrategyName(orderDetail.getStrategyId());
    updateItem(row, OpenOrderModelOrderId, uint(orderId));
    updateItem(row, OpenOrderModelTotalQuantity, 0);
    updateItem(row, OpenOrderModelRemainingQuantity, uint(orderDetail.getOrder().totalQuantity));
    updateItem(row, OpenOrderModelInstrumentType, QString::fromStdString(orderDetail.getContract().secType));
    updateItem(row, OpenOrderModelInstrumentSymbol, QString::fromStdString(orderDetail.getContract().symbol));
    updateItem(row, OpenOrderModelOrderType, QString::fromStdString(orderDetail.getOrder().orderType));
    updateItem(row, OpenOrderModelAction, QString::fromStdString(orderDetail.getOrder().action));
    updateItem(row, OpenOrderModelStrategy, strategyName);
    updateItem(row, OpenOrderModelPlacedDate, orderDetail.getPlacedTime());
    //update(row, OpenOrderModelUpdatedDate, orderDetail.getLastUpdatedTime());
    updateItem(row, OpenOrderModelGoodTillDate, QString::fromStdString(orderDetail.getOrder().goodTillDate));

    QString instrumentType("EQ");
    updateItem(row, OpenOrderModelInstrumentType, instrumentType);
    updateItem(row, OpenOrderModelExchange, QString::fromStdString(orderDetail.getContract().exchange));
}


void OpenOrderView3::showHideOrder(const int row, const int tab)
{
    if(row!=-1)
    {
        OpenOrderViewItem2* it = static_cast<OpenOrderViewItem2*>(item(row,0));
        OrderStatus status = it->getOrderStatus();
        switch(tab)
        {
            case 1:
            {
                if(status != Canceled )
                {
                    hideRow(row);
                }
                break;
            }

            case 0:
            {
                 if(status != Canceled && status != FullyFilled )
                 {
                    hideRow(row);
                 }
                 break;
            }
        }
    }
}

