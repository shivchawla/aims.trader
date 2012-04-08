/*
//  StrategyReport.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
*/

#ifndef StrategyReport_h
#define StrategyReport_h

#include <list>
#include "Platform/Reports/Report.h"
class Strategy;

class StrategyReport: public Report
{
    private:
//        Strategy* _strategyWPtr;

    public:
        StrategyReport(const QString& reportName);
        ~StrategyReport();
    
    public:
        void reportHeaders(const std::list<QString>& headers);
        void report(const std::list<QString>& columns, const QString& datetime);
};

#endif
