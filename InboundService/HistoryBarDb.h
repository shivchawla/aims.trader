#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "HistoryBarData.h"

class HistoryBarDb : public DbBase
{
private:
	enum HistoryBar {
		HISTORY_BAR_ID,
		HISTORY_DATE,
		OPEN,
		CLOSE,
		HIGH,
		LOW,
		VOLUME,
		UPDATED_BY,
		UPDATED_DATE,
		INSTRUMENT_ID
	};

	//QSqlDatabase db;

public:
	HistoryBarDb(void);
	~HistoryBarDb(void);
	HistoryBarData* GetHistoryBarById(QUuid id);
	unsigned int InsertHistoryBar(HistoryBarData data);
	unsigned int InsertHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low, qint32 volume, QString updatedBy, QDateTime updatedDate, QUuid instrumentId);
};

