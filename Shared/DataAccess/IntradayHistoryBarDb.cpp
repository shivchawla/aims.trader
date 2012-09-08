#include <DataAccess/IntradayHistoryBarDb.h>
#include <Data/HistoryBarData.h>

IntradayHistoryBarDb::IntradayHistoryBarDb():DbBase(){}

IntradayHistoryBarDb::~IntradayHistoryBarDb(){}

HistoryBarData* IntradayHistoryBarDb::getIntradayHistoryBarById(const InstrumentId &instrumentId) {
    return NULL; // for now
}

ulong IntradayHistoryBarDb::insertIntradayHistoryBars(const QList<HistoryBarData*>& list, const InstrumentId &instrumentId)
{
    //check database if available to work with
    if (!openDatabase()) {
        return 0; //to signify zero inserted rows
    }

    QSqlQuery turnOffKeysQuery = getBlankQuery();
    turnOffKeysQuery.prepare("ALTER TABLE IntradayHistoryBar DISABLE KEYS");
    turnOffKeysQuery.exec();

    //prepare statement
    QSqlQuery query = getBlankQuery();

    db.transaction();

     //Old bulk insert method
    query.prepare("Insert into IntradayHistoryBar(HistoryDateTime, Open, Close, High, Low, Volume, InstrumentId) "
                  "Values(:HistoryDateTime, :Open, :Close, :High, :Low, :Volume, :InstrumentId) "
                  );

    ulong ctr=0;
    foreach(HistoryBarData* barData, list) {
        query.bindValue(":HistoryDateTime", barData->historyDateTime);
        query.bindValue(":Open", barData->open);
        query.bindValue(":Close", barData->close);
        query.bindValue(":High", barData->high);
        query.bindValue(":Low", barData->low);
        query.bindValue(":Volume", barData->volume);
        query.bindValue(":InstrumentId", instrumentId);

        //barData->instrumentId = instrumentId;
        bool res = query.exec();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of minute history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        ctr += (res ? 1 : 0);
    }

    db.commit();

    if (ctr < list.count()) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Error in bulk insert of minute history bar data" << endl;
        qDebug() << query.lastError().text() << endl;
    }


    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE IntradayHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();

    return ctr;
}


void IntradayHistoryBarDb::deleteOldIntradayRecords(const QDateTime& deleteBeforeDateTime)
{
    //check database if available to work with
    if (!openDatabase()) {
        return; //to signify zero inserted rows
    }

    QSqlQuery turnOffKeysQuery = getBlankQuery();
    turnOffKeysQuery.prepare("ALTER TABLE IntradayHistoryBar DISABLE KEYS");
    turnOffKeysQuery.exec();


    db.transaction();

    QSqlQuery query = getBlankQuery();
     //Old bulk insert method
    query.prepare("delete from IntradayHistoryBar"
                  "where HistoryDateTime < :HistoryDateTime"
                  );

    query.bindValue(":HistoryDateTime", deleteBeforeDateTime);
    query.exec();

    db.commit();

    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE IntradayHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();
}
