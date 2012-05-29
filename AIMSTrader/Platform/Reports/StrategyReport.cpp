//
//  StrategyReport.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include "Platform/Reports/StrategyReport.h"

StrategyReport::StrategyReport(const String& reportName):Report(reportName)
{
    init();

}

StrategyReport::~StrategyReport(){}

void StrategyReport::init()
{
    _strategyReportHeaders.push_back("DateTime");
    _strategyReportHeaders.push_back("InstrumentId");
    _strategyReportHeaders.push_back("Position");
    _strategyReportHeaders.push_back("FillPrice");
    _strategyReportHeaders.push_back("Commmission");
    _strategyReportHeaders.push_back("Net Profit");
     reportHeaders(_strategyReportHeaders);
}

void StrategyReport::reportHeaders(const std::list<QString>& headers)
{
    QString output;
    output.append(ROW_START);
    
    std::list<QString>::const_iterator it;
    for(it = headers.begin(); it!=headers.end(); ++it) 
    {
        output.append(HEADER_START).append(*it).append(HEADER_END);
    }
    output.append(ROW_END);
    write(output);
}

void StrategyReport::report(const std::list<String>& columns, const String& datetime)
{
    String output;
    output.append(ROW_START);
    output.append(FIELD_START).append(datetime).append(FIELD_END);

    std::list<String>::const_iterator it;
    for(it=columns.begin(); it!=columns.end(); ++it) 
    {
        output.append(FIELD_START).append(*it);//.append(FIELD_END);
    }
    output.append(ROW_END);
    write(output);
    emit logMessage(output);
}
