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
#include <omp.h>
#include <QObject>
#include <QMutex>

class Strategy;

//This class keep a track of open order related information 
//Like corresponding strategy, average fill price etc 

class OpenOrder: public QObject
{
    Q_OBJECT
	private:  
        OrderId _orderId;
		Order _order;
        ExecutionStatus _executionStatus;
        TickerId _tickerId;
        QMutex mutex;
        bool _isClosingOrder;
	
	public:
        OpenOrder(){}
        OpenOrder(const OrderId, const Order&, const TickerId);
		~OpenOrder();
    
    public:
        void setOrderStatus(const OrderStatus orderStatus);
	
	//Properties
	public:
        const OrderId getOrderId();
		const Order& getOrder();
	
    signals:
        void orderUpdated(const OrderId, const ExecutionStatus&, const bool);
        void statusUpdated(const OrderId, const ExecutionStatus&);

    public slots:
        void updateOrder(/*const Contract&,*/ const Execution&);
        void reset();

    public:
        const OrderStatus getOrderStatus();
        const bool IsClosingOrder()
        {
            return _isClosingOrder;
        }

        void setIsClosingOrder()
        {
            _isClosingOrder = true;
        }
};
#endif
