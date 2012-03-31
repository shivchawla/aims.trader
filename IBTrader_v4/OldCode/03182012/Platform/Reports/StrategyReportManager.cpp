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
   /* df2 = NumberFormatterFactory.getNumberFormatter(2);
    df5 = NumberFormatterFactory.getNumberFormatter(5);
    TimeZone timeZone = strategy.getTradingSchedule().getTimeZone();
    dateFormat = new SimpleDateFormat("MM/dd/yy");
    dateFormat.setTimeZone(timeZone);
    timeFormat = new SimpleDateFormat("HH:mm:ss.SSS z");
    timeFormat.setTimeZone(timeZone);
    
    
    strategyReportColumns = new ArrayList<String>();
    strategyReportHeaders = new ArrayList<String>();
    strategyReportHeaders.add("Date");
    strategyReportHeaders.add("Time");
    strategyReportHeaders.add("Trade #");
    
    String symbol1 = strategy.getInstrument1().getContract().m_symbol;
    String symbol2 = strategy.getInstrument2().getContract().m_symbol;
    
    strategyReportHeaders.add(symbol1 + " Position");
    strategyReportHeaders.add(symbol1 + " Fill Price");
    strategyReportHeaders.add(symbol2 + " Position");
    strategyReportHeaders.add(symbol2 + " Fill Price");
    strategyReportHeaders.add("Commission");
    strategyReportHeaders.add("Trade Net Profit");
    strategyReportHeaders.add("Total Net Profit");
    */
    


void StrategyReportManager::report()
{
}
    /*if (_strategyReportSPtr == NULL)
    {
        _strategyReportSPtr = new StrategyReport(strategy.getName());
        _strategyReportSPtr->reportHeaders(strategyReportHeaders);
    }
    
    boolean isCompletedTrade = performanceManager.getIsCompletedTrade();
    
    strategyReportColumns.clear();
    strategyReportColumns.add(isCompletedTrade ? String.valueOf(performanceManager.getTrades()) : "--");
    Instrument instrument1 = strategy.getInstrument1();
    Instrument instrument2 = strategy.getInstrument2();
    
    strategyReportColumns.add(df5.format(instrument1.getCurrentPosition()));
    strategyReportColumns.add(df5.format(instrument1.getAveFillPrice()));
    
    strategyReportColumns.add(df5.format(instrument2.getCurrentPosition()));
    strategyReportColumns.add(df5.format(instrument2.getAveFillPrice()));
    
    strategyReportColumns.add(df2.format(performanceManager.getTradeCommission()));
    strategyReportColumns.add(isCompletedTrade ? df2.format(performanceManager.getTradeProfit()) : "--");
    strategyReportColumns.add(df2.format(performanceManager.getNetProfit()));
    
    long now = strategy.getTime();
    String date = dateFormat.format(now);
    String time = timeFormat.format(now);
    strategyReport.report(strategyReportColumns, date, time);
    */

