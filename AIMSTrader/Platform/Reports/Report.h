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
<<<<<<< HEAD
#include <Platform/typedefs.h>
#include <QMutex>
=======
typedef std::string String;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

class Report: public QObject
{
    Q_OBJECT
    protected:
        static QString FIELD_START;
        //static String FIELD_END;
<<<<<<< HEAD
        static String FIELD_END;
        static String HEADER_START;
        static String HEADER_END;
        static String ROW_START;
        static String ROW_END;
        static String FIELD_BREAK;
        static String REPORT_DIR;
=======
        static QString FIELD_END;
        static QString HEADER_START;
        static QString HEADER_END;
        static QString ROW_START;
        static QString ROW_END;
        static QString FIELD_BREAK;
        static QString REPORT_DIR;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
       // static QString ENDOFLINE;
    
    private: 
        //std::ofstream fout;
        FILE* pFile;
<<<<<<< HEAD
        String REPORT_NAME;
        QMutex mutex;
        //PrintWriter writer;
    
    public:
        Report(const String& reportName);
=======
        QString REPORT_NAME;
        //PrintWriter writer;
    
    public:
        Report(const QString& reportName);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
        virtual ~Report(); 
    
    protected:
        void write(const QString&);
        void write(const char*);
<<<<<<< HEAD

    signals:
        void logMessage(const String&);
=======
        const char* getCurrentTime();

    signals:
        void logMessage(const QString&);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
};


#endif
