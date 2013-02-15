#include "SpreadDb.h"

SpreadDb::SpreadDb():DbBase()
{}

SpreadData SpreadDb::getSpreadData(const uint spreadId)
{
    if (!openDatabase()) {
        return SpreadData();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select SpreadId, InstrumentId1, InstrumentId2 from Spread where SpreadId = :SpreadId");

    query.bindValue(":SpreadId", spreadId);

    bool result = query.exec();
    if (!result) {
        qDebug() << query.lastError().text() << endl;
        return SpreadData();
    }

    SpreadData data;
    data.spreadId = query.value(DbSpreadId).toUInt();
    data.instrumentId1 = query.value(InstrumentId1).toUInt();
    data.instrumentId2 = query.value(InstrumentId2).toUInt();

    return data;
}
