/*
 *  ServerJob.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/22/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef ServerJob_h
#define ServerJob_h

enum Job
{
	
	PlaceOrder,
	CancelOrder,
	UpdateAccount,
	GetMarketData,
	CancelMarketData,
	ExecutionDetails
};

#endif