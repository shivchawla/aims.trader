#include <QtSql/qsqlerror.h>
#include "DataAccess/DailyHistoryBarDb.h"
#include "Utils/BootStrapper.h"

DailyHistoryBarDb::DailyHistoryBarDb():DbBase()
{}

DailyHistoryBarDb::~DailyHistoryBarDb()
{}

HistoryBarData DailyHistoryBarDb::getDailyHistoryBarById(uint id) {
    //qDebug() << "Received " << id.toString() << endl;

    if (!openDatabase()){
        return HistoryBarData();
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
        return HistoryBarData();
	}

    HistoryBarData h;// = new HistoryBarData();

    h.historyDateTime = query.value(HistoryDateTime).toDateTime();
    h.open = query.value(Open).toDouble();
    h.close = query.value(Close).toDouble();
    h.high = query.value(High).toDouble();
    h.low = query.value(Low).toDouble();
    h.volume = query.value(Volume).toUInt();

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

uint DailyHistoryBarDb::insertDailyHistoryBars(const QHash<InstrumentId, QList<HistoryBarData> >& data)
{
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
    query.prepare("Insert IGNORE into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:TimeStamp, :Open, :Close, :High, :Low, :Volume, :InstrumentId) ");// ON DUPLICATE KEY UPDATE "
                  //" Open = :Open, Close = :Close, High = :High, Low = :Low, Volume = :Volume ");

    QVariantList dateTime;
    QVariantList open;
    QVariantList close;
    QVariantList high;
    QVariantList low;
    QVariantList volume;
    QVariantList instrumentIds;

    int ctr=0;
    uint numRecords = 0;
    foreach(InstrumentId instrumentId, data.keys()){
        QList<HistoryBarData> list = data[instrumentId];
        foreach(HistoryBarData barData, list) {
            numRecords++;
            dateTime << barData.historyDateTime;
            open << barData.open;
            close << barData.close;
            high << barData.high;
            low << barData.low;
            volume << barData.volume;
            InstrumentId id = instrumentId;
            instrumentIds << id;
        }
    }

    query.addBindValue(dateTime);
    query.addBindValue(open);
    query.addBindValue(close);
    query.addBindValue(high);
    query.addBindValue(low);
    query.addBindValue(volume);
    query.addBindValue(instrumentIds);

    bool res = query.execBatch();
    if (!res) {
        //qDebug()<<instrumentId;
//                qDebug()<< barData.historyDateTime;
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Error in bulk insert of daily history bar data" << endl;
        qDebug() << query.lastError().text() << endl;
    }

    db.commit();

//    if (ctr < numRecords) {
//        qDebug() << query.executedQuery() << endl;
//        qDebug() << "Error in bulk insert of daily history bar data" << endl;
//        qDebug() << query.lastError().text() << endl;
//    }

    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE DailyHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();
    return ctr;
}

uint DailyHistoryBarDb :: insertDailyHistoryBars(const QList<HistoryBarData>& list, const InstrumentId instrumentId) {
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
    query.prepare("Insert IGNORE into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:TimeStamp, :Open, :Close, :High, :Low, :Volume, :InstrumentId) ");// ON DUPLICATE KEY UPDATE "
                  //" Open = :Open, Close = :Close, High = :High, Low = :Low, Volume = :Volume ");


    int ctr=0;

    QVariantList dateTime;
    QVariantList open;
    QVariantList close;
    QVariantList high;
    QVariantList low;
    QVariantList volume;
    QVariantList instrumentIds;

    foreach(HistoryBarData barData, list) {
        dateTime << barData.historyDateTime;
        open << barData.open;
        close << barData.close;
        high << barData.high;
        low << barData.low;
        volume << barData.volume;
        InstrumentId id = instrumentId;
        instrumentIds << id;
    }
        query.addBindValue(dateTime);
        query.addBindValue(open);
        query.addBindValue(close);
        query.addBindValue(high);
        query.addBindValue(low);
        query.addBindValue(volume);
        query.addBindValue(instrumentIds);

        //qDebug()<<barData.historyDateTime<<" "<<instrumentId;


        bool res = query.execBatch();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of daily history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        else
            qDebug()<<query.numRowsAffected();
   //     ctr += (res ? 1 : 0);
    //}

    db.commit();



//    if (ctr < list.count()) {
//        qDebug() << query.executedQuery() << endl;
//        qDebug() << "Error in bulk insert of daily history bar data" << endl;
//        qDebug() << query.lastError().text() << endl;
//    }

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

uint DailyHistoryBarDb :: write(const InstrumentId instrumentId, const QList<HistoryBarData*>& list, const uint frequency)
{
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
    query.prepare("Insert IGNORE into DailyHistoryBar(HistoryDate, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:TimeStamp, :Open, :Close, :High, :Low, :Volume, :InstrumentId) ");// ON DUPLICATE KEY UPDATE "
                  //" Open = :Open, Close = :Close, High = :High, Low = :Low, Volume = :Volume ");


    int ctr=0;

    QVariantList dateTime;
    QVariantList open;
    QVariantList close;
    QVariantList high;
    QVariantList low;
    QVariantList volume;
    QVariantList instrumentIds;

    foreach(HistoryBarData* barData, list) {
        dateTime << barData->historyDateTime;
        open << barData->open;
        close << barData->close;
        high << barData->high;
        low << barData->low;
        volume << barData->volume;
        InstrumentId id = instrumentId;
        instrumentIds << id;
    }
        query.addBindValue(dateTime);
        query.addBindValue(open);
        query.addBindValue(close);
        query.addBindValue(high);
        query.addBindValue(low);
        query.addBindValue(volume);
        query.addBindValue(instrumentIds);

        bool res = query.execBatch();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of daily history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        else
            qDebug()<<query.numRowsAffected();

    db.commit();

    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE DailyHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();
    return ctr;
}

