#include <DataAccess/IntradayHistoryBarDb.h>
#include <Data/HistoryBarData.h>

IntradayHistoryBarDb::IntradayHistoryBarDb():DbBase(){}

IntradayHistoryBarDb::~IntradayHistoryBarDb(){}

HistoryBarData IntradayHistoryBarDb::getIntradayHistoryBarById(const InstrumentId &instrumentId) {
    return HistoryBarData(); // for now
}


ulong IntradayHistoryBarDb::insertIntradayHistoryBars(const QHash<uint, QList<HistoryBarData> >& data, const int frequency)
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
   query.prepare("Insert IGNORE into IntradayHistoryBar(HistoryDateTime, Open, Close, High, Low, Volume, InstrumentId, Frequency) "
                 "Values(:HistoryDateTime, :Open, :Close, :High, :Low, :Volume, :InstrumentId, :Frequency) "
                 );

   int numRecords = 0;
   foreach(InstrumentId instrumentId, data.keys()){
       numRecords += data[instrumentId].length();
   }

   QVariantList dateTime;
   dateTime.reserve(numRecords);
   QVariantList open;
   open.reserve(numRecords);
   QVariantList close;
   close.reserve(numRecords);
   QVariantList high;
   high.reserve(numRecords);
   QVariantList low;
   low.reserve(numRecords);
   QVariantList volume;
   volume.reserve(numRecords);
   QVariantList instrumentIds;
   instrumentIds.reserve(numRecords);
   QVariantList freq;
   freq.reserve(numRecords);

    int ctr=0;
    foreach(InstrumentId instrumentId, data.keys()){
        QList<HistoryBarData> list = data[instrumentId];
        foreach(HistoryBarData barData, list) {
            dateTime << barData.historyDateTime;
            open << barData.open;
            close << barData.close;
            high << barData.high;
            low << barData.low;
            volume << barData.volume;
            InstrumentId id = instrumentId;
            instrumentIds << id;
            int f = frequency;
            freq << f;

        }
    }

    query.addBindValue(dateTime);
    query.addBindValue(open);
    query.addBindValue(close);
    query.addBindValue(high);
    query.addBindValue(low);
    query.addBindValue(volume);
    query.addBindValue(instrumentIds);
    query.addBindValue(freq);

        //barData->instrumentId = instrumentId;
        bool res = query.execBatch();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of minute history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        ctr += (res ? 1 : 0);
        //}
    //}

    db.commit();

//    if (numRecords == ctr) {
//        qDebug() << query.executedQuery() << endl;
//        qDebug() << "Error in bulk insert of minute history bar data" << endl;
//        qDebug() << query.lastError().text() << endl;
//    }


    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE IntradayHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();

    return ctr;
}


ulong IntradayHistoryBarDb::insertIntradayHistoryBars(const QList<HistoryBarData>& list, const InstrumentId& instrumentId, const int frequency )
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
    query.prepare("Insert IGNORE into IntradayHistoryBar(HistoryDateTime, Open, Close, High, Low, Volume, InstrumentId, Frequency) "
                  "Values(:HistoryDateTime, :Open, :Close, :High, :Low, :Volume, :InstrumentId, :Frequency) "
                  );

    int numRecords = list.length();
    QVariantList dateTime;
    dateTime.reserve(numRecords);
    QVariantList open;
    open.reserve(numRecords);
    QVariantList close;
    close.reserve(numRecords);
    QVariantList high;
    high.reserve(numRecords);
    QVariantList low;
    low.reserve(numRecords);
    QVariantList volume;
    volume.reserve(numRecords);
    QVariantList instrumentIds;
    instrumentIds.reserve(numRecords);

    QVariantList freq;
    freq.reserve(numRecords);

    foreach(HistoryBarData barData, list) {
        dateTime << barData.historyDateTime;
        open << barData.open;
        close << barData.close;
        high << barData.high;
        low << barData.low;
        volume << barData.volume;
        InstrumentId id = instrumentId;
        instrumentIds << id;
        int f = frequency;
        freq << f;
    }
        query.addBindValue(dateTime);
        query.addBindValue(open);
        query.addBindValue(close);
        query.addBindValue(high);
        query.addBindValue(low);
        query.addBindValue(volume);
        query.addBindValue(instrumentIds);
        query.addBindValue(freq);

        //barData->instrumentId = instrumentId;
        bool res = query.execBatch();
        if (!res) {
            qDebug() << query.executedQuery() << endl;
            qDebug() << "Error in bulk insert of minute history bar data" << endl;
            qDebug() << query.lastError().text() << endl;
        }
        //ctr += (res ? 1 : 0);

    db.commit();

//    if (ctr < list.count()) {
//        qDebug() << query.executedQuery() << endl;
//        qDebug() << "Error in bulk insert of minute history bar data" << endl;
//        qDebug() << query.lastError().text() << endl;
//    }


    QSqlQuery turnOnKeysQuery = getBlankQuery();
    turnOnKeysQuery.prepare("ALTER TABLE IntradayHistoryBar ENABLE KEYS");
    turnOnKeysQuery.exec();

    db.close();

    return 0;
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


uint IntradayHistoryBarDb :: write(const InstrumentId instrumentId, const QList<HistoryBarData*>& list, const int frequency)
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
   query.prepare("Insert IGNORE into IntradayHistoryBar(HistoryDateTime, Open, Close, High, Low, Volume, InstrumentId, Frequency) "
                 "Values(:HistoryDateTime, :Open, :Close, :High, :Low, :Volume, :InstrumentId, :Frequency) ");

         uint numRecords = list.length();
         QVariantList dateTime;
         dateTime.reserve(numRecords);
         QVariantList open;
         open.reserve(numRecords);
         QVariantList close;
         close.reserve(numRecords);
         QVariantList high;
         high.reserve(numRecords);
         QVariantList low;
         low.reserve(numRecords);
         QVariantList volume;
         volume.reserve(numRecords);
         QVariantList instrumentIds;
         instrumentIds.reserve(numRecords);

         QVariantList freq;
         freq.reserve(numRecords);

         foreach(HistoryBarData* barData, list) {
             dateTime << barData->historyDateTime;
             open << barData->open;
             close << barData->close;
             high << barData->high;
             low << barData->low;
             volume << barData->volume;
             InstrumentId id = instrumentId;
             instrumentIds << id;
             int f = frequency;
             freq << f;
         }
             query.addBindValue(dateTime);
             query.addBindValue(open);
             query.addBindValue(close);
             query.addBindValue(high);
             query.addBindValue(low);
             query.addBindValue(volume);
             query.addBindValue(instrumentIds);
             query.addBindValue(freq);

             //barData->instrumentId = instrumentId;
             bool res = query.execBatch();
             if (!res) {
                 qDebug() << query.executedQuery() << endl;
                 qDebug() << "Error in bulk insert of minute history bar data" << endl;
                 qDebug() << query.lastError().text() << endl;
             }
             else

             //ctr += (res ? 1 : 0);

         db.commit();

     //    if (ctr < list.count()) {
     //        qDebug() << query.executedQuery() << endl;
     //        qDebug() << "Error in bulk insert of minute history bar data" << endl;
     //        qDebug() << query.lastError().text() << endl;
     //    }


         QSqlQuery turnOnKeysQuery = getBlankQuery();
         turnOnKeysQuery.prepare("ALTER TABLE IntradayHistoryBar ENABLE KEYS");
         turnOnKeysQuery.exec();

         db.close();

         return res ? numRecords : 0;
}
