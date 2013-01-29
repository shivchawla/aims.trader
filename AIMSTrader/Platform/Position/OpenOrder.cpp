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

OpenOrder::OpenOrder(const OrderId orderId, const StrategyId strategyId, const Order& order, const Contract& contract)
                    :_orderId(orderId)
//                    ,_tickerId(tickerId)
//                    ,_strategyId(strategyId)
                    ,_orderDetail(strategyId, order, contract)
{}

OpenOrder::OpenOrder(const OrderId orderId, const StrategyId strategyId, const Order& order)
                    :_orderId(orderId)
//                    ,_tickerId(tickerId)
//                    ,_strategyId(strategyId)
                    ,_orderDetail(strategyId, order)
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
    filledShares = 0;
    lastFilledShares = 0;
    pendingShares = 0;
    avgFillPrice = 0;
    lastFillPrice = 0;
    status = PendingSubmit;
    isClosingOrder = 0;
    placedTime = QDateTime();
    lastUpdatedTime = QDateTime();
    commission = 0;
    strategyId = 0;
    //tickerId = 0;
    isSpreadOrder = false;
}

OrderDetail::OrderDetail(const OrderDetail& detail)
{
    order = detail.order;
    filledShares = detail.filledShares;
    lastFilledShares = detail.lastFilledShares;
    pendingShares = detail.pendingShares;
    avgFillPrice = detail.avgFillPrice;
    lastFillPrice = detail.lastFillPrice;
    status = detail.status;
    isClosingOrder = detail.isClosingOrder;
    placedTime = detail.placedTime;
    lastUpdatedTime = detail.lastUpdatedTime;
    contract = detail.contract;
    commission = detail.commission;
    strategyId = detail.strategyId;
    //tickerId = detail.tickerId;
    isSpreadOrder = detail.isSpreadOrder;
}

OrderDetail::OrderDetail(const StrategyId strategyId, const Order& order, const Contract& contract)
{
    this->order = order;
    status = PendingSubmit;
    filledShares = 0;
    pendingShares = order.totalQuantity;
    avgFillPrice = 0;
    lastFillPrice = 0;
    isClosingOrder = 0;
    lastFilledShares = 0;
    placedTime = lastUpdatedTime = QDateTime::currentDateTime();
    commission = 0;
    this->contract = contract;
    //this->tickerId = tickerId;
    this->strategyId = strategyId;
    isSpreadOrder  = false;
    if(contract.comboLegs)
    {
        isSpreadOrder = contract.comboLegs->size() > 1 ? true : false;
    }
}

OrderDetail::OrderDetail(const StrategyId strategyId, const Order& order)
{
    this->order = order;
    status = PendingSubmit;
    filledShares = 0;
    pendingShares = order.totalQuantity;
    avgFillPrice = 0;
    lastFillPrice = 0;
    isClosingOrder = 0;
    lastFilledShares = 0;
    placedTime = lastUpdatedTime = QDateTime::currentDateTime();
    commission = 0;
    //this->tickerId = tickerId;
    this->strategyId = strategyId;
    isSpreadOrder  = false;
}


void OrderDetail::update(const Execution& execution)
{
    lastFillPrice = execution.price;
    lastFilledShares = execution.shares;
    avgFillPrice = (avgFillPrice*filledShares + lastFilledShares*lastFillPrice)/(filledShares += lastFilledShares);
    pendingShares = order.totalQuantity - filledShares;
    if(pendingShares==0)
    {
        status = FullyFilled;
    }
    lastUpdatedTime = QDateTime::fromString( QString::fromStdString(execution.time),Qt::ISODate);

    commission = CommissionFactory::getNorthAmericaStockCommission().getCommission(filledShares, avgFillPrice, PriceBased);
}


