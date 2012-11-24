#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QObject>
#include <myglobal.h>
#include "Data/HistoryBarData.h"
//typedef QList<HistoryBarData> QListHistoryBarData;
#include <QHash>
class DBInterface : public QObject
{
    Q_OBJECT
    private:
        int _cacheNumRecords;
        QHash<InstrumentId, QListHistoryBarData>_cacheHistoryBarDataList;
        QHash<InstrumentId, QDateTime> _cacheLastUpdateDateTime;
        static DBInterface * _interface;

    public:
        explicit DBInterface(QObject *parent = 0);

    public:
        static DBInterface* dbInterface()
        {
            if(_interface == NULL)
            {
                _interface = new DBInterface();
            }

            return _interface;
        }

    public:
        void timerEvent(QTimerEvent *event);

    private slots:
        void start();

    public slots:
        void updateIntradayHistoryData(const InstrumentId, const QListHistoryBarData&, const int frequency, const bool forcedWrite = false);
        void updateDailyHistoryData(const InstrumentId, const QListHistoryBarData&, const bool forcedWrite = false );

};

#endif // DBINTERFACE_H
