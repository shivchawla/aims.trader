#include "StrategyLinkedSpreadDb.h"

StrategyLinkedSpreadDb::StrategyLinkedSpreadDb():DbBase()
{
}


void StrategyLinkedSpreadDb::insertStrategyLinkedSpread(const uint runId, const uint strategyId, const uint spreadId, const int spreadDetailId)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("insert into StrategyLinkedSpread (RunId, StrategyId, SpreadId, SpreadDetailId) "
                             "values(:RunId, :StrategyId, :SpreadId, :SpreadDetailId)");

    query.bindValue(":RunId", runId);
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":SpreadId", spreadId);
    query.bindValue(":SpreadDetailId", spreadDetailId);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
    }
}
