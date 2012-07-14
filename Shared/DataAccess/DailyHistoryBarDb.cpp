#include <QtSql/qsqlerror.h>
#include "DataAccess/DailyHistoryBarDb.h"
#include "Utils/BootStrapper.h"

DailyHistoryBarDb::DailyHistoryBarDb():DbBase()
{
    _pendingRecords = 0;
}


DailyHistoryBarDb::~DailyHistoryBarDb()
{
}

DailyHistoryBarData* DailyHistoryBarDb::getDailyHistoryBarById(uint id) {
    //qDebug() << "Received " << id.toString() << endl;

    if (!openDatabase()){
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume, InstrumentId from DailyHistoryBar"
                  " where DailyHistoryBarId = :DailyHistoryBarId ");
    query.bindValue(":DailyHistoryBarId", id);

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

    h->dailyHistoryBarId = query.value(DailyHistoryBarId).toUInt();
    h->historyDate = query.value(HistoryDate).toDateTime();
    h->open = query.value(Open).toFloat();
    h->close = query.value(Close).toFloat();
    h->high = query.value(High).toFloat();
    h->low = query.value(Low).toFloat();
    h->volume = query.value(Volume).toUInt();
    h->instrumentId = query.value(InstrumentId).toUInt();

    query.finish();
    db.close();

	return h;
}

uint DailyHistoryBarDb :: insertDailyHistoryBar(const DailyHistoryBarData& data) {
    return insertDailyHistoryBar(data.historyDate, data.open, data.close, data.high, data.low, data.volume,
                             data.instrumentId);
}

uint DailyHistoryBarDb :: insertDailyHistoryBar(QDateTime historyDate, float open, float close, float high, float low,
                                                uint volume, uint instrumentId) {
	//check database if available to work with
    if (!openDatabase()) {
        return NULL;
    }

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("Insert into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume,"
                  " InstrumentId) "
                  "Values( "
                  ":HistoryDate, :Open, :Close, :High, :Low, :Volume, "
                  ":InstrumentId) "
                  );

    query.bindValue(":HistoryDate", historyDate);
    query.bindValue(":Open", open);
    query.bindValue(":Close", close);
    query.bindValue(":High", high);
    query.bindValue(":Low", low);
    query.bindValue(":Volume", volume);
    query.bindValue(":InstrumentId", instrumentId);

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

uint DailyHistoryBarDb :: insertDailyHistoryBars(const QList<DailyHistoryBarData*>& list, uint instrumentId) {
    //check database if available to work with
    if (!openDatabase()) {
        return 0; //to signify zero inserted rows
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();

    db.transaction();

     //Old bulk insert method
    query.prepare("Insert into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:HistoryDate, :Open, :Close, :High, :Low, :Volume, :InstrumentId) "
                  );


    //QVariantList idList, historyDateList, openList, closeList, highList, lowList, volumeList, instrumentIdList;
    int ctr=0;
    foreach(DailyHistoryBarData* barData, list) {
        //idList << QVariant(uint :: createUuid());
        query.bindValue(":HistoryDate", barData->historyDate);
        query.bindValue(":Open", barData->open);
        query.bindValue(":Close", barData->close);
        query.bindValue(":High", barData->high);
        query.bindValue(":Low", barData->low);
        query.bindValue(":Volume", barData->volume);
        query.bindValue(":InstrumentId", instrumentId);

        barData->instrumentId = instrumentId;
        bool res = query.exec();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of daily history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        ctr += (res ? 1 : 0);
    }

    db.commit();

    if (ctr < list.count()) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Error in bulk insert of daily history bar data" << endl;
        qDebug() << query.lastError().text() << endl;
    }

    db.close();

    return ctr;
}

// this method is deperecated.

QDateTime DailyHistoryBarDb :: getLastHistoryDate(uint instrumentId) {
    if (!openDatabase()) {
        return QDateTime();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("SELECT max(HistoryDate) FROM StratTrader.DailyHistoryBar "
                  "where InstrumentId = :InstrumentId ");
    query.bindValue(":InstrumentId", instrumentId);

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
