/*
 *  IndicatorManager.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Indicator/IndicatorManager.h"

IndicatorManager::IndicatorManager()
{}

IndicatorManager::~IndicatorManager()
{}

IndicatorManager::IndicatorManager(Strategy* strategySPtr):_strategyWPtr(strategySPtr)
{}

