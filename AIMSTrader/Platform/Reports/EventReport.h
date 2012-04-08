//
//  EventReport.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef EventReport_h
#define EventReport_h

#include "Platform/Reports/Report.h"

class EventReport: public Report
{
    //Q_OBJECT
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
        //void report(const char* reporter, const char* message);
        void report();
    //public slots:


    
};

#endif
