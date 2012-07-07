#include "DataManager.h"
#include <string>
#include <QDateTime>
#include "API/Helper.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "Data/InstrumentData.h"
#include "Data/ConfigurationData.h"
#include <Shared/ATServerAPIDefines.h>
//#include "DataAccess/DailyHistoryBarDb.h"
#include "DataAccess/InstrumentDb.h"

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

void DataManager :: setHistoryStartDate(ConfigurationData* conf) {
    _historyStartDateConf = conf;
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

void DataManager::requestDataToActiveTick(const InstrumentData* instrument)
{
    if(!_sessionp->IsSessionReady()) {
        reconnectActiveTickAPI();
    }


    InstrumentDb instDb;
    QDateTime start = instDb.getLastHistoryDate(instrument->instrumentId);

    if (start == QDateTime())
        start = QDateTime :: fromString(_historyStartDateConf->value, "dd-MMM-yyyy");

    if (start == QDateTime()) {
        qDebug() << "Invalid start date!! Skipping Inbound for instrument "
                 << instrument->symbol << " " << instrument->type << endl;
        //delete historyStartDateConf;
        return;
    }

    //Convert date time strings to YYYYMMDDHHMMSS format
    QString format = "yyyyMMddhhmmss";
    start = start.addDays(1);
    QDateTime end = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

    qDebug() <<"Start and end dates " << start << " " << end << endl;

    if (IsIgnoreCase(start, end)) {
        qDebug() << "Ignoring dates as dates could have Sat/Sun. Start:" << start << " End: " << end << endl;
        //delete historyStartDateConf;
        return;
    }

    ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
    ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

    std::string sym = (instrument->symbol).toStdString();
    qDebug() << "Fetching data for " << instrument->symbol << " at " << QDateTime ::currentDateTime() << endl;

    ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

    mutex.lock();
    uint64_t requestId = _requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily, 0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);
    /* qDebug()<<"RequestId"<<requestId;

    qDebug()<<"Requesting"<<QThread::currentThreadId();
    qDebug()<<"Requesting Done";
    _numRequests++; */
    //register the request with the instrumentID
    _requestIdToInstrumentId[requestId] = instrument->instrumentId;

    condition.wait(&mutex);
    mutex.unlock();

    //someReadWriteLock.unlock();
    qDebug()<<"Unlocked"<<QThread::currentThreadId();

}

void DataManager::onActiveTickHistoryDataUpdate(uint64_t requestId, const QList<DailyHistoryBarData*> historyList)
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
        int n = historyBarDb.insertDailyHistoryBars(historyList, instrumentId);
        qDebug() << n << " daily history records written to db for instrument id" << instrumentId << endl;

        foreach(DailyHistoryBarData* history, historyList)
            delete history; //memory cleanup

       InstrumentDb instDb;
       instDb.updateDailyHistoryBarDate(instrumentId);
    }
    // qDebug()<<"Updated"<<instrumentId;
}

void DataManager::requestData(const QList<InstrumentData*>& instruments)
{
    foreach(InstrumentData* it, instruments) {
        //qDebug()<<"Requesting"<<(*it)->symbol;
        requestDataToActiveTick(it);
    }
}

//Uses start date and end date to determine if dates should be ignored and data not fetched from server
// as the dates might signify saturday and sunday
bool DataManager :: IsIgnoreCase(QDateTime startDate, QDateTime endDate) {
    QString startDay = startDate.toString("ddd");
    QString endDay = endDate.toString("ddd");
    if ((startDay == "Sat" && endDay == "Sun") ||
        (startDate.date() == endDate.date() && (startDay == "Sat" || startDay == "Sun")))
        return true; //means this is a ignore case
    else
        return false;
}
