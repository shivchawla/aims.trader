#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QHash>
#include <QUuid>
#include <ActiveTickServerAPI.h>

#include <QReadWriteLock>
#include <QMutex>
#include <QWaitCondition>

class InboundService;
class APISession;
class Requestor;
class QTimer;
class InstrumentData;

using namespace std;
class DailyHistoryBarData;
#include "DataAccess/DailyHistoryBarDb.h"

class DataManager
{
    APISession* _sessionp;
    Requestor* _requestorp;

    QHash<uint64_t, QUuid> _requestIdToInstrumentId;

    QMutex mutex;
    QWaitCondition condition;

    static DataManager* _dataManager;

    private:
        DataManager();
        void Logon(string serverAddress, string apiUserId, string userId, string password);
        void init();
        void setupActiveTickSession();
        void shutdownActiveTickSession();
        void reconnectActiveTickAPI();

        void requestDataToActiveTick(const InstrumentData*, const ATTIME& , const ATTIME&);

    public:
        ~DataManager();
        static DataManager* Instance();
        void requestData(const QList<InstrumentData*>&, const std::string&, const std::string&);
        void onActiveTickHistoryDataUpdate(const uint64_t, const QList<DailyHistoryBarData*>&);

};

#endif // DATAMANAGER_H
