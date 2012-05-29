/*
 *  OrderStatus.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 10/4/11.
 *  Copyright 2011 AIMS inc. All rights reserved.
 *
 */

#ifndef OrderStatus_h
#define OrderStatus_h

enum OrderStatus
{
    None,
	PendingSubmit,
	PendingCancel,
	PreSubmitted,
	Submitted,
	Cancelled,
	FullyFilled,
	Inactive,
	PartiallyFilled,
	ApiPending,
	ApiCancelled
};



#endif
