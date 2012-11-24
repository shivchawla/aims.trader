#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QHash>
#include <ActiveTickServerAPI.h>

#include <QMutex>
#include <QWaitCondition>
#include <myglobal.h>
#include <QDateTime>
#include "Data/HistoryBarData.h"
#include "Data/InstrumentData.h"

using namespace std;
class InboundService;
class APISession;
class Requestor;
class QTimer;
class GeneralConfigurationData;

struct DataRequest
{
    InstrumentData instrument;
    DataType type;
    QDateTime start;

    DataRequest(const InstrumentData instrument, const DataType type, const QDateTime& start):instrument(instrument),type(type), start(start){}
    DataRequest()
    {
        type = DailyBar;
    }
};

class DataManager : public QObject
{
    Q_OBJECT
    APISession* _sessionp;
    Requestor* _requestorp;
    //QHash<InstrumentId, QList<HistoryBarData> > _cacheHistoryBarDataList;
    //QHash<InstrumentId, QDateTime> _cacheLastUpdateDateTime;
    //uint _cacheNumRecords;

    QHash<uint64_t, DataRequest> _requestIdToRequestInfo;

    QMutex mutex;
    QWaitCondition condition;

    static DataManager* _dataManager;

    private:
        DataManager();
        void Logon(string serverAddress, string apiUserId, string userId, string password);
        void init();
        bool IsIgnoreCase(QDateTime startDate, QDateTime endDate);
        void setupActiveTickSession();
        void shutdownActiveTickSession();
        void reconnectActiveTickAPI();
        void updateDailyHistoryData(const InstrumentId, const QList<HistoryBarData>&, const bool lastUpdate = false );
        void updateIntradayHistoryData(const InstrumentId, const QList<HistoryBarData>&, const bool lastUpate = false);
        void requestDataToActiveTick(const InstrumentData&, const QDateTime start, const DataType type = DailyBar);
        void fetchActiveTickConnInfo(string &serverAddress,string &apiUserId, string &userId, string &password);

    public:
        ~DataManager();
        static DataManager* Instance();
        void requestDailyHistoryData(const QList<InstrumentData>&, const QDateTime&);
        void onActiveTickHistoryDataUpdate(const uint64_t, QList<HistoryBarData*>&);
        void requestIntradayHistoryData(const QList<InstrumentData>&, const QDateTime&);
        void requestTimeout(const uint64_t requestId);

     signals:
        void dailyHistoryDataUpdateRequested(const InstrumentId, const QListHistoryBarData&);
        void intradayHistoryDataUpdateRequested(const InstrumentId, const QListHistoryBarData&, const int);

};

#endif // DATAMANAGER_H
