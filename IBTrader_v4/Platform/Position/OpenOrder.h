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
class Strategy;

//This class keep a track of open order related information 
//Like corresponding strategy, average fill price etc 

class OpenOrder: public QObject
{
    Q_OBJECT
	private:  
        OrderId _orderId;
		Order _order;
        Execution _execution;
        OrderStatus _orderStatus;
        Contract _contract;
	
	public:
        OpenOrder(){}
        OpenOrder(const OrderId, const Order&, const Contract&);
		~OpenOrder();
    
    public:
        void setOrderStatus(const OrderStatus orderStatus);
	
	//Properties
	public:
        const OrderId getOrderId();
		const Order& getOrder();
	
    signals:
        void orderUpdated(const OrderId, const Execution&);

    public slots:
        void updateOrder(const Contract&, const Execution&);
        void reset();
    public:
        const OrderStatus getOrderStatus();
};
#endif
