/*
 *  IndicatorManager.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef IndicatorManager_h
#define IndicatorManager_h

#include "Platform/Indicator/Indicator.h"
#include <list>

class Strategy;

class IndicatorManager
{
	private:
		Strategy* _strategyWPtr;
	
	private:
		std::list<Indicator> _indicators;

	public:
		IndicatorManager();
		IndicatorManager(Strategy*);
		~IndicatorManager();
	
};

#endif
