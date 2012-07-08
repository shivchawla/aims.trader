#include <QByteArray>
#include <QtSql/qsqlerror.h>
#include "DataAccess/DailyHistoryBarDb.h"
#include "Utils/BootStrapper.h"
#include <QVariant>
#include <QUuid>

DailyHistoryBarDb::DailyHistoryBarDb():DbBase()
{
    _pendingRecords = 0;
   // db.transaction();
}


DailyHistoryBarDb::~DailyHistoryBarDb()
{
}

DailyHistoryBarData* DailyHistoryBarDb::getDailyHistoryBarById(QUuid id) {
    //qDebug() << "Received " << id.toString() << endl;

    if (!openDatabase())
    {
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume, InstrumentId from DailyHistoryBar where DailyHistoryBarId = StrToUuid(:DailyHistoryBarId) ");
    query.bindValue(":DailyHistoryBarId", QVariant(id));

	query.exec();
    //qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch DailyHistoryBarData row. Error: " << query.lastError().text() << endl;
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
    h->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());

    //qDebug() << h->dailyHistoryBarId << " " << h->historyDate << " " << h->volume << " " << h->instrumentId << endl;
    query.finish();
    db.close();

	return h;
}

unsigned int DailyHistoryBarDb :: insertDailyHistoryBar(const DailyHistoryBarData& data) {
    return insertDailyHistoryBar(data.historyDate, data.open, data.close, data.high, data.low, data.volume,
                             data.instrumentId);
}

unsigned int DailyHistoryBarDb :: insertDailyHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low, qint32 volume, QUuid instrumentId) {
	//check database if available to work with
    if (!openDatabase())
    {
        return NULL;
    }

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("Insert into DailyHistoryBar(DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume,"
                  " InstrumentId) "
                  "Values(StrToUuid(:DailyHistoryBarId), "
                  ":HistoryDate, :Open, :Close, :High, :Low, :Volume, "
                  "StrToUuid(:InstrumentId) "
                  ") On duplicate key update Open = :Open2, Close=:Close2, High=:High2, Low=:Low2, Volume=:Volume2"
                  );

    query.bindValue(":DailyHistoryBarId", QVariant(QUuid :: createUuid()));
    query.bindValue(":HistoryDate", historyDateTime);
    query.bindValue(":Open", open);
    query.bindValue(":Close", close);
    query.bindValue(":High", high);
    query.bindValue(":Low", low);
    query.bindValue(":Volume", volume);
    query.bindValue(":InstrumentId", QVariant(instrumentId));

    query.bindValue(":Open2", open);
    query.bindValue(":Close2", close);
    query.bindValue(":High2", high);
    query.bindValue(":Low2", low);
    query.bindValue(":Volume2", volume);

    //qDebug() << "Binding to instrument id " << instrumentId.toString() << endl;
	//execute

	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
		return 0;
	}
    //qDebug() << "Inserted a history bar row" << endl;
    db.close();

	return 1;
}

unsigned int DailyHistoryBarDb :: insertDailyHistoryBars(const QList<DailyHistoryBarData*>& list, QUuid instrumentId) {
    //check database if available to work with
    if (!openDatabase())
    {
        return 0; //to signify zero inserted rows
    }

//    _pendingRecords +=list.length();

//    if(_pendingRecords < 100000)
//    {
//        bool transcation = db.transaction();
//    }

    //prepare statement
    QSqlQuery query = getBlankQuery();

    query.prepare("ALTER TABLE StratTrader.DailyHistoryBar DISABLE KEYS; SET FOREIGN_KEY_CHECKS = 0; SET UNIQUE_CHECKS = 0;SET AUTOCOMMIT = 0;");


    bool result = query.exec();

    query.clear();

    query.prepare("Insert into DailyHistoryBar(DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume,"
                  "InstrumentId) "
                  "Values(StrToUuid(?), "
                  "?, ?, ?, ?, ?, ?, StrToUuid(?) "
                  ") "
                  );

    QVariantList idList, historyDateList, openList, closeList, highList, lowList, volumeList, instrumentIdList;

    foreach(DailyHistoryBarData* barData, list) {
        idList << QVariant(QUuid :: createUuid());
        historyDateList << barData->historyDate;
        openList << barData->open;
        closeList << barData->close;
        highList << barData->high;
        lowList << barData->low;
        volumeList << barData->volume;
        instrumentIdList << QVariant(instrumentId);

        barData->instrumentId = instrumentId;
    }

    //bind
    query.addBindValue(idList);
    query.addBindValue(historyDateList);
    query.addBindValue(openList);
    query.addBindValue(closeList);
    query.addBindValue(highList);
    query.addBindValue(lowList);
    query.addBindValue(volumeList);
    query.addBindValue(instrumentIdList);

    //execute
    result = query.execBatch();
    if (!result) {
        qDebug() << "Couldn't insert daily history bar data rows for InstrumentId: " << instrumentId
                 << "Error: " << query.lastError().text() << " " << endl;
        qDebug() << query.lastQuery() << endl;
        return 0;
    }

    query.clear();
    query.prepare("SET UNIQUE_CHECKS = 1;"
                  "SET FOREIGN_KEY_CHECKS = 1;"
                  "ALTER TABLE `table_name` ENABLE KEYS;"
                  "COMMIT;");

    result = query.exec();

    db.close();


//    if(_pendingRecords > 100000)
//    {
//        db.commit();
//        _pendingRecords = 0;
//    }

    return list.count();
}

QDateTime DailyHistoryBarDb :: getLastHistoryDate(QUuid instrumentId) {
    if (!openDatabase()) {
        return QDateTime();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("SELECT max(HistoryDate) FROM StratTrader.DailyHistoryBar "
                  "where InstrumentId = StrToUuid(:InstrumentId) ");
    query.bindValue(":InstrumentId", QVariant(instrumentId));

    bool result = query.exec();
    //qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch InstrumentData row. Error: " << query.lastError().text() << endl;
        db.close();
        return QDateTime();
    }
    query.next();
    QDateTime lastHistoryDate = query.value(0).toDateTime();
    //qDebug() << "Returning " << lastHistoryDate << endl;

    query.finish();
    db.close();

    return lastHistoryDate;
}
