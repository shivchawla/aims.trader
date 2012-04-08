//
//  EventReport.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/24/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Reports/EventReport.h"
#include <QDateTime>

EventReport::EventReport():Report("EventReport")
{
    _isEnabled = true;
    
    String output;
    output.append(ROW_START);
    output.append(FIELD_START).append("DateTime").append(FIELD_END);
    output.append(FIELD_START).append("Reporter").append(FIELD_END);
    output.append(FIELD_START).append("Message").append(FIELD_END);
    output.append(ROW_END);
    write(output);
        
    String startupMessage;
    startupMessage.append("Event Report Started ");
    report("AIMSTrader", startupMessage);
}

EventReport::~EventReport()
{}

void EventReport::disable() 
{
    _isEnabled = false;
}
    
void EventReport::enable()
{
    _isEnabled = true;
}
    
void EventReport::report(const String& reporter, const String& message)
{
    if(_isEnabled)
    {
        //get date and time too
        String output;
        output.reserve(reporter.size() + message.size() + 20);
        output.append(ROW_START);
        output.append(FIELD_START).append(QDateTime::currentDateTime().toString()).append(FIELD_END);
        output.append(FIELD_START).append(reporter).append(FIELD_END);
        output.append(FIELD_START).append(message).append(FIELD_END);
        output.append(ROW_END);
        write(output);
        //emit logMessage(output);
    }
}
    
/*void EventReport::report(const char* reporter, const char* message)
{
    if(_isEnabled)
    {
        //get date and time too
        //String output;
        //output.append(ROW_START);
        //output.append(FIELD_START).append(dateFormat.format(date)).append(FIELD_END);
        //output.append(FIELD_START).append(timeFormat.format(date)).append(FIELD_END);
        //output.append(reporter).append(FIELD_END);
        //output.append(FIELD_START).append(message).append(FIELD_END);
        //output.append(ROW_END);
        //output.append(reporter).append(FIELD_END).append(message).append(FIELD_END)
        char output[100];
        strcat(output, reporter);
        strcat(output, &FIELD_END);
        strcat(output, message);
        strcat(output, &FIELD_END);
        strcat(output, getCurrentTime());
        write(output);
    }
}*/

