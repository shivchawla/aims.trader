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

String Report::FIELD_START = "<td>";
//String Report::FIELD_END = "</td>";
//String Report::FIELD_END = " ";
char Report::FIELD_END =' ';
String Report::HEADER_START = "<th>";
String Report::HEADER_END = "</th>";
String Report::ROW_START = "<tr>";
String Report::ROW_END = "</tr>";
String Report::FIELD_BREAK = "<br>";
String Report::REPORT_DIR="/Users/shivkumarchawla/Work/AutomatedTrading/QT/IBTrader_v4/Reports/";

Report::Report(const String& reportName)
{
    REPORT_NAME = REPORT_DIR;
    REPORT_NAME = REPORT_NAME.append(reportName).append(".txt");
    pFile = fopen(REPORT_NAME.c_str(),"w");
    
    //String output;
    //output.append("<meta http-equiv=\"refresh\" content=\"1\" >");
    //output.append("</table><br>"); // close the previously created table, if any
    //output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
    //write(output);
}

Report::~Report()
{
    //fout.close();
    fclose(pFile);
}

void Report::write(const String& output)
{
    const char* cstr = output.c_str();
    fprintf(pFile, "%s\n",cstr);
    printf("%s\n",cstr);
}

void Report::write(const char* output)
{
    fprintf(pFile, "%s",output);
    printf("%s",output);
}

const char* Report::getCurrentTime()
{
    time_t rawtime;
    time(&rawtime);
    return ctime(&rawtime) ;
}

