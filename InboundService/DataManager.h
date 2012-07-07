#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QHash>
#include <QUuid>
#include <ActiveTickServerAPI.h>

#include <QReadWriteLock>
#include <QMutex>
#include <QWaitCondition>
#include "DataAccess/DailyHistoryBarDb.h"

using namespace std;
class InboundService;
class APISession;
class Requestor;
class QTimer;
class InstrumentData;
class ConfigurationData;
class DailyHistoryBarData;

class DataManager
{
    APISession* _sessionp;
    Requestor* _requestorp;
    ConfigurationData* _historyStartDateConf;

    QHash<uint64_t, QUuid> _requestIdToInstrumentId;

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

        void requestDataToActiveTick(const InstrumentData*);

    public:
        ~DataManager();
        static DataManager* Instance();
        void requestData(const QList<InstrumentData*>&);
        void onActiveTickHistoryDataUpdate(const uint64_t, const QList<DailyHistoryBarData*>);
        void setHistoryStartDate(ConfigurationData* conf);

};

#endif // DATAMANAGER_H
