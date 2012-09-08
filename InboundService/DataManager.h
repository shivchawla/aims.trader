#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QHash>
#include <ActiveTickServerAPI.h>

#include <QMutex>
#include <QWaitCondition>
#include <myglobal.h>
#include <QDateTime>

using namespace std;
class InboundService;
class APISession;
class Requestor;
class QTimer;
class InstrumentData;
class GeneralConfigurationData;
class HistoryBarData;

struct DataRequest
{
    InstrumentId instrumentId;
    DataType type;

    DataRequest(const InstrumentId instrumentId, const DataType type):instrumentId(instrumentId),type(type){}
    DataRequest()
    {
        instrumentId = 0;
        type = DailyBar;
    }
};

class DataManager
{
    APISession* _sessionp;
    Requestor* _requestorp;

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
        void updateDailyHistoryData(const InstrumentId, const QList<HistoryBarData*>&);
        void updateIntradayHistoryData(const InstrumentId, const QList<HistoryBarData*>&);
        void requestDataToActiveTick(const InstrumentData*, const QDateTime start, const DataType type = DailyBar);
        void fetchActiveTickConnInfo(string &serverAddress,string &apiUserId, string &userId, string &password);

    public:
        ~DataManager();
        static DataManager* Instance();
        void requestDailyHistoryData(const QList<InstrumentData*>&, const QDateTime&);
        void onActiveTickHistoryDataUpdate(const uint64_t, const QList<HistoryBarData*>);
        //void setHistoryStartDate(GeneralConfigurationData* conf);
        void requestIntradayHistoryData(const QList<InstrumentData*>&, const QDateTime&);
};

#endif // DATAMANAGER_H
