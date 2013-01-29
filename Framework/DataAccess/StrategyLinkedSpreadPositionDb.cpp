#include "StrategyLinkedSpreadPositionDb.h"

StrategyLinkedSpreadPositionDb::StrategyLinkedSpreadPositionDb()
{}

QHash<uint, QHash<uint, QHash<uint, uint> > > StrategyLinkedSpreadPositionDb::getStrategyLinkedSpreadPositions(const uint runId)
{
    QHash< uint, QHash<uint, QHash<uint, uint> > > strategyLinkedSpreadPositions;
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return QHash<uint, QHash<uint, QHash<uint, uint> > >();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select RunId, StrategyId, SpreadId, InstrumentId, PositionId "
                  "from StrategyLinkedSpreadPosition where RunId = :RunId ");

    query.bindValue(":RunId", runId);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return QHash<uint, QHash<uint, QHash<uint, uint> > >();
    }

    while(query.next())
    {
        uint strategyId = query.value(1).toUInt();
        uint spreadId = query.value(2).toUInt();
        uint instrumentId = query.value(3).toUInt();
        uint positionId = query.value(4).toUInt();

        QHash<uint, QHash<uint, uint> > instrumentSpreadPositions;
        if((instrumentSpreadPositions = strategyLinkedSpreadPositions.value(strategyId, QHash<uint, QHash<uint, uint> >())) == QHash<uint, QHash<uint,uint> >())
        {
            instrumentSpreadPositions = strategyLinkedSpreadPositions[strategyId] = QHash<uint, QHash<uint, uint> >();
        }

        QHash<uint, uint> instrumentPositions;
        if(instrumentSpreadPositions.value(spreadId, QHash<uint, uint>()) == QHash<uint,uint>())
        {
            instrumentPositions = instrumentSpreadPositions[spreadId] = QHash<uint,uint>();
        }

        if(instrumentPositions.value(instrumentId,-1) !=-1)
        {
            instrumentPositions[instrumentId] = positionId;
        }
    }

    query.finish();
    db.close();
    return strategyLinkedSpreadPositions;

}

void StrategyLinkedSpreadPositionDb::insertRow(const uint runId, const uint strategyId, const uint spreadId, const uint instrumentId, const uint positionId)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("insert into StrategyLinkedSpreadPosition (RunId, StrategyId, SpreadId, InstrumentId, SpreadPositionId) "
                             "values(:RunId, :StrategyId, :SpreadId, :InstrumentId, :PositionId)");

    query.bindValue(":RunId", runId);
    query.bindValue(":StrategyId",strategyId);
    query.bindValue(":SpreadId", spreadId);
    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":SpreadPositionId", positionId);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
    }
}



