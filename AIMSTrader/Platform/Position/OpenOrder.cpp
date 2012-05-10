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
<<<<<<< HEAD
                    ,_tickerId(tickerId)
{
    //mutex.lock();
    _executionStatus.orderStatus = PendingSubmit;
    _isClosingOrder = false;
    //mutex.unlock();
=======
                    ,_contract(contract)
{
    mutex.lock();
    _executionStatus.orderStatus = PendingSubmit;
    mutex.unlock();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

OpenOrder::~OpenOrder()
{}

<<<<<<< HEAD
///Updates an openorder with new execution information
void OpenOrder::updateOrder(/*const Contract& contract,*/ const Execution& execution)
=======
//no need to synchronize these fucntions as they have only one pount of entry
//only one thread can enter in it at one time
void OpenOrder::updateOrder(const Contract& contract, const Execution& execution)
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    mutex.lock();
    _executionStatus.execution = execution;
    int filledShares = execution.cumQty;
    int orderedShares = _order.totalQuantity;
    if(filledShares==orderedShares)
    {
        _executionStatus.orderStatus=FullyFilled;
    }
<<<<<<< HEAD



    //emits a signal to GUI
    emit orderUpdated(_orderId, _executionStatus, _isClosingOrder);
=======
    emit orderUpdated(_orderId, _executionStatus);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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
    _executionStatus.orderStatus = orderstatus;
    mutex.unlock();
    emit statusUpdated(_orderId, _executionStatus);
}

const OrderStatus OpenOrder::getOrderStatus()
{
    return _executionStatus.orderStatus;
}

