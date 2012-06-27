#include <QByteArray>
#include <QtSql/qsqlerror.h>
#include "DailyHistoryBarDb.h"
#include "Utils/BootStrapper.h"
#include <QVariant>
#include <QUuid>

DailyHistoryBarDb::DailyHistoryBarDb(void)
{
}


DailyHistoryBarDb::~DailyHistoryBarDb(void)
{
}

DailyHistoryBarData* DailyHistoryBarDb::getDailyHistoryBarById(QUuid id) {
	qDebug() << "Received " << id.toString() << endl;

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }
    //else qDebug() << "Database connected!!" << endl;

    QSqlQuery query;
    query.prepare("select DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume, UpdatedBy, UpdatedDate, InstrumentId from DailyHistoryBar where DailyHistoryBarId = Unhex(Replace(Replace(Replace(:DailyHistoryBarId, '{', ''), '}', ''), '-', ''))");
    query.bindValue(":DailyHistoryBarId", QVariant(id));
    //query.bindValue(":DailyHistoryBarId", id.toByteArray());
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
    DailyHistoryBarData *h = new DailyHistoryBarData();

    h->dailyHistoryBarId = QUuid::fromRfc4122(query.value(DailyHistoryBarId).toByteArray());
    h->historyDate = query.value(HistoryDate).toDateTime();
    h->open = query.value(Open).toFloat();
    h->close = query.value(Close).toFloat();
    h->high = query.value(High).toFloat();
    h->low = query.value(Low).toFloat();
    h->volume = query.value(Volume).toUInt();
    h->updatedBy = query.value(UpdatedBy).toString();
    h->updatedDate = query.value(UpdatedDate).toDateTime();
    h->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());

    qDebug() << h->dailyHistoryBarId << " " << h->historyDate << " " << h->volume << " " << h->instrumentId << endl;
    query.finish();
    db.close();

	return h;
}

unsigned int DailyHistoryBarDb :: insertDailyHistoryBar(DailyHistoryBarData data) {
    return 	insertDailyHistoryBar(data.historyDate, data.open, data.close, data.high, data.low, data.volume,
							data.updatedBy, data.updatedDate, data.instrumentId);
}

unsigned int DailyHistoryBarDb :: insertDailyHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low, qint32 volume, QString updatedBy, QDateTime updatedDate, QUuid instrumentId) {
	//check database if available to work with
	if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }
    //else qDebug() << "Database connected!!" << endl;

	//prepare statement
	QSqlQuery query;
    query.prepare("Insert into DailyHistoryBar(DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume, UpdatedBy, UpdatedDate, InstrumentId) "
                  "Values(Unhex(Replace(Replace(Replace(:DailyHistoryBarId, '{', ''), '}', ''), '-', '')), "
                  ":HistoryDate, :Open, :Close, :High, :Low, :Volume, :UpdatedBy, :UpdatedDate, "
                  "Unhex(Replace(Replace(Replace(:InstrumentId, '{', ''), '}', ''), '-', '')) "
                  ") On duplicate key update Open = :Open2, Close=:Close2, High=:High2, Low=:Low2, Volume=:Volume2, UpdatedDate=:UpdatedDate2 "
                  );
    query.bindValue(":DailyHistoryBarId", QVariant(QUuid :: createUuid()));
    query.bindValue(":HistoryDate", historyDateTime);
    query.bindValue(":Open", open);
    query.bindValue(":Close", close);
    query.bindValue(":High", high);
    query.bindValue(":Low", low);
    query.bindValue(":Volume", volume);
    query.bindValue(":UpdatedBy", updatedBy);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":InstrumentId", QVariant(instrumentId));

    query.bindValue(":Open2", open);
    query.bindValue(":Close2", close);
    query.bindValue(":High2", high);
    query.bindValue(":Low2", low);
    query.bindValue(":Volume2", volume);
    query.bindValue(":UpdatedDate2", updatedDate);

	qDebug() << "Binding to instrument id " << instrumentId.toString() << endl;
	//execute

	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
		return 0;
	}
    //	qDebug() << "Inserted a history bar row" << endl;

	return 1;
}
