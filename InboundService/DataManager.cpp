#include "DataManager.h"
#include <string>
#include <QDateTime>
#include <QSettings>
#include "API/Helper.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "Data/InstrumentData.h"
#include "Data/GeneralConfigurationData.h"
#include <Shared/ATServerAPIDefines.h>
#include "DataAccess/InstrumentDb.h"
#include <Utils/Log.h>
#include <QCoreApplication>
#include "DataAccess/DailyHistoryBarDb.h"
#include "DataAccess/IntradayHistoryBarDb.h"
#include "DbInterface.h"
#include <QThreadPool>
#include "DataAccess/IntradayHistoryWriteTask.h"

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
    //_cacheNumRecords = 0;
    Logger::log()<<QDateTime::currentDateTime() <<" Setting up ActiveTick Session" << endl;

    qRegisterMetaType<InstrumentId>("InstrumentId");
    qRegisterMetaType<HistoryBarData>("HistoryBarData");
    qRegisterMetaType<QListHistoryBarData >("QListHistoryBarData");

    //connect(this, SIGNAL(dailyHistoryDataUpdateRequested(const InstrumentId, const QListHistoryBarData&)), DBInterface::dbInterface(),SLOT(updateDailyHistoryData(const InstrumentId, const QListHistoryBarData&)));
    //connect(this, SIGNAL(intradayHistoryDataUpdateRequested(const InstrumentId, const QListHistoryBarData&, const int)), DBInterface::dbInterface(),SLOT(updateIntradayHistoryData(const InstrumentId, const QListHistoryBarData&, const int)));

    setupActiveTickSession();
}

void DataManager::shutdownActiveTickSession()
{
   _sessionp->ShutdownSession();
}

void DataManager::reconnectActiveTickAPI()
{
    Logger::log()<<QDateTime::currentDateTime() << "Connection with ActiveTick Server is broken. Retry!!" << endl;
    _sessionp->CreateSession();
    //login now
    std::string serverIpAddress, apiUserid, userid, password;

//    serverIpAddress="activetick1.activetick.com";
//    apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
//    userid="shivchawla";
//    password= "27as04sh";

    fetchActiveTickConnInfo(serverIpAddress, apiUserid, userid, password);

    Logon(serverIpAddress, apiUserid, userid, password);
}

void DataManager::setupActiveTickSession()
{
    ATInitAPI();
    _sessionp = new APISession();
    _requestorp = new Requestor(*_sessionp);

    //login now
    std::string serverIpAddress, apiUserid, userid, password;

//    serverIpAddress="activetick1.activetick.com";
//    apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
//    userid="shivchawla";
//    password= "27as04sh";

    fetchActiveTickConnInfo(serverIpAddress, apiUserid, userid, password);

    Logon(serverIpAddress, apiUserid, userid, password);

}

//Fetches active tick connection info into provided variables
void DataManager::fetchActiveTickConnInfo(string &serverAddress, string &apiUserId, string &userId, string &password) {
    QString path = QCoreApplication::applicationDirPath() + "/Config.ini";
    QSettings settings(path, QSettings::IniFormat);
    //qDebug() << settings.allKeys() << endl;
    serverAddress = settings.value("activetick/server", "activetick1.activetick.com").toString().toStdString();
    apiUserId = settings.value("activetick/apiuserid", "").toString().toStdString();
    userId = settings.value("activetick/userid", "").toString().toStdString();
    password = settings.value("activetick/password", "").toString().toStdString();

    if (userId == "" || password == "") {
        Logger::log(Logger::CRITICAL)<<QDateTime::currentDateTime() << "Active tick credentials not available" << endl;
    }
}

void DataManager::Logon(std::string serverAddress, std::string apiUserId, std::string userId, std::string password)
{
    Logger::log()<<QDateTime::currentDateTime() << "Requesting ActiveTick Server for connection" << endl;
    uint32_t serverPort = 0;
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserId);
    bool rc = _sessionp->Init(guidApiUserid, serverAddress, serverPort, &Helper::ConvertString(userId).front(), &Helper::ConvertString(password).front());

    Logger::log()<<QDateTime::currentDateTime() << "ActiveTick Server is connected" << endl;
}

