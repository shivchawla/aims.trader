#include "StrategyLinkedSpreadDetailDb.h"

StrategyLinkedSpreadDetailDb::StrategyLinkedSpreadDetailDb()
{
}


uint StrategyLinkedSpreadDetailDb::insertSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& createdDate)
{
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into SpreadDetail (RunId, SpreadDetailId, ValueBought, ValueSold, Commission, "
                  " CreatedDate, UpdateDate) Values(:RunId, :SpreadDetailId, :ValueBought, :ValueSold, "
                  ":Commission, :CreatedDate, :UpdatedDate)"
    );

    query.bindValue(":RunId", runId);
    query.bindValue(":SpreadDetailId", spreadDetailId);
    query.bindValue(":ValueBought", vBought);
    query.bindValue(":ValueSold", vSold);
    query.bindValue(":Commission", commission);
    query.bindValue(":CreatedDate", createdDate);
    query.bindValue(":UpdatedDate", createdDate);

    //execute
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
    return 0;
    }
    qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}

uint StrategyLinkedSpreadDetailDb::updateSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& lastUpdated)
{
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("update SpreadDetail set ValueBought = :ValueBought, ValueSold = :ValueSold, Commission = :Commission, "
                  "UpdateDate = :UpdatedDate where RunId = :RunId and SpreadDetailId = :SpreadDetailId"
    );

    query.bindValue(":RunId", runId);
    query.bindValue(":SpreadDetailId", spreadDetailId);
    query.bindValue(":ValueBought", vBought);
    query.bindValue(":ValueSold", vSold);
    query.bindValue(":Commission", commission);
    query.bindValue(":UpdatedDate", lastUpdated);

    //execute
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
    return 0;
    }
    qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}
