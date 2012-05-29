/*
 *  OpenOrder.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/OpenOrder.h"
#include "Platform/Strategy/Strategy.h"

OpenOrder::OpenOrder(const OrderId orderId, const Order& order, const TickerId tickerId)
                    :_orderId(orderId)
                    ,_order(order)
                    ,_tickerId(tickerId)
{
    mutex.lock();
    _status = PendingSubmit;
    mutex.unlock();
}

OpenOrder::~OpenOrder()
{}

///Updates an openorder with new execution information
void OpenOrder::updateOrder(/*const Contract& contract,*/ const Execution& execution)
{
    mutex.lock();
    //_execution = execution;
    _lastFillPrice = execution.avgPrice;
    _avgFillPrice = (_avgFillPrice*_filledShares + execution.shares*_lastFillPrice)/(_filledShares += execution.shares);

    //_filledShares = execution.cumQty;
    _pendingShares = _order.totalQuantity - _filledShares;
   // orderedShares = _order.totalQuantity;
    if(_pendingShares==0)
    {
        _status = FullyFilled;
    }

    //emits a signal to GUI
    //emit orderUpdated(_orderId, _executionStatus, _isClosingOrder);
    mutex.unlock();
}

//no need to synchronize these fucntions as they have only one pount of entry
//only one thread can enter in it at one time
/*void OpenOrder::updateOrder(const std::string status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string whyHeld)
{
    //lock the order enitities
    _sharesFilled = filled;
    executionStatus.avgFillPrice = avgFillPrice;
    executionStatus.remaining = remaining;
    executionStatus.lastFillPrice = lastFillPrice;
    
    if(status=="PendingSubmit")
    {
        executionStatus.orderStatus=PendingSubmit;
    }
    else if(status=="PendingCancel")
    {
        executionStatus.orderStatus=PendingCancel;
    }
    else if(status=="PreSubmitted")
    {
        executionStatus.orderStatus=PreSubmitted;
    }
    else if(status=="Submitted")
    {
        executionStatus.orderStatus = Submitted;
    }
    else if(status == "Cancelled")
    {
        executionStatus.orderStatus=Cancelled;
    }
    else if(status == "Filled")
    {
        executionStatus.orderStatus = FullyFilled;
    }
    else if(status == "Inactive")
    {
        executionStatus.orderStatus = Inactive;
    }

    omp_set_lock(&lockExecutionStatus);
    executionStatus = _executionStatus;
    omp_unset_lock(&lockExecutionStatus);
    _strategyWPtr->updatePosition(_orderId, executionStatus);
}*/

void OpenOrder::reset()
{
    _orderId=0;
}

///Updates the order status
void OpenOrder::setOrderStatus(const OrderStatus orderstatus)
{
    mutex.lock();
    _status = orderstatus;
    mutex.unlock();
    //emit statusUpdated(_orderId, _executionStatus);
}

const String OpenOrder::getOrderStatusString() const
{
    switch(_status)
    {
        case PendingSubmit: return "PendingSubmit"; break;
        case PendingCancel: return "PendingCancel";break;
        case PreSubmitted: return "PreSubmitted";break;
        case Submitted: return "Submitted";break;
        case Cancelled: return "Cancelled";break;
        case FullyFilled: return "FullyFilled";break;
        case Inactive: return "InActive"; break;
        case PartiallyFilled: return "PartiallyFilled";break;
        case ApiPending: return "ApiPending";break;
        case ApiCancelled:return "ApiCancelled";break;
    }
}

const OrderStatus OpenOrder::getOrderStatus() const
{
    return _status;
}

