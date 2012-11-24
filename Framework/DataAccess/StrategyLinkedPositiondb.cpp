#include "StrategyLinkedPositiondb.h"

StrategyLinkedPositionDb::StrategyLinkedPositionDb()
{}

QHash<uint, QHash<uint, uint> > StrategyLinkedPositionDb::getStrategyLinkedPositions(const uint runId)
{
    QHash<uint, QHash<uint, uint> > strategyLinkedPositions;
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return QHash<uint, QHash<uint, uint> >();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select RunId, StrategyId, InstrumentId, PositionId "
                  "from StrategyLinkedPosition2 where RunId = :RunId ");

    query.bindValue(":RunId", runId);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return QHash<uint, QHash<uint, uint> >();
    }

    while(query.next())
    {
        uint strategyId = query.value(1).toUInt();
        uint instrumentId = query.value(2).toUInt();
        uint positionId = query.value(3).toUInt();

        QHash<uint, uint> instrumentPositions;
        if((instrumentPositions = strategyLinkedPositions.value(strategyId, QHash<uint, uint>())) == QHash<uint,uint>())
        {
            instrumentPositions = strategyLinkedPositions[strategyId] = QHash<uint, uint>();
        }

        if(instrumentPositions.value(instrumentId,-1) !=-1)
        {
            instrumentPositions[instrumentId] = positionId;
        }
    }

    query.finish();
    db.close();
    return strategyLinkedPositions;

}

void StrategyLinkedPositionDb::insertRow(const uint runId, const uint strategyId, const uint instrumentId, const uint positionId)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("insert into StrategyLinkedPosition2 (RunId, StrategyId, InstrumentId, PositionId) "
                             "values(:RunId, :StrategyId, :InstrumentId, :PositionId)");

    query.bindValue(":RunId", runId);
    query.bindValue(":StrategyId",strategyId);
    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":PositionId",positionId);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
    }

}



