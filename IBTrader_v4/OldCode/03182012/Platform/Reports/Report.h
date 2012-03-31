//
//  Report.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef Report_h
#define Report_h

#include<string>
#include<fstream>

typedef std::string String;

class Report
{
    protected:
        static String FIELD_START;
        static String FIELD_END;
        static String HEADER_START;
        static String HEADER_END;
        static String ROW_START;
        static String ROW_END;
        static String FIELD_BREAK;
    
    private: 
        std::ofstream fout;
        String REPORT_NAME;
        //PrintWriter writer;
    
    public:
        Report(const String& reportName);         
        virtual ~Report(); 
    
    protected:
        void write(const String&);
};


#endif
