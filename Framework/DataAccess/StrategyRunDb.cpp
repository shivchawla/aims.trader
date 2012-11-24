#include "StrategyRunDb.h"


StrategyRunDb::StrategyRunDb()
{}

uint StrategyRunDb::generateRunId(const Mode mode)
{
    //uint runId;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return -1;
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("insert into StrategyRun (RunType, StartTime, EndTime, CreatedBy) "
                             "values(:Mode, :StartDateTime, :EndDateTime, :CreatedBy)");

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
