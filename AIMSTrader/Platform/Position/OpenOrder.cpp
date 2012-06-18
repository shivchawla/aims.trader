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
    //mutex.lock();
    _status = PendingSubmit;
    _filledShares = 0;
    _pendingShares = order.totalQuantity;
    _avgFillPrice = 0;
    _lastFillPrice = 0;
    _isClosingOrder = 0;
    _lastFilledShares = 0;
}

OpenOrder::~OpenOrder()
{}

///Updates an openorder with new execution information
void OpenOrder::updateOrder(/*const Contract& contract,*/ const Execution& execution)
{
    _mutex.lock();
    //_execution = execution;
    _lastFillPrice = execution.price;
    _lastFilledShares = execution.shares;
    _avgFillPrice = (_avgFillPrice*_filledShares + _lastFilledShares*_lastFillPrice)/(_filledShares += _lastFilledShares);

    //_filledShares = execution.cumQty;
    _pendingShares = _order.totalQuantity - _filledShares;
   // orderedShares = _order.totalQuantity;
    if(_pendingShares==0)
    {
        _status = FullyFilled;
    }
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
    //emit statusUpdated(_orderId, _executionStatus);
}

const OrderStatus OpenOrder::getOrderStatus() const
{
    return _status;
}

