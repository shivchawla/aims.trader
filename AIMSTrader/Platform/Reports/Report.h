//
//  Report.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//
#pragma once
#ifndef Report_h
#define Report_h

#include<string>
#include<fstream>
#include<stdio.h>
#include <QObject>
#include <AimsTraderDefs/typedefs.h>
#include <QMutex>


class Report: public QObject
{
    Q_OBJECT
    protected:
        static String FIELD_START;
        static String FIELD_END;
        static String HEADER_START;
        static String HEADER_END;
        static String ROW_START;
        static String ROW_END;
        static String FIELD_BREAK;
        static String REPORT_DIR;

    private: 
        FILE* _pFile;
        QMutex _mutex;
    
    public:
        Report(const String& reportName);
        String REPORT_NAME;
    
    public:
        virtual ~Report(); 
    
    protected:
        void write(const String&);
        void write(const char*);

    signals:
        void logMessage(const String&);

        const char* getCurrentTime();

};


#endif
