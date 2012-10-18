/*
 *  OpenOrder.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/OpenOrder.h"
#include "Platform/Commission/CommissionFactory.h"

OpenOrder::OpenOrder(const OrderId orderId, const TickerId tickerId, const StrategyId strategyId, const Order& order, const Contract& contract)
                    :_orderId(orderId)
//                    ,_tickerId(tickerId)
//                    ,_strategyId(strategyId)
                    ,_orderDetail(tickerId, strategyId, order, contract)
{}

OpenOrder::OpenOrder(const OrderId orderId, const TickerId tickerId, const StrategyId strategyId, const Order& order)
                    :_orderId(orderId)
//                    ,_tickerId(tickerId)
//                    ,_strategyId(strategyId)
                    ,_orderDetail(tickerId, strategyId, order)
{}


OpenOrder::OpenOrder(const OpenOrder& openOrder)
{
    _orderId = openOrder._orderId;
   /* _tickerId = openOrder._tickerId;
    _strategyId = openOrder._strategyId*/;
    _mutex.lock();
    _orderDetail = openOrder._orderDetail;
    _mutex.unlock();
}

OpenOrder::~OpenOrder()
{}

///Updates an openorder with new execution information
void OpenOrder::updateOrder(const Execution& execution)
{
    _mutex.lock();
    _orderDetail.update(execution);
    _mutex.unlock();
}

void OpenOrder::reset()
{
    _orderId=0;
//    _tickerId = 0;
//    _strategyId = 0;
}

///Updates the order status
void OpenOrder::setOrderStatus(const OrderStatus orderstatus)
{
    //_mutex.lock();
    //_status = orderstatus;
    //_mutex.unlock();
}

OrderDetail::OrderDetail()
{
    _filledShares = 0;
    _lastFilledShares = 0;
    _pendingShares = 0;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _status = PendingSubmit;
    _isClosingOrder = 0;
    _placedTime = QDateTime();
    _lastUpdatedTime = QDateTime();
    _commission = 0;
    _strategyId = 0;
    _tickerId = 0;
}

OrderDetail::OrderDetail(const OrderDetail& detail)
{
    _order = detail._order;
    _filledShares = detail._filledShares;
    _lastFilledShares = detail._lastFilledShares;
    _pendingShares = detail._pendingShares;
    _avgFillPrice = detail._avgFillPrice;
    _lastFillPrice = detail._lastFillPrice;
    _status = detail._status;
    _isClosingOrder = detail._isClosingOrder;
    _placedTime = detail._placedTime;
    _lastUpdatedTime = detail._lastUpdatedTime;
    _contract = detail._contract;
    _commission = detail._commission;
    _strategyId = detail._strategyId;
    _tickerId = detail._tickerId;
}

OrderDetail::OrderDetail(const TickerId tickerId, const StrategyId strategyId, const Order& order, const Contract& contract)
{
    _order = order;
    _status = PendingSubmit;
    _filledShares = 0;
    _pendingShares = order.totalQuantity;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _isClosingOrder = 0;
    _lastFilledShares = 0;
    _placedTime = _lastUpdatedTime = QDateTime::currentDateTime();
    _commission = 0;
    _contract = contract;
    _tickerId = tickerId;
    _strategyId = strategyId;
}

OrderDetail::OrderDetail(const TickerId tickerId, const StrategyId strategyId, const Order& order)
{
    _order = order;
    _status = PendingSubmit;
    _filledShares = 0;
    _pendingShares = order.totalQuantity;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _isClosingOrder = 0;
    _lastFilledShares = 0;
    _placedTime = _lastUpdatedTime = QDateTime::currentDateTime();
    _commission = 0;
    _tickerId = tickerId;
    _strategyId = strategyId;
}


void OrderDetail::update(const Execution& execution)
{
    _lastFillPrice = execution.price;
    _lastFilledShares = execution.shares;
    _avgFillPrice = (_avgFillPrice*_filledShares + _lastFilledShares*_lastFillPrice)/(_filledShares += _lastFilledShares);
    _pendingShares = _order.totalQuantity - _filledShares;
    if(_pendingShares==0)
    {
        _status = FullyFilled;
    }
    _lastUpdatedTime = QDateTime::fromString( QString::fromStdString(execution.time),Qt::ISODate);

    _commission = CommissionFactory::getNorthAmericaStockCommission().getCommission(_filledShares, _avgFillPrice, PriceBased);
}


