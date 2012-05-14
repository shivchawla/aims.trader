#include <QByteArray>
#include <QtSql/qsqlerror.h>
#include "stdafx.h"
#include "HistoryBarDb.h"
#include "BootStrapper.h"
#include <QVariant>
#include <QUuid>

HistoryBarDb::HistoryBarDb(void)
{
}


HistoryBarDb::~HistoryBarDb(void)
{
}

HistoryBarData* HistoryBarDb::GetHistoryBarById(QUuid id) {
	qDebug() << "Received " << id.toString() << endl;

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }
    else qDebug() << "Database connected!!" << endl;

    QSqlQuery query;
    query.prepare("select HISTORY_BAR_ID, HISTORY_DATE, OPEN, CLOSE, HIGH, LOW, VOLUME, UPDATED_BY, UPDATED_DATE, INSTRUMENT_ID from HISTORYBAR where HISTORY_BAR_ID=Unhex(Replace(Replace(Replace(:HISTORY_BAR_ID, '{', ''), '}', ''), '-', ''))");
    query.bindValue(":HISTORY_BAR_ID", QVariant(id));
	//query.bindValue(":HISTORY_BAR_ID", id.toByteArray()); 
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	HistoryBarData *h = new HistoryBarData();

    h->historyBarId = QUuid::fromRfc4122(query.value(HISTORY_BAR_ID).toByteArray());
	h->historyDate = query.value(HISTORY_DATE).toDateTime();
	h->open = query.value(OPEN).toFloat();
	h->close = query.value(CLOSE).toFloat();
	h->high = query.value(HIGH).toFloat();
	h->low = query.value(LOW).toFloat();
	h->volume = query.value(VOLUME).toUInt();
	h->updatedBy = query.value(UPDATED_BY).toString();
	h->updatedDate = query.value(UPDATED_DATE).toDateTime();
	h->instrumentId = QUuid::fromRfc4122(query.value(INSTRUMENT_ID).toByteArray());

    qDebug() << h->historyBarId << " " << h->historyDate << " " << h->volume << " " << h->instrumentId << endl;
    query.finish();
    db.close();

	return h;
}

unsigned int HistoryBarDb :: InsertHistoryBar(HistoryBarData data) {
	return 	InsertHistoryBar(data.historyDate, data.open, data.close, data.high, data.low, data.volume,
							data.updatedBy, data.updatedDate, data.instrumentId);
}

unsigned int HistoryBarDb :: InsertHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low, qint32 volume, QString updatedBy, QDateTime updatedDate, QUuid instrumentId) {
	//check database if available to work with
	if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }
	else qDebug() << "Database connected!!" << endl;

	//prepare statement
	QSqlQuery query;
    query.prepare("Insert into HistoryBar(HISTORY_BAR_ID, HISTORY_DATE, OPEN, CLOSE, HIGH, LOW, VOLUME, UPDATED_BY, UPDATED_DATE, INSTRUMENT_ID) "
                  "Values(Unhex(Replace(Replace(Replace(:HISTORY_BAR_ID, '{', ''), '}', ''), '-', '')), "
                  ":HISTORY_DATE, :OPEN, :CLOSE, :HIGH, :LOW, :VOLUME, :UPDATED_BY, :UPDATED_DATE, "
                  "Unhex(Replace(Replace(Replace(:INSTRUMENT_ID, '{', ''), '}', ''), '-', '')) "
                  ") On duplicate key update OPEN = :OPEN2, CLOSE=:CLOSE2, HIGH=:HIGH2, LOW=:LOW2, VOLUME=:VOLUME2, UPDATED_DATE=:UPDATED_DATE2 "
                  );
    query.bindValue(":HISTORY_BAR_ID", QVariant(QUuid :: createUuid()));
	query.bindValue(":HISTORY_DATE", historyDateTime);
	query.bindValue(":OPEN", open);
	query.bindValue(":CLOSE", close);
	query.bindValue(":HIGH", high);
	query.bindValue(":LOW", low);
	query.bindValue(":VOLUME", volume);
	query.bindValue(":UPDATED_BY", updatedBy);
	query.bindValue(":UPDATED_DATE", updatedDate);
    query.bindValue(":INSTRUMENT_ID", QVariant(instrumentId));

    query.bindValue(":OPEN2", open);
    query.bindValue(":CLOSE2", close);
    query.bindValue(":HIGH2", high);
    query.bindValue(":LOW2", low);
    query.bindValue(":VOLUME2", volume);
    query.bindValue(":UPDATED_DATE2", updatedDate);

	qDebug() << "Binding to instrument id " << instrumentId.toString() << endl;
	//execute

	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
		return 0;
	}
		qDebug() << "Inserted a history bar row" << endl;

	return 1;
}
