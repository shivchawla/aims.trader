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
<<<<<<< HEAD
        void report(const String& reporter, const String& message);
=======
        void report(const QString& reporter, const QString& message);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
        //void report(const char* reporter, const char* message);
        void report();
    //public slots:


    
};

#endif
