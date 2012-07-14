#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "Data/DailyHistoryBarData.h"

class DailyHistoryBarDb : public DbBase
{
private:
    enum DailyHistoryBar {
        DailyHistoryBarId,
        HistoryDate,
        Open,
        Close,
        High,
        Low,
        Volume,
        InstrumentId
	};

	//QSqlDatabase db;
    int _pendingRecords;

public:
    DailyHistoryBarDb();
    ~DailyHistoryBarDb();
    DailyHistoryBarData* getDailyHistoryBarById(uint id);
    unsigned int insertDailyHistoryBar(const DailyHistoryBarData& data);
    unsigned int insertDailyHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low,
                                       uint volume, uint instrumentId);
    unsigned int insertDailyHistoryBars(const QList<DailyHistoryBarData*>& list, uint instrumentId);
    QDateTime getLastHistoryDate(uint instrumentId);

};

