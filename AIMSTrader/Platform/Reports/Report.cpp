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

<<<<<<< HEAD
String Report::FIELD_START ="<td>";
String Report::FIELD_END = "</td>";
String Report::HEADER_START = "<th>";
String Report::HEADER_END = "</th>";
String Report::ROW_START = "<tr>";
String Report::ROW_END = "</tr>";
String Report::FIELD_BREAK = "<br>";
String Report::REPORT_DIR="/Users/shivkumarchawla/aims.trader/AIMSTrader/Reports/";

Report::Report(const String& reportName)
{
    REPORT_NAME = REPORT_DIR;
    REPORT_NAME = REPORT_NAME.append(reportName).append(".html");
=======

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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

    pFile = fopen(REPORT_NAME.toLatin1(),"w");
    
     String output;
    //output.append("<meta http-equiv=\"refresh\" content=\"1\" >");
<<<<<<< HEAD
    output.append("</table><br>"); // close the previously created table, if any
    output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
    write(output);

    connect(this, SIGNAL(logMessage(const String&)), MainWindow::mainWindow(), SLOT(onLog(const String&)));
=======
    //output.append("</table><br>"); // close the previously created table, if any
    //output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
    //write(output);

    connect(this, SIGNAL(logMessage(const QString&)), MainWindow::mainWindow(), SLOT(onLog(const QString&)));
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

Report::~Report()
{
    //fout.close();
    fclose(pFile);
}

<<<<<<< HEAD
//either protect this for multiple threads or run this thing on separate thread
void Report::write(const String& output)
{
    const char* cstr = output.toLatin1();
    mutex.lock();
    fprintf(pFile, "%s\n",cstr);
    mutex.unlock();

    emit logMessage(output);
=======
void Report::write(const QString& output)
{
    const char* cstr = output.toLatin1();
    fprintf(pFile, "%s\n",cstr);

    emit logMessage(output);
    //printf("%s\n",cstr);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void Report::write(const char* output)
{
<<<<<<< HEAD
    mutex.lock();
    //this message goes to a file
    fprintf(pFile, "%s\n",output);
    mutex.unlock();
=======
    //this message goes to a file
    fprintf(pFile, "%s\n",output);
    //printf("%s",output);
    //this message goes to the HUI Output
    emit logMessage(QString::fromLatin1(output).append("\n"));
}
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

}

