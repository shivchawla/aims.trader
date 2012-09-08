#include <QtSql/qsqlerror.h>
#include "DataAccess/DailyHistoryBarDb.h"
#include "Utils/BootStrapper.h"

DailyHistoryBarDb::DailyHistoryBarDb():DbBase()
{}

DailyHistoryBarDb::~DailyHistoryBarDb()
{}

HistoryBarData* DailyHistoryBarDb::getDailyHistoryBarById(uint id) {
    //qDebug() << "Received " << id.toString() << endl;

    if (!openDatabase()){
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select DailyHistoryBarId, HistoryDate, Open, Close, High, Low, Volume, InstrumentId from DailyHistoryBar"
                  " where DailyHistoryBarId = :DailyHistoryBarId ");
    query.bindValue(":DailyHistoryBarId", id);

	query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch DailyHistoryBarData row. Error: " << query.lastError().text() << endl;
        db.close();
		return NULL;
	}

    HistoryBarData *h = new HistoryBarData();

    h->historyDateTime = query.value(HistoryDateTime).toDateTime();
    h->open = query.value(Open).toFloat();
    h->close = query.value(Close).toFloat();
    h->high = query.value(High).toFloat();
    h->low = query.value(Low).toFloat();
    h->volume = query.value(Volume).toUInt();

    query.finish();
    db.close();

	return h;
}

uint DailyHistoryBarDb :: insertDailyHistoryBar(const InstrumentId instrumentId, const HistoryBarData& data) {
    return insertDailyHistoryBar(data.historyDateTime, data.open, data.close, data.high, data.low, data.volume,
                             instrumentId);
}

uint DailyHistoryBarDb :: insertDailyHistoryBar(const QDateTime& timeStamp, const float open, const float close, const float high, const float low,
                                                const uint volume, const InstrumentId instrumentId) {
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

    query.bindValue(":HistoryDate", timeStamp);
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

uint DailyHistoryBarDb :: insertDailyHistoryBars(const QList<HistoryBarData*>& list, const InstrumentId instrumentId) {
    //check database if available to work with
    if (!openDatabase()) {
        return 0; //to signify zero inserted rows
    }

    QSqlQuery turnOffKeysQuery = getBlankQuery();
    turnOffKeysQuery.prepare("ALTER TABLE DailyHistoryBar DISABLE KEYS");
    turnOffKeysQuery.exec();

    //prepare statement
    QSqlQuery query = getBlankQuery();

    db.transaction();

     //Old bulk insert method
    query.prepare("Insert into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:TimeStamp, :Open, :Close, :High, :Low, :Volume, :InstrumentId) "
                  );

    int ctr=0;
    foreach(HistoryBarData* barData, list) {
        query.bindValue(":HistoryDate", barData->historyDateTime);
        query.bindValue(":Open", barData->open);
        query.bindValue(":Close", barData->close);
        query.bindValue(":High", barData->high);
        query.bindValue(":Low", barData->low);
        query.bindValue(":Volume", barData->volume);
        query.bindValue(":InstrumentId", instrumentId);

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

    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE DailyHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

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
