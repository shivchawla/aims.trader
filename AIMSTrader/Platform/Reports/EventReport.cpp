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
    
<<<<<<< HEAD
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
=======
    QString output;
    output.append(ROW_START);
    //output.append("<TH WIDTH=\"130\">").append("Time").append(HEADER_END);
    output.append("<TH WIDTH=\"130\">").append("Reporter").append(HEADER_END);
    output.append(HEADER_START).append("Message").append(HEADER_END);
    output.append("<TH WIDTH=\"80\">").append("Date").append(HEADER_END);
    output.append(ROW_END);
    write(output);
        
    QString startupMessage;
    startupMessage.append("Event Report Started ");
    report(QString::fromAscii("AIMSTrader"), startupMessage);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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
    
<<<<<<< HEAD
void EventReport::report(const String& reporter, const String& message)
=======
void EventReport::report(const QString& reporter, const QString& message)
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    if(_isEnabled)
    {
        //get date and time too
<<<<<<< HEAD
        String output;
        output.reserve(reporter.size() + message.size() + 20);
        output.append(ROW_START);
        output.append(FIELD_START).append(QDateTime::currentDateTime().toString()).append(FIELD_END);
        output.append(FIELD_START).append(reporter).append(FIELD_END);
        output.append(FIELD_START).append(message).append(FIELD_END);
=======
        QString output;
        output.append(ROW_START);
        output.append(FIELD_START).append(reporter).append(FIELD_END);
        output.append(FIELD_START).append(message).append(FIELD_END);
        output.append(FIELD_START).append(QDateTime::currentDateTime().toString()).append(FIELD_END);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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

