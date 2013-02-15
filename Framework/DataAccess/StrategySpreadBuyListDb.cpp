#include "StrategySpreadBuyListDb.h"

StrategySpreadBuyListDb::StrategySpreadBuyListDb():DbBase()
{
}

QList<SpreadData> StrategySpreadBuyListDb::getStrategySpreadBuyList(const QString& strategyName)
{
    QList<SpreadData> list;
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select Spread.SpreadId, Spread.InstrumentId1, Spread.InstrumentId2 from Spread "
                  " inner join StrategySpreadBuyList"
                  " on Spread.SpreadId = StrategySpreadBuyList.SpreadId"
                  " where StrategySpreadBuyList.StrategyName = :StrategyName ");
    query.bindValue(":StrategyName", strategyName);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        db.close();
        return list;
    }

    while(query.next()) {
        SpreadData d;
        d.spreadId = query.value(DbSpreadId).toUInt();
        d.instrumentId1 = query.value(InstrumentId1).toUInt();
        d.instrumentId2 = query.value(InstrumentId2).toUInt();
        list.append(d);
    }

    query.finish();
    db.close();

    return list;
}
