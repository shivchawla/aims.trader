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

//This class keep a track of open order related information 
//Like corresponding strategy, average fill price etc 

class OpenOrder
{
	private:  
        OrderId _orderId;
		Order _order;
        long _filledShares;
        long _lastFilledShares;
        long _pendingShares;
        double _avgFillPrice;
        double _lastFillPrice;
        OrderStatus _status;
        //InstrumentId _instrumentId;
        TickerId _tickerId;
        bool _isClosingOrder;
        //InstrumentContract _instrumentContract;
        Contract _contract;
        QMutex _mutex;

    public:
        OpenOrder(){}
        OpenOrder(const OpenOrder&);
        OpenOrder(const OrderId, const Order&, const TickerId, const Contract&);
        OpenOrder(const OrderId, const Order&, const TickerId);

        ~OpenOrder();
    
    public:
        void setOrderStatus(const OrderStatus orderStatus);
        void setIsClosingOrder()
        {
            _isClosingOrder = true;
        }
	
	//Properties
	public:
        const OrderId getOrderId() const {return _orderId;}
        const TickerId getTickerId() const {return _tickerId;}
        //const InstrumentId getInstrumentId() const {return _instrumentId;}

        const Order& getOrder() const {return _order;}
        const OrderStatus getOrderStatus() const {return _status;}
        //const InstrumentContract& getInstrumentContract() const {return _instrumentContract;}


        const long getFilledShares() const {return _filledShares;}
        const long getPendingShares() const {return _pendingShares;}
        const double getAvgFillPrice() const {return _avgFillPrice;}
        const double getLastFillPrice() const {return _lastFillPrice;}
        const long getLastFilledShares() const {return _lastFilledShares;}
        const bool IsClosingOrder() const{ return _isClosingOrder; }
	
    public:
        void updateOrder(const Execution&);
        void reset();
};
#endif
