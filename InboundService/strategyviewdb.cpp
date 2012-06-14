#include "strategyviewdb.h"

StrategyViewDb :: StrategyViewDb(void) {

}

StrategyViewDb :: ~StrategyViewDb(void) {

}

//Methods

StrategyViewData* StrategyViewDb :: getStrategyViewById(QUuid id) {
    qDebug() << "Received " << id << endl;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }

    QSqlQuery query;
    query.prepare("select StrategyId, Name, ParentStrategyId, ParentName, Since, UsedInTrading from StrategyView "
                  "where StrategyId = StrToUuid(:StrategyId) ");
    query.bindValue(":StrategyId", QVariant(id));
    query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
        query.finish();
        db.close();
        qDebug() << "No row found for StrategyId " << id << " in StrategyView " << endl;
        return NULL;
    }

    StrategyViewData *item = new StrategyViewData();
    item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
    item->name = query.value(Name).toString();
    item->parentStrategyId = QUuid::fromRfc4122(query.value(ParentStrategyId).toByteArray());
    item->parentName = query.value(ParentName).toString();
    item->since = query.value(Since).toDateTime();
    item->usedInTrading = query.value(UsedInTrading).toBool();

    qDebug() << item->strategyId << endl;
    query.finish();
    db.close();
    return item;
}

QList<StrategyViewData*> StrategyViewDb :: getStrategyViews() {
    QList<StrategyViewData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("select StrategyId, Name, ParentStrategyId, ParentName, Since, UsedInTrading from StrategyView ");
    if (!result) {
        qDebug() << "No rows found in StrategyView " << endl;
        query.finish();
        db.close();
        return list;
    }

    while (query.next()) {
        StrategyViewData *item = new StrategyViewData();
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->name = query.value(Name).toString();
        item->parentStrategyId = QUuid::fromRfc4122(query.value(ParentStrategyId).toByteArray());
        item->parentName = query.value(ParentName).toString();
        item->since = query.value(Since).toDateTime();
        item->usedInTrading = query.value(UsedInTrading).toBool();
        list.append(item);
    }

    qDebug() << "Got " << query.size() << " rows from StrategyView" << endl;
    query.finish();
    db.close();
    return list;
}
