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
       std::list<String> _strategyReportHeaders;
       void init();

    public:
        StrategyReport(const QString& reportName);
        ~StrategyReport();

    public:
<<<<<<< HEAD
        void reportHeaders(const std::list<String>& headers);
        void report(const std::list<String>& columns, const String& datetime);
=======
        void reportHeaders(const std::list<QString>& headers);
        void report(const std::list<QString>& columns, const QString& datetime);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
};

#endif
