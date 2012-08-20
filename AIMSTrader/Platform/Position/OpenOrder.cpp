/*
 *  OpenOrder.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/OpenOrder.h"

OpenOrder::OpenOrder(const OrderId orderId, const Order& order, const TickerId tickerId, const Contract& contract)
                    :_orderId(orderId)
                    ,_order(order)
                    ,_tickerId(tickerId)
                    ,_contract(contract)
{
    _status = PendingSubmit;
    _filledShares = 0;
    _pendingShares = order.totalQuantity;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _isClosingOrder = 0;
    _lastFilledShares = 0;
    _placedTime = _lastUpdatedTime = QDateTime::currentDateTime();

}

OpenOrder::OpenOrder(const OrderId orderId, const Order& order, const TickerId tickerId)
                    :_orderId(orderId)
                    ,_order(order)
                    ,_tickerId(tickerId)
{
    _status = PendingSubmit;
    _filledShares = 0;
    _pendingShares = order.totalQuantity;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _isClosingOrder = 0;
    _lastFilledShares = 0;
    _placedTime = _lastUpdatedTime = QDateTime::currentDateTime();
}


OpenOrder::OpenOrder(const OpenOrder& openOrder)
{
    _orderId = openOrder._orderId;
    _order = openOrder._order;
    _filledShares = openOrder._filledShares;
    _lastFilledShares = openOrder._lastFilledShares;
    _pendingShares = openOrder._pendingShares;
    _avgFillPrice = openOrder._avgFillPrice;
    _lastFillPrice = openOrder._lastFillPrice;
    _status = openOrder._status;
    _tickerId = openOrder._tickerId;
    //_instrumentId = openOrder._instrumentId;
    _isClosingOrder = openOrder._isClosingOrder;
    _lastUpdatedTime  = openOrder._lastUpdatedTime;
    _placedTime = openOrder._placedTime;
}


OpenOrder::~OpenOrder()
{}

///Updates an openorder with new execution information
void OpenOrder::updateOrder(/*const Contract& contract,*/ const Execution& execution)
{
    _mutex.lock();
    _lastFillPrice = execution.price;
    _lastFilledShares = execution.shares;
    _avgFillPrice = (_avgFillPrice*_filledShares + _lastFilledShares*_lastFillPrice)/(_filledShares += _lastFilledShares);
    _pendingShares = _order.totalQuantity - _filledShares;
    if(_pendingShares==0)
    {
        _status = FullyFilled;
    }
    _lastUpdatedTime = QDateTime::fromString( QString::fromStdString(execution.time),Qt::ISODate);
    _mutex.unlock();
}

void OpenOrder::reset()
{
    _orderId=0;
}

///Updates the order status
void OpenOrder::setOrderStatus(const OrderStatus orderstatus)
{
    _mutex.lock();
    _status = orderstatus;
    _mutex.unlock();
}

