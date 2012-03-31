//
//  StrategyReport.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include "Platform/Reports/StrategyReport.h"

StrategyReport::StrategyReport(const String& reportName):Report(reportName)
{}

StrategyReport::~StrategyReport(){}

void StrategyReport::reportHeaders(const std::list<String>& headers) 
{
    String output;
    output.append(ROW_START);
    
    std::list<String>::const_iterator it;
    for(it = headers.begin(); it!=headers.end(); ++it) 
    {
        output.append(HEADER_START).append(*it).append(HEADER_END);
    }
    output.append(ROW_END);
    write(output);
}

void StrategyReport::report(const std::list<String>& columns, const String& date, const String& time)
{
    String output;     
    output.append(ROW_START);
    output.append(FIELD_START).append(date);//.append(FIELD_END);
    output.append(FIELD_START).append(time);//.append(FIELD_END);
    
    std::list<String>::const_iterator it;
    for(it=columns.begin(); it!=columns.end(); ++it) 
    {
        output.append(FIELD_START).append(*it);//.append(FIELD_END);
    }
    output.append(ROW_END);
    write(output);
}
