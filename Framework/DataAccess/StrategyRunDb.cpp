#include "StrategyRunDb.h"


StrategyRunDb::StrategyRunDb():DbBase()
{}

uint StrategyRunDb::generateRunId()
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return -1;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select RunId from StrategyRun order by RunId desc LIMIT 1");

    bool result = query.exec();
    if (!result) {
        return -1;
    }

    query.next();
    uint runId = query.value(0).toUInt();

    return runId + 1;
}

uint StrategyRunDb::getLastRunId(const Mode mode)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return -1;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select RunId from StrategyRun where RunType = :RunType order by RunId desc LIMIT 1");

    query.bindValue(":RunType", getModeName(mode));
    bool result = query.exec();
    if (!result) {
        return -1;
    }

    query.next();
    uint lastRunId = query.value(0).toUInt();

    return lastRunId;
}


uint StrategyRunDb::insertStrategy(const uint runId, const Mode mode, const DbStrategyId strategyId, const QString& strategyName, const bool newPositionFlag)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return -1;
    }

    QSqlQuery query  =getBlankQuery();

    query.prepare("Insert into StrategyRun (RunId, StrategyId, StrategyName, NewPositionFlag, RunType, StartTime, EndTime, CreatedBy)"
                  "values(:RunId, :StrategyId, :StrategyName, :NewPositionFlag, :Mode, :StartDateTime, :EndDateTime, :CreatedBy)");

    query.bindValue(":RunId",runId);
    query.bindValue(":StrategyId",strategyId);
    query.bindValue(":StrategyName",strategyName);
    query.bindValue(":NewPositionFlag", newPositionFlag);
    query.bindValue(":Mode", QString::fromLatin1(getModeName(mode)));
    query.bindValue(":StartDateTime",QDateTime::currentDateTime());
    query.bindValue(":EndDateTime", QDateTime::currentDateTime());
    query.bindValue(":CreatedBy","schawla");

    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
    }

    return query.lastInsertId().toUInt();
}

