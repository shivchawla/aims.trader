#include "DataManager.h"
#include <string>
#include <QDateTime>
#include "API/Helper.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "Data/InstrumentData.h"
#include <Shared/ATServerAPIDefines.h>
#include "Data/DailyHistoryBarData.h"
#include <QDebug>

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
   _numRequests=0;
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
    //_sessionp->WaitForSession();
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

    uint64_t requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily, 0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

    someReadWriteLock.lockForWrite();
    _numRequests++;
    //register the request with the instrumentID
    _requestIdToInstrumentId[requestId] = instrument->instrumentId;
    someReadWriteLock.unlock();
}

void DataManager::onActiveTickHistoryDataUpdate(uint64_t requestId, const QList<DailyHistoryBarData*>& historyList)
{
    someReadWriteLock.lockForWrite();
    //register the request with the instrumentID
    if(_requestIdToInstrumentId.contains(requestId))
    {
        QUuid instrumentId = _requestIdToInstrumentId[requestId];
    }

    int pendingRequests = --_numRequests;
    someReadWriteLock.unlock();

    //insert to DB
    if(pendingRequests)
    {
        shutdownActiveTickSession();
    }
}

void DataManager::requestData(const QList<InstrumentData*>& instruments, const string& startTime, const string& endTime)
{
    ATTIME atBeginTime = Helper::StringToATTime(startTime);
    ATTIME atEndTime = Helper::StringToATTime(endTime);

    QList<InstrumentData*>::const_iterator end = instruments.end();
    QList<InstrumentData*>::const_iterator it;

    for(it =instruments.begin(); it!=end;++it)
    {
        requestDataToActiveTick(*it, atBeginTime, atEndTime);
    }
}
