/*
 *  OpenOrder.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#ifndef OpenOrder_h
#define OpenOrder_h

#include "Platform/Shared/Order.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/OrderStatus.h"
#include <QObject>
#include <QMutex>

class Strategy;

//This class keep a track of open order related information 
//Like corresponding strategy, average fill price etc 

class OpenOrder//: public QObject
{
    //Q_OBJECT
	private:  
        OrderId _orderId;
		Order _order;
        //Execution _execution;
        long _filledShares;
        long _pendingShares;
        double _avgFillPrice;
        double _lastFillPrice;
        OrderStatus _status;
        TickerId _tickerId;
        bool _isClosingOrder;
        Contract _contract;
        QMutex mutex;

    public:
        OpenOrder(){}
        OpenOrder(const OrderId, const Order&, const TickerId);
		~OpenOrder();
    
    public:
        void setOrderStatus(const OrderStatus orderStatus);
	
	//Properties
	public:
        const OrderId getOrderId() const {return _orderId;}
        const TickerId getTickerId() const {return _tickerId;}
        const Order& getOrder() const {return _order;}
        const String getOrderStatusString() const;// {return _status;}
        const OrderStatus getOrderStatus() const;// {return _status;}
        const Contract& getContract() const {return _contract;}
        const long getFilledShares() const {return _filledShares;}
        const long getPendingShares() const {return _pendingShares;}
        const double getAvgFillPrice() const {return _avgFillPrice;}
        const double getLastFillPrice() const {return _lastFillPrice;}
        const bool IsClosingOrder() const
        {
            return _isClosingOrder;
        }
	
//    signals:
//        void orderUpdated(const OrderId, const ExecutionStatus&, const bool);
//        void orderUpdated(const OrderId, const ExecutionStatus&);
//        void statusUpdated(const OrderId, const ExecutionStatus&);

    public:
        void updateOrder(/*const Contract&,*/ const Execution&);
        void reset();

    public:
        void setIsClosingOrder()
        {
            _isClosingOrder = true;
        }
};
#endif