void DataManager::requestDataToActiveTick(const InstrumentData& instrument, const QDateTime start, const DataType historyType)
{
    if(!_sessionp->IsSessionReady()) {
        reconnectActiveTickAPI();
    }

    if (start == QDateTime()) {
        qWarning() << "Invalid start date!! Skipping Inbound for instrument "
                 << instrument.symbol << " " << instrument.type << endl;
        return;
    }

    //Convert date time strings to YYYYMMDDHHMMSS format
    QString format = "yyyyMMddhhmmss";
    QDateTime end = QDateTime::currentDateTime();

    qDebug() <<"Start and end dates " << start << " " << end << endl;

    if (IsIgnoreCase(start, end)) {
        qDebug() << instrument.symbol << " Ignoring dates as dates could have Sat/Sun. Start:" << start << " End: " << end << endl;

        return;
    }

    ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
    ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

    std::string sym = (instrument.symbol).toStdString();
    qDebug() << "Fetching data for " << instrument.symbol << " at " << QDateTime ::currentDateTime() << endl;

    ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

    mutex.lock();

    ATBarHistoryType atType;
    int intradayCompression = 0;

    if(historyType == DailyBar)
    {
        atType = BarHistoryDaily;
    }
    else
    {
        atType = BarHistoryIntraday;
        switch(historyType)
        {
            case IntradayOneMinuteBar: intradayCompression = 1; break;
            case IntradayTwoMinuteBar: intradayCompression = 2; break;
            case IntradayFiveMinuteBar: intradayCompression = 5; break;
            case IntradayTenMinuteBar: intradayCompression = 10; break;
            case IntradayTwentyMinuteBar: intradayCompression = 20; break;
            case IntradayThirtyMinuteBar: intradayCompression = 30; break;
            case IntradayOneHourBar: intradayCompression = 60; break;
        }
    }


    uint64_t requestId;
    while(!(requestId = _requestorp->requestHistoryData(atSymbol,atBeginTime, atEndTime,atType, intradayCompression))) //requestId of zero indicates connection got broken
    {
        reconnectActiveTickAPI(); //this will block the main thread in session
//        //next statement is executed when main thread is released (this happens when a conection has been established)
//        //when main thread is released send another request to Active Tick
//        requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, atType, intradayCompression, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);
    }

    qDebug()<<"Request Sent: "<<requestId;

    //register the request with the instrumentID
    DataRequest requestInfo(instrument, historyType, start);
    _requestIdToRequestInfo[requestId] = requestInfo ;

    condition.wait(&mutex);
    mutex.unlock();

    //qDebug()<<"Unlocked"<<QThread::currentThreadId();
}

void DataManager::onActiveTickHistoryDataUpdate(uint64_t requestId, QList<HistoryBarData*>& historyList)
{
    //external thread locks the mutex to read instrumentID for requestId
    mutex.lock();

    //external API thread wakes up the MAIN thread
    condition.wakeAll();

    //register the request with the instrumentID
    InstrumentId instrumentId;
    DataType type;

    if(_requestIdToRequestInfo.contains(requestId))
    {
        instrumentId = _requestIdToRequestInfo[requestId].instrument.instrumentId;
        type = _requestIdToRequestInfo[requestId].type;
    }

    mutex.unlock();

    QThreadPool* threadPool = QThreadPool::globalInstance();
    if(type == DailyBar)
    {
        threadPool->start(new DailyHistoryWriteTask(instrumentId, historyList));
    }
    else
    {
//        switch(type)
//        {
             threadPool->start(new IntradayHistoryWriteTask(instrumentId, historyList, type)); //(instrumentId, historyList, 1 )); break;
//            case IntradayOneMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 1 )); break;
//            case IntradayTwoMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 2)); break;
//            case IntradayFiveMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 5 )); break;
//            case IntradayTenMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 10)); break;
//            case IntradayTwentyMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 20 )); break;
//            case IntradayThirtyMinuteBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 30 )); break;
//            case IntradayOneHourBar:  threadPool->start(new DbWriteTask<IntradayHistoryBarDb, HistoryBarData>(instrumentId, historyList, 60 )); break;
        //}
    }
}

void DataManager::updateDailyHistoryData(const InstrumentId instrumentId, const QList<HistoryBarData>& historyList, const bool lastUpdate)
{
    //insert to DB
    // is instrumentId is blank then it is not valid so no insert
//        if((_cacheNumRecords > 1000000 || lastUpdate) && _cacheHistoryBarDataList.count()>0)
//        {
//            DailyHistoryBarDb historyBarDb;
//            int n = historyBarDb.insertDailyHistoryBars(_cacheHistoryBarDataList);
//            Logger::log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

//            InstrumentDb instDb;
//            instDb.updateDailyHistoryBarDate(_cacheLastUpdateDateTime);

//            _cacheHistoryBarDataList.clear();
//            _cacheLastUpdateDateTime.clear();
//            _cacheNumRecords = 0;
//        }
//        else if(historyList.length()>0 && instrumentId > 0)
//        {
//            _cacheHistoryBarDataList[instrumentId] = historyList;
//            _cacheLastUpdateDateTime[instrumentId] = historyList.last().historyDateTime;
//            _cacheNumRecords += historyList.length();
//        }
    if(instrumentId  > 0)
    {
        if(historyList.length() > 0)  //if records retrieved are non-zero
        {
            DailyHistoryBarDb historyBarDb;
            int n = historyBarDb.insertDailyHistoryBars(historyList, instrumentId);
            Logger::log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

            HistoryBarData data = historyList[historyList.length()-1];
            qDebug() << data.historyDateTime;

            if(n>0)
            {
                InstrumentDb instDb;
                instDb.updateDailyHistoryBarDate(instrumentId, data.historyDateTime);
            }
        }
    }
}

