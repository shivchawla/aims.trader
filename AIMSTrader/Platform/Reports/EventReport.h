//
//  EventReport.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//
#pragma once
#ifndef EventReport_h
#define EventReport_h

#include "Platform/Reports/Report.h"

class EventReport: public Report
{
    private:
        bool _isEnabled;
        
    public: 
        EventReport();
        ~EventReport(); 
    
    public:
        void disable();
        void enable(); 
    
    public:
        void report(const String& reporter, const String& message);
        void report();
    
};

static EventReport* eventReport()
{
    static EventReport* er = new EventReport();
    return er;
}


#endif
