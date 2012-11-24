#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "Data/HistoryBarData.h"
#include <myglobal.h>

class DailyHistoryBarDb : public DbBase
{
private:
    enum DailyHistoryBar {
        HistoryDateTime,
        Open,
        Close,
        High,
        Low,
        Volume
    };

public:
    DailyHistoryBarDb();
    ~DailyHistoryBarDb();
    HistoryBarData getDailyHistoryBarById(uint id);
    unsigned int insertDailyHistoryBar(const InstrumentId instrumentId, const HistoryBarData& data);
    unsigned int insertDailyHistoryBar(const QDateTime& historyDateTime, const float open, const float close, const float high, const float low,
                                       const uint volume, const InstrumentId instrumentId);
    unsigned int insertDailyHistoryBars(const QList<HistoryBarData>& list, const InstrumentId instrumentId);
    uint insertDailyHistoryBars(const QHash<InstrumentId, QList<HistoryBarData> >&);
    uint write(const InstrumentId, const QList<HistoryBarData*>&, const uint);
    QString getName() {return  "DailyHistoryBarDb";}


    QDateTime getLastHistoryDate(const InstrumentId instrumentId);

};