void DataManager::updateIntradayHistoryData(const InstrumentId instrumentId, const QList<HistoryBarData>& historyList, const bool lastUpdate)
{
    //insert to DB
//        if((_cacheNumRecords > 1000000 || lastUpdate) && _cacheHistoryBarDataList.count() > 0)
//        {
//            IntradayHistoryBarDb historyBarDb;
//            int n = historyBarDb.insertIntradayHistoryBars(_cacheHistoryBarDataList);
//            Logger::log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

//            InstrumentDb instDb;
//            instDb.updateIntradayHistoryBarDate(_cacheLastUpdateDateTime);

//            _cacheHistoryBarDataList.clear();
//            _cacheLastUpdateDateTime.clear();
//            _cacheNumRecords = 0;
//        }
//        else if (instrumentId > 0 && historyList.length() > 0)
//        {
//            _cacheHistoryBarDataList[instrumentId] = historyList;
//            _cacheLastUpdateDateTime[instrumentId] = historyList.last().historyDateTime;
//            _cacheNumRecords += historyList.length();
//        }

    // is instrumentId is blank then it is not valid so no insert
    if (instrumentId > 0)
    {
        if(historyList.length() > 0)  //if records retrieved are non-zero
        {
            IntradayHistoryBarDb intradayHistoryBarDb;
            int n = intradayHistoryBarDb.insertIntradayHistoryBars(historyList, instrumentId);
            Logger::log() << QDateTime::currentDateTime() << n << " intraday history records written to db for instrument id" << instrumentId << endl;

            HistoryBarData data = historyList[historyList.length()-1];

             //historyList.end()
             qDebug() << data.historyDateTime;
             if(n>0)
             {
                InstrumentDb instDb;
                instDb.updateIntradayHistoryBarDate(instrumentId, data.historyDateTime);
             }
        }
    }
}


void DataManager::requestDailyHistoryData(const QList<InstrumentData>& instruments, const QDateTime& defaultDateTime)
{
    InstrumentDb instDb;
    QHash<uint, QDateTime> lastUpdatedDailyHistoryDateTimeMap = instDb.getLastDailyHistoryUpdateDateForAllInstruments();
    foreach(InstrumentData it, instruments)
    {
        QDateTime dateTime;

        if(lastUpdatedDailyHistoryDateTimeMap.contains(it.instrumentId))
        {
            dateTime = lastUpdatedDailyHistoryDateTimeMap[it.instrumentId].addDays(1);
        }
        else
        {
            dateTime = defaultDateTime;
        }

        requestDataToActiveTick(it, dateTime, DailyBar);
    }
}


void DataManager::requestIntradayHistoryData(const QList<InstrumentData>& instruments, const QDateTime& defaultDateTime)
{
    IntradayHistoryBarDb iDb;
    iDb.deleteOldIntradayRecords(defaultDateTime);

    InstrumentDb instDb;
    QHash<uint, QHash<uint, QDateTime> > lastUpdatedIntradayHistoryDateTimeMap = instDb.getLastIntradayHistoryUpdateDateForAllInstruments();

    foreach(InstrumentData it, instruments) {
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(1, defaultDateTime), IntradayOneMinuteBar);
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(2, defaultDateTime), IntradayTwoMinuteBar);
        requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(5, defaultDateTime), IntradayFiveMinuteBar);
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(10, defaultDateTime), IntradayTenMinuteBar);
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(20, defaultDateTime), IntradayTwentyMinuteBar);
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(30, defaultDateTime), IntradayThirtyMinuteBar);
        //requestDataToActiveTick(it, lastUpdatedIntradayHistoryDateTimeMap.value(it.instrumentId, QHash<uint, QDateTime>()).value(60, defaultDateTime), IntradayOneHourBar);

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

void DataManager::requestTimeout(const uint64_t requestId)
{
    //register the request with the instrumentID
//    InstrumentData instrument;
//    DataType type;
//    QDateTime start;

    mutex.lock();

    //external API thread wakes up the MAIN thread
    condition.wakeAll();
    DataRequest request = _requestIdToRequestInfo.value(requestId,DataRequest());

    mutex.unlock();

    //resend the request
    requestDataToActiveTick(request.instrument, request.start, request.type);
}

