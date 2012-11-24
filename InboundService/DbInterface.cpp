#include "DbInterface.h"
#include "DataAccess/DailyHistoryBarDb.h"
#include "DataAccess/IntradayHistoryBarDb.h"
#include "DataAccess/InstrumentDb.h"
#include "Utils/ThreadManager.h"
#include "Utils/Log.h"
#include <QThread>

DBInterface* DBInterface:: _interface = NULL;

DBInterface::DBInterface(QObject *parent) :
    QObject(parent)
{
    _cacheNumRecords = 0;
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(start()));
    thread->start();
}

void DBInterface::start()
{
   //startTimer(60000 * 5);  // 5-minute timer
}

void DBInterface::timerEvent(QTimerEvent *event)
{
    QListHistoryBarData d;
    updateDailyHistoryData(0, d, true);
    updateIntradayHistoryData(0, d, true);
}

void DBInterface::updateDailyHistoryData(const InstrumentId instrumentId, const QListHistoryBarData& historyList, const bool forcedWrite)
{
//    if((_cacheNumRecords > 1000000 || forcedWrite) && _cacheHistoryBarDataList.count()>0)
//    {
//        DailyHistoryBarDb historyBarDb;
//        int n = historyBarDb.insertDailyHistoryBars(_cacheHistoryBarDataList);
//        log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

//        InstrumentDb instDb;
//        instDb.updateDailyHistoryBarDate(_cacheLastUpdateDateTime);

//        foreach(QListHistoryBarData data, _cacheHistoryBarDataList.values())
//        {
//            data.clear();
//        }

//        _cacheHistoryBarDataList.clear();
//        _cacheLastUpdateDateTime.clear();
//        _cacheNumRecords = 0;
//    }
//    else if(historyList.length()>0 && instrumentId > 0)
//    {
//        _cacheHistoryBarDataList[instrumentId] = *(const_cast<QListHistoryBarData*>(&historyList));
//        _cacheLastUpdateDateTime[instrumentId] = historyList.last().historyDateTime;
//        _cacheNumRecords += historyList.length();
//    }

    if(instrumentId  > 0)
    {
        if(historyList.length() > 0)  //if records retrieved are non-zero
        {
            DailyHistoryBarDb historyBarDb;
            int n = historyBarDb.insertDailyHistoryBars(historyList, instrumentId);
            log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

            HistoryBarData data = historyList[historyList.length()-1];
            qDebug() << data.historyDateTime;

            InstrumentDb instDb;
            instDb.updateDailyHistoryBarDate(instrumentId, data.historyDateTime);
        }
    }
}

void DBInterface::updateIntradayHistoryData(const InstrumentId instrumentId, const QListHistoryBarData& historyList, const int frequency, const bool forcedWrite)
{
//        if((_cacheNumRecords > 1000000 || forcedWrite) && _cacheHistoryBarDataList.count()>0)
//        {
//            DailyHistoryBarDb historyBarDb;
//            int n = historyBarDb.insertDailyHistoryBars(_cacheHistoryBarDataList);
//            log() << QDateTime::currentDateTime() << n << " daily history records written to db for instrument id" << instrumentId << endl;

//            InstrumentDb instDb;
//            instDb.updateDailyHistoryBarDate(_cacheLastUpdateDateTime);

//            foreach(QListHistoryBarData data, _cacheHistoryBarDataList.values())
//            {
//                data.clear();
//            }

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

    // is instrumentId is blank then it is not valid so no insert
    if (instrumentId > 0)
    {
        if(historyList.length() > 0)  //if records retrieved are non-zero
        {
            IntradayHistoryBarDb intradayHistoryBarDb;
            int n = intradayHistoryBarDb.insertIntradayHistoryBars(historyList, instrumentId, frequency);
            log() << QDateTime::currentDateTime() << n << " intraday history records written to db for instrument id" << instrumentId << endl;

            HistoryBarData data = historyList[historyList.length()-1];

             //historyList.end()
             qDebug() << data.historyDateTime;

            InstrumentDb instDb;
            instDb.updateIntradayHistoryBarDate(instrumentId, data.historyDateTime, frequency);
        }
    }
}


