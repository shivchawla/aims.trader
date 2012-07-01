#include <QDebug>
#include "InboundService.h"
#include "DataAccess/ConfigurationDb.h"
#include "DataAccess/InstrumentDb.h"
#include "Utils/Constants.h"
#include <QTimer>
#include "DataManager.h"
#include <iostream>

InboundService::InboundService():QObject()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(StartInbound()));
    timer.setSingleShot(true);
}

InboundService::~InboundService()
{}

void InboundService::Init()
{}

void InboundService :: StartInbound() {

    qDebug() << "Starting inbound..." << endl;
    //return;

    Init();
    startDataProcessing();
    scheduleNextRun();
}

void InboundService::startDataProcessing()
{
    //Load new symbols from instrument File
    loadNewSymbols();
    updatePriceHistory();
}

void InboundService::loadNewSymbols()
{}

void InboundService::updatePriceHistory()
{
    ConfigurationDb confDb;
    ConfigurationData* historyStartDateConf = confDb.GetConfigurationByKey("HistoryStartDate");

    InstrumentDb instDb;
    QList<InstrumentData*> instruments = instDb.getInstruments();

    QString format = QString("yyyyMMddhhmmss");
    QDateTime start = QDateTime :: fromString(historyStartDateConf->value, "dd-MMM-yyyy");
    QDateTime end = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

    if (start == QDateTime()) {
        qDebug() << "Invalid start date!! Stopping Daily History Bar Inbound." << endl;
        delete historyStartDateConf;
        return;
    }

    if (IsIgnoreCase(start, end)) {
        qDebug() << "Ignoring dates as dates culd have Sat/Sun. Start:" << start << " End: " << end << endl;
        delete historyStartDateConf;
        return;
    }

    DataManager::Instance()->requestData(instruments, start.toString(format).toStdString(), end.toString(format).toStdString());

    qDebug() << "All instruments data sent to server..." << endl;
    historyStartDateConf->value = QDateTime::currentDateTime().toString("dd-MMM-yyyy");
    confDb.UpdateConfiguration(historyStartDateConf, QString("HistoryStartDate"));
    qDebug() << "HistoryStartDate updated as " << historyStartDateConf->value << endl;

    //clean up memory
    qDebug() << "Cleaning up memory!" << endl;
    for(int i=0; i< instruments.count(); ++i)
    {
        delete instruments.at(i);
    }
}

void InboundService :: scheduleNextRun()
{
    //get schedule everytime because it could have changed
    ConfigurationDb confDb;
    ConfigurationData* scheduleRunTime = confDb.GetConfigurationByKey(CONF_SCHEDULE_RUNTIME);

    QTime scheduleTime = QTime::fromString(scheduleRunTime->value, "HH:mm:ss");
    if (!scheduleTime.isValid()) {
        qDebug() << "Invalid schedule time value detected in configiration. Must be in HH:mm:ss (military time) format. Defaulting to 7am" << endl;
        scheduleTime = QTime(7, 0,0); //7am
    }

    //Find how many seconds between right now and next occurance of 7am
    QDateTime now = QDateTime::currentDateTime();
    QDateTime next = now.addDays(1);
    next.setTime(scheduleTime);
    uint seconds = now.secsTo(next);

    timer.setInterval(seconds*1000);
    timer.start();

    qDebug() << "Inbound scheduled to run next time at " << next << endl;
}

void InboundService::shutdown()
{
	qDebug() << "Stopping service..." << endl;
}

void InboundService::InvokeService()
{
	qDebug() << "Invoking service" << endl;
}

//Uses start date and end date to determine if dates should be ignored and data not fetched from server
// as the dates might signify saturday and sunday
bool InboundService :: IsIgnoreCase(QDateTime startDate, QDateTime endDate) {
    QString startDay = startDate.toString("ddd");
    QString endDay = endDate.toString("ddd");
    if ((startDay == "Sat" && endDay == "Sun") ||
        (startDate.date() == endDate.date() && (startDay == "Sat" || startDay == "Sun")))
        return true; //means this is a ignore case
    else
        return false;
}


//My data or logical objects shold be agnostic about the API(s). I just request Data and rest of the logic be outside
