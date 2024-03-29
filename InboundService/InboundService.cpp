#include <QDebug>
#include "InboundService.h"
#include "DataAccess/GeneralConfigurationDb.h"
#include "DataAccess/InstrumentDb.h"
#include "Utils/Constants.h"
#include <QTimer>
#include "DataManager.h"
#include <Utils/Log.h>

InboundService::InboundService():QObject()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(StartInbound()));
    timer.setSingleShot(true);
}

InboundService::~InboundService()
{}

void InboundService::Init()
{}

void InboundService :: StartInbound()
{
    //qDebug() << QDateTime::currentDateTime();
    Logger::log() << QDateTime::currentDateTime() << " Starting Inbound Service" << endl;
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
    GeneralConfigurationDb confDb;
    Logger::log() << QDateTime::currentDateTime() << " Loading Generic History Date"<< endl;
    GeneralConfigurationData dailyHistoryStartDateConf = confDb.getConfigurationByKey("DailyHistoryStartDate");
    //GeneralConfigurationData* intradayHistoryStartDateConf = confDb.GetConfigurationByKey("IntradayHistoryStartDate");

    InstrumentDb instDb;
    Logger::log() << QDateTime::currentDateTime() << " Loading Instruments"<<endl;
    QList<InstrumentData> instruments = instDb.getInstruments();

    Logger::log() << QDateTime::currentDateTime() << " Requesting Daily History Bar Data for Instruments"<<endl;
    DataManager::Instance()->requestDailyHistoryData(instruments, QDateTime::fromString(dailyHistoryStartDateConf.value, Qt::ISODate));

    Logger::log() << QDateTime::currentDateTime() << " Requesting OneMinute History Bar Data for Instruments"<<endl;
    DataManager::Instance()->requestIntradayHistoryData(instruments, QDateTime(QDate::currentDate().addMonths(-1), QTime(0,0)));

    qDebug() << "All instruments data sent to server..." << endl;
}

void InboundService :: scheduleNextRun()
{
    //get schedule everytime because it could have changed
    GeneralConfigurationDb confDb;
    Logger::log() << QDateTime::currentDateTime() << " Loading Time for scheduling next run "<< endl;
    GeneralConfigurationData scheduleRunTime = confDb.getConfigurationByKey(CONF_SCHEDULE_RUNTIME);

    QTime scheduleTime = QTime::fromString(scheduleRunTime.value, "HH:mm:ss");
    if (!scheduleTime.isValid()) {
        qWarning() << "Invalid schedule time value detected in configiration. Must be in HH:mm:ss (military time) format. Defaulting to 7am" << endl;
        scheduleTime = QTime(7, 0,0); //7am
    }

    //Find how many seconds between right now and next occurance of 7am
    QDateTime now = QDateTime::currentDateTime();
    QDateTime next = now.addDays(1);
    next.setTime(scheduleTime);
    uint seconds = now.secsTo(next);

    timer.setInterval(seconds*1000);
    timer.start();

    Logger::log() << QDateTime::currentDateTime() << "Inbound scheduled to run next time at " << next << endl;
}

void InboundService::shutdown()
{
    Logger::log() << QDateTime::currentDateTime() << "Stopping Service" << endl;
    qDebug() << "Stopping service..." << endl;
}

void InboundService::InvokeService()
{
	qDebug() << "Invoking service" << endl;
}


//My data or logical objects shold be agnostic about the API(s). I just request Data and rest of the logic be outside
