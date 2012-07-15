#include "DataManager.h"
#include <string>
#include <QDateTime>
#include "API/Helper.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "Data/InstrumentData.h"
#include "Data/GeneralConfigurationData.h"
#include <Shared/ATServerAPIDefines.h>
#include "DataAccess/InstrumentDb.h"
#include <Utils/Log.h>


DataManager* DataManager::_dataManager = NULL;

DataManager::DataManager()
{
    init();
}

DataManager* DataManager::Instance()
{
    if(!_dataManager)
    {
        _dataManager = new DataManager();
    }
    return _dataManager;
}

DataManager::~DataManager()
{
    delete _requestorp;
    _sessionp->Destroy();
    ATShutdownAPI();
    delete _sessionp;
}

void DataManager::init()
{
    log()<<QDateTime::currentDateTime() <<" Setting up ActiveTick Session" << endl;
    setupActiveTickSession();
}

void DataManager::shutdownActiveTickSession()
{
   _sessionp->ShutdownSession();
}

void DataManager :: setHistoryStartDate(GeneralConfigurationData* conf) {
    _historyStartDateConf = conf;
}

void DataManager::reconnectActiveTickAPI()
{
    log()<<QDateTime::currentDateTime() << "Connection with ActiveTick Server is broken. Retry!!" << endl;
    _sessionp->CreateSession();
    //login now
    std::string serverIpAddress, apiUserid, userid, password;

    serverIpAddress="activetick1.activetick.com";
    apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
    userid="shivchawla";
    password= "27as04sh";

    Logon(serverIpAddress, apiUserid, userid, password);
}

void DataManager::setupActiveTickSession()
{
    ATInitAPI();
    _sessionp = new APISession();
    _requestorp = new Requestor(*_sessionp);

    //login now
    std::string serverIpAddress, apiUserid, userid, password;

    serverIpAddress="activetick1.activetick.com";
    apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
    userid="shivchawla";
    password= "27as04sh";

    Logon(serverIpAddress, apiUserid, userid, password);

}

void DataManager::Logon(std::string serverAddress, std::string apiUserId, std::string userId, std::string password)
{
    log()<<QDateTime::currentDateTime() << "Requesting ActiveTick Server for connection" << endl;
    uint32_t serverPort = 0;
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserId);
    bool rc = _sessionp->Init(guidApiUserid, serverAddress, serverPort, &Helper::ConvertString(userId).front(), &Helper::ConvertString(password).front());

    log()<<QDateTime::currentDateTime() << "ActiveTick Server is connected" << endl;
}

void DataManager::requestDataToActiveTick(const InstrumentData* instrument, const QDateTime start)
{
    if(!_sessionp->IsSessionReady()) {
        reconnectActiveTickAPI();
    }

    if (start == QDateTime()) {
        qWarning() << "Invalid start date!! Skipping Inbound for instrument "
                 << instrument->symbol << " " << instrument->type << endl;
        return;
    }

    //Convert date time strings to YYYYMMDDHHMMSS format
    QString format = "yyyyMMddhhmmss";
    QDateTime end = QDateTime::currentDateTime();

    qDebug() <<"Start and end dates " << start << " " << end << endl;

    if (IsIgnoreCase(start, end)) {
        qDebug() << instrument->symbol << "Ignoring dates as dates could have Sat/Sun. Start:" << start << " End: " << end << endl;

        return;
    }

    ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
    ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

    std::string sym = (instrument->symbol).toStdString();
    qDebug() << "Fetching data for " << instrument->symbol << " at " << QDateTime ::currentDateTime() << endl;

    ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

    mutex.lock();
    uint64_t requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily, 0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

    while(!requestId) //requestId of zero indicates connection got broken
    {
        reconnectActiveTickAPI(); //this will block the main thread in session

        //next statement is executed when main thread is released (this happens when a conection has been established)
        //when main thread is released send another request to Active Tick
        requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily, 0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);
    }

    qDebug()<<"Request Sent: "<<requestId;

    //register the request with the instrumentID
    _requestIdToInstrumentId[requestId] = instrument->instrumentId;

    condition.wait(&mutex);
    mutex.unlock();

    //qDebug()<<"Unlocked"<<QThread::currentThreadId();
}

void DataManager::onActiveTickHistoryDataUpdate(uint64_t requestId, const QList<DailyHistoryBarData*> historyList)
{
    //external thread locks the mutex to read instrumentID for requestId
    mutex.lock();

    //external API thread wakes up the MAIN thread
    condition.wakeAll();

    //register the request with the instrumentID
    uint instrumentId;
    if(_requestIdToInstrumentId.contains(requestId))
    {
        instrumentId = _requestIdToInstrumentId[requestId];
    }
    mutex.unlock();
    //qDebug()<<"Unlocked"<<QThread::currentThreadId();

    //insert to DB
    // is instrumentId is blank then it is not valid so no insert
    if (instrumentId > 0)
    {
        if(historyList.length() > 0)  //if records retrieved are non-zero
        {
            DailyHistoryBarDb historyBarDb;
            int n = historyBarDb.insertDailyHistoryBars(historyList, instrumentId);
            log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

            DailyHistoryBarData* data = historyList[historyList.length()-1];

             //historyList.end()
             qDebug() << data->historyDate;

            InstrumentDb instDb;
            instDb.updateDailyHistoryBarDate(instrumentId, data->historyDate);

            foreach(DailyHistoryBarData* history, historyList)
                delete history; //memory cleanup
        }
    }
    // qDebug()<<"Updated"<<instrumentId;
}

void DataManager::requestData(const QList<InstrumentData*>& instruments)
{
    InstrumentDb instDb;
    QHash<uint, QDateTime> lastUpdatedHistoryDateTimeMap = instDb.getLastHistoryUpdateDateForAllInstruments();
    foreach(InstrumentData* it, instruments) {
        QDateTime dateTime;

        if(lastUpdatedHistoryDateTimeMap.contains(it->instrumentId))
        {
            dateTime = lastUpdatedHistoryDateTimeMap[it->instrumentId].addDays(1);
        }
        else
        {
            dateTime = QDateTime::fromString(_historyStartDateConf->value,Qt::ISODate);
        }

        requestDataToActiveTick(it, dateTime);
    }
}

//Uses start date and end date to determine if dates should be ignored and data not fetched from server
// as the dates might signify saturday and sunday
bool DataManager :: IsIgnoreCase(QDateTime startDate, QDateTime endDate)
{
    if(startDate > endDate)
    {
        return true;
    }

    int daysGap = startDate.daysTo(endDate);

    if(daysGap == 0 && endDate.date().dayOfWeek() == 6)
    {
        return true;
    }
    else if(daysGap == 1 && endDate.date().dayOfWeek() == 7)
    {
        return true;
    }
    else if(daysGap == 2 && endDate.date().dayOfWeek() == 1)
    {
        return true;
    }

    return false;
}
