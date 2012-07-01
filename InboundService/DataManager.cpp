#include "DataManager.h"
#include <string>
#include <QDateTime>
#include "API/Helper.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "Data/InstrumentData.h"
#include <Shared/ATServerAPIDefines.h>
//#include "DataAccess/DailyHistoryBarDb.h"
#include <QDebug>
#include <QThread>

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
   setupActiveTickSession();
}

void DataManager::shutdownActiveTickSession()
{
   _sessionp->ShutdownSession();
}

void DataManager::reconnectActiveTickAPI()
{
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
    uint32_t serverPort = 0;

    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserId);
    bool rc = _sessionp->Init(guidApiUserid, serverAddress, serverPort, &Helper::ConvertString(userId).front(), &Helper::ConvertString(password).front());
}

void DataManager::requestDataToActiveTick(const InstrumentData* instrument, const ATTIME& atBeginTime, const ATTIME& atEndTime)
{
    if(!_sessionp->IsSessionReady())
    {
        reconnectActiveTickAPI();
    }

    //Convert date time strings to YYYYMMDDHHMMSS format
    std::string sym = (instrument->symbol).toStdString();
    qDebug() << "Fetching data for " << instrument->symbol << " at " << QDateTime ::currentDateTime() << endl;

    ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

    mutex.lock();
    uint64_t requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily, 0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);
   // qDebug()<<"RequestId"<<requestId;

    //qDebug()<<"Requesting"<<QThread::currentThreadId();
    //qDebug()<<"Requesting Done";
    //_numRequests++;
    //register the request with the instrumentID
    _requestIdToInstrumentId[requestId] = instrument->instrumentId;

    condition.wait(&mutex);
    mutex.unlock();

    //someReadWriteLock.unlock();
    qDebug()<<"Unlocked"<<QThread::currentThreadId();

}

void DataManager::onActiveTickHistoryDataUpdate(uint64_t requestId, const QList<DailyHistoryBarData*>& historyList)
{
   // qDebug()<<"Updating"<<QThread::currentThreadId();

    //external API thread wakes up the MAIN thread
    condition.wakeAll();

    //external thread locks the mutex to read instrumentID for requestId
    mutex.lock();
    //register the request with the instrumentID
    QUuid instrumentId;
    if(_requestIdToInstrumentId.contains(requestId))
    {
        instrumentId = _requestIdToInstrumentId[requestId];
    }
    mutex.unlock();
    qDebug()<<"Unlocked"<<QThread::currentThreadId();

    //insert to DB
    if (instrumentId != QUuid()) {// is instrumentId is blank then it is not valid so no insert
        DailyHistoryBarDb historyBarDb;
        foreach(DailyHistoryBarData* history, historyList) {
            history->instrumentId = instrumentId;
            history->updatedBy = "InboundService";
            history->updatedDate = QDateTime::currentDateTime();
            history->dailyHistoryBarId = QUuid::createUuid();
            historyBarDb.insertDailyHistoryBar(*history);
            delete history; //memory cleanup
        }
    }
   // qDebug()<<"Updated"<<instrumentId;
}

void DataManager::requestData(const QList<InstrumentData*>& instruments, const string& startTime, const string& endTime)
{
    ATTIME atBeginTime = Helper::StringToATTime(startTime);
    ATTIME atEndTime = Helper::StringToATTime(endTime);

    QList<InstrumentData*>::const_iterator end = instruments.end();
    QList<InstrumentData*>::const_iterator it;

    for(it = instruments.begin(); it!=end; ++it)
    {
        //qDebug()<<"Requesting"<<(*it)->symbol;
        requestDataToActiveTick(*it, atBeginTime, atEndTime);
    }
}
