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
#include<stdio.h>
#include <QObject>
typedef std::string String;

class Report: public QObject
{
    Q_OBJECT
    protected:
        static QString FIELD_START;
        //static String FIELD_END;
        static QString FIELD_END;
        static QString HEADER_START;
        static QString HEADER_END;
        static QString ROW_START;
        static QString ROW_END;
        static QString FIELD_BREAK;
        static QString REPORT_DIR;
       // static QString ENDOFLINE;
    
    private: 
        //std::ofstream fout;
        FILE* pFile;
        QString REPORT_NAME;
        //PrintWriter writer;
    
    public:
        Report(const QString& reportName);
        virtual ~Report(); 
    
    protected:
        void write(const QString&);
        void write(const char*);
        const char* getCurrentTime();

    signals:
        void logMessage(const QString&);
};


#endif
