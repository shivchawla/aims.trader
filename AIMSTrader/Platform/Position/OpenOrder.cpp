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

OpenOrder::OpenOrder(const OrderId orderId, const Order& order, const Contract& contract)
                    :_orderId(orderId)
                    ,_order(order)
                    ,_contract(contract)
{}

OpenOrder::~OpenOrder()
{}


//no need to synchronize these fucntions as they have only one pount of entry
//only one thread can enter in it at one time
void OpenOrder::updateOrder(const Contract& contract, const Execution& execution)
{
    _execution = execution;
    emit orderUpdated(_orderId, execution);
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

void OpenOrder::setOrderStatus(const OrderStatus orderstatus)
{
    _orderStatus = orderstatus;
}

const OrderStatus OpenOrder::getOrderStatus()
{
    return _orderStatus;
}

