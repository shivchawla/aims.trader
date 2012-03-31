//
//  Report.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include "Platform/Reports/Report.h"

String Report::FIELD_START = "<td>";
String Report::FIELD_END = "</td>";
String Report::HEADER_START = "<th>";
String Report::HEADER_END = "</th>";
String Report::ROW_START = "<tr>";
String Report::ROW_END = "</tr>";
String Report::FIELD_BREAK = "<br>";

Report::Report(const String& reportName)
{
    REPORT_NAME = reportName;
    REPORT_NAME = REPORT_NAME.append(".htm");
    fout.open(REPORT_NAME.c_str());
    
    String output;
    output.append("</table><br>"); // close the previously created table, if any
    output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
    
    fout<<output;
}

Report::~Report()
{
    fout.close();
}

void Report::write(const String& output)
{
    fout<<output;
}
