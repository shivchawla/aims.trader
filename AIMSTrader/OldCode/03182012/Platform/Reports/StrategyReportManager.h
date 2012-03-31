//
//  StrategyReportManager.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef StrategyReportManager_h
#define StrategyReportManager_h
class Strategy;
class StrategyReport;

class StrategyReportManager
{
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
