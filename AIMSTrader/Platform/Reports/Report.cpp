//
//  Report.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include "Platform/Reports/Report.h"
#include <unistd.h>
#include <sys/param.h>
#include <iostream>
#include "Platform/View/MainWindow.h"


QString Report::FIELD_START = "<td>";
QString Report::FIELD_END = "</td>";
//String Report::FIELD_END = " ";
//char Report::FIELD_END =' ';
QString Report::HEADER_START = "<th>";
QString Report::HEADER_END = "</th>";
QString Report::ROW_START = "<tr>";
QString Report::ROW_END = "</tr>";
QString Report::FIELD_BREAK = "<br>";
QString Report::REPORT_DIR="/Users/shivkumarchawla/aims.trader/AIMSTrader/Reports/";
//QString Report::ENDOFLINE="\n";

Report::Report(const QString& reportName)
{
    REPORT_NAME = REPORT_DIR;
    REPORT_NAME = REPORT_NAME.append(reportName).append(".txt");

    pFile = fopen(REPORT_NAME.toLatin1(),"w");
    
    //String output;
    //output.append("<meta http-equiv=\"refresh\" content=\"1\" >");
    //output.append("</table><br>"); // close the previously created table, if any
    //output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
    //write(output);

    connect(this, SIGNAL(logMessage(const QString&)), MainWindow::mainWindow(), SLOT(onLog(const QString&)));
}

Report::~Report()
{
    //fout.close();
    fclose(pFile);
}

void Report::write(const QString& output)
{
    const char* cstr = output.toLatin1();
    fprintf(pFile, "%s\n",cstr);

    emit logMessage(output);
    //printf("%s\n",cstr);
}

void Report::write(const char* output)
{
    //this message goes to a file
    fprintf(pFile, "%s\n",output);
    //printf("%s",output);
    //this message goes to the HUI Output
    emit logMessage(QString::fromLatin1(output).append("\n"));
}

const char* Report::getCurrentTime()
{
    time_t rawtime;
    time(&rawtime);
    return ctime(&rawtime) ;
}

