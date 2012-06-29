//
//  StrategyReportManager.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//
#pragma once
#ifndef StrategyReportManager_h
#define StrategyReportManager_h
class Strategy;
class StrategyReport;

#include <list>
#include "AimsTraderDefs/typedefs.h"

class StrategyReportManager
{
    private:
     std::list<String> _strategyReportHeaders;

    private:
        Strategy* _strategyWPtr;
        StrategyReport* _strategyReportSPtr;
    

    public:
        StrategyReportManager(){}
        StrategyReportManager(Strategy* strategySPtr);
        ~StrategyReportManager();
    
    private:
        void initialize();
    
    public:
        void report();
        
            
        
    
    
};


#endif
