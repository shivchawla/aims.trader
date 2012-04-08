//
//  StrategyReportManager.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Reports/StrategyReportManager.h"


StrategyReportManager::StrategyReportManager(Strategy* strategySPtr):_strategyWPtr(strategySPtr)
{
    initialize();
}

StrategyReportManager::~StrategyReportManager()
{}

void StrategyReportManager::initialize()
{
}

void StrategyReportManager::report()
{
}
