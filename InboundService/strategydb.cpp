#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "strategydb.h"

StrategyDb :: StrategyDb(void)
{
}
StrategyDb :: ~StrategyDb(void)
{
}
StrategyData* StrategyDb :: getStrategyById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select StrategyId, Name, Since, UsedInTrading, ParentStrategyId from Strategy "
                  "where StrategyId = StrToUUid(:StrategyId) ");
	query.bindValue(":StrategyId", QVariant(id));
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	StrategyData *item = new StrategyData();
	item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
	item->name = query.value(Name).toString();
	item->since = query.value(Since).toDateTime();
    item->usedInTrading = query.value(UsedInTrading).toBool();
	item->parentStrategyId = QUuid::fromRfc4122(query.value(ParentStrategyId).toByteArray());
	qDebug() << item->strategyId << endl;
	query.finish();
	db.close();
	return item;
}

QList<StrategyData*> StrategyDb :: getStrategies() {
    QList<StrategyData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("select StrategyId, Name, Since, UsedInTrading, ParentStrategyId from Strategy ");

    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    while (query.next()) {
        StrategyData *item = new StrategyData();
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->name = query.value(Name).toString();
        item->since = query.value(Since).toDateTime();
        item->usedInTrading = query.value(UsedInTrading).toBool();
        item->parentStrategyId = QUuid::fromRfc4122(query.value(ParentStrategyId).toByteArray());
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

unsigned int StrategyDb :: insertStrategy(const StrategyData* data) {
    return insertStrategy(data->strategyId, data->name, data->since, data->usedInTrading, data->parentStrategyId);
 }

unsigned int StrategyDb :: insertStrategy(QUuid strategyId, QString name, QDateTime since, bool usedInTrading,
                                          QUuid parentStrategyId) {
 	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
	QSqlQuery query;
    query.prepare("insert into Strategy(StrategyId, Name, Since, UsedInTrading, ParentStrategyId) "
                  "Values(StrToUUid(:StrategyId), :Name, :Since, :UsedInTrading, StrToUUid(:ParentStrategyId) )"
	);

    query.bindValue(":StrategyId", QVariant(strategyId));
	query.bindValue(":Name", name);
	query.bindValue(":Since", since);
	query.bindValue(":UsedInTrading", usedInTrading);
	query.bindValue(":ParentStrategyId", QVariant(parentStrategyId));
	//execute
	bool result = query.exec();
	if (!result) {
        qDebug() << query.executedQuery() << endl;
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
	}
	qDebug() << "Inserted a row" << endl;
    return query.size();
}

unsigned int StrategyDb :: updateStrategy(const StrategyData* data, QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Update Strategy Set Name = :Name, Since = :Since, UsedInTrading = :UsedInTrading, "
                  "ParentStrategyId = StrToUUid(:ParentStrategyId) Where StrategyId = StrToUUid(:StrategyId) ");
	query.bindValue(":Name", data->name);
	query.bindValue(":Since", data->since);
	query.bindValue(":UsedInTrading", data->usedInTrading);
    query.bindValue(":StrategyId", QVariant(id));
    query.bindValue(":ParentStrategyId", QVariant(data->parentStrategyId));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not update table for id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

unsigned int StrategyDb :: deleteStrategy(QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Delete from Strategy where StrategyId = StrToUUid(:StrategyId) ");
    query.bindValue(":StrategyId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

