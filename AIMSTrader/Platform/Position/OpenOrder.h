/*
 *  OpenOrder.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef OpenOrder_h
#define OpenOrder_h

#include "InteractiveBroker/Shared/Order.h"
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Execution.h"
#include "AimsTraderDefs/typedefs.h"
#include <QMutex>
#include "Data/InstrumentData.h"
#include <QDateTime>

//This class keep a track of open order related information 
//Like corresponding strategy, average fill price etc 

class OpenOrder
{
	private:  
        OrderId _orderId;
        OrderDetail _orderDetail;

        mutable QMutex _mutex;

    public:
        OpenOrder(){}
        OpenOrder(const OpenOrder&);
        OpenOrder(const OrderId, const StrategyId, const Order&, const Contract&);
        OpenOrder(const OrderId, const StrategyId, const Order&);

        ~OpenOrder();
    
    public:
        void setOrderStatus(const OrderStatus orderStatus);
//        void setIsClosingOrder()
//        {
//            _isClosingOrder = true;
//        }
	
	//Properties
	public:
        const OrderId getOrderId() const {return _orderId;}
//        const TickerId getTickerId() const {return _tickerId;}
//        const StrategyId getStrategyId() const {return _strategyId;}
        const OrderDetail getOrderDetail() const
        {
            _mutex.lock();
            OrderDetail orderDetail = _orderDetail;
            _mutex.unlock();

            return orderDetail;
        }
	
    public:
        void updateOrder(const Execution&);
        void reset();
};

#endif
