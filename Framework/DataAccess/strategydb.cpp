#include <QtSql/QSqlError>
#include "strategydb.h"

StrategyDb :: StrategyDb(void)
{
}
StrategyDb :: ~StrategyDb(void)
{
}
StrategyData* StrategyDb :: getStrategyById(const uint &id) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyId, Name, Since, UsedInTrading, ParentStrategyId from Strategy "
                  "where StrategyId = :StrategyId ");
    query.bindValue(":StrategyId", id);
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	StrategyData *item = new StrategyData();
    item->strategyId = query.value(StrategyId).toUInt();
	item->name = query.value(Name).toString();
    item->since = query.value(Since).toDateTime();
    item->usedInTrading = query.value(UsedInTrading).toBool();
    item->parentStrategyId = query.value(ParentStrategyId).toUInt();
    item->parentStrategyName = query.value(ParentStrategyName).toString();
    item->printDebug();
	query.finish();
	db.close();
	return item;
}

StrategyData* StrategyDb :: getStrategyViewByName(const QString &strategyName) {
     //qDebug() << "Received " << strategyName << endl;
     if (!openDatabase()) {
         qDebug() << "Unable to connect to database!!" << endl;
         qDebug() << db.lastError().driverText();
         return NULL;
     }

     QSqlQuery query = getBlankQuery();
     query.prepare("select StrategyId, Name, Since, UsedInTrading, ParentStrategyId from Strategy "
                   "where Name = :Name ");
     query.bindValue(":Name", strategyName);
     query.exec();
     qDebug() << "Got " << query.size() << " rows" << endl;
     if (!query.next()) {
         query.finish();
         db.close();
         return NULL;
     }
     StrategyData *item = new StrategyData();
     item->strategyId = query.value(StrategyId).toUInt();
     item->name = query.value(Name).toString();
     item->since = query.value(Since).toDateTime();
     item->usedInTrading = query.value(UsedInTrading).toBool();
     item->parentStrategyId = query.value(ParentStrategyId).toUInt();
     item->parentStrategyName = query.value(ParentStrategyName).toString();
     item->printDebug();
     query.finish();
     db.close();
     return item;
}

QList<StrategyData*> StrategyDb :: getStrategies() {
    QList<StrategyData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select c.StrategyId, c.Name, c.Since, c.UsedInTrading, c.ParentStrategyId, p.Name ParentStrategyName"
                             " from Strategy c left join Strategy p on c.ParentStrategyId = p.StrategyId ");

    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    while (query.next()) {
        StrategyData *item = new StrategyData();
        item->strategyId = query.value(StrategyId).toUInt();
        item->name = query.value(Name).toString();
        item->since = query.value(Since).toDateTime();
        item->usedInTrading = query.value(UsedInTrading).toBool();
        item->parentStrategyId = query.value(ParentStrategyId).toUInt();
        item->parentStrategyName = query.value(ParentStrategyName).toString();
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

uint StrategyDb :: insertStrategy(const StrategyData* data) {
    return insertStrategy(data->name, data->since, data->usedInTrading, data->parentStrategyId);
 }

uint StrategyDb :: insertStrategy(QString name, QDateTime since, bool usedInTrading, uint parentStrategyId) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Strategy(Name, Since, UsedInTrading, ParentStrategyId) "
                  "Values(:Name, :Since, :UsedInTrading, :ParentStrategyId ) "
	);

	query.bindValue(":Name", name);
	query.bindValue(":Since", since);
	query.bindValue(":UsedInTrading", usedInTrading);
    query.bindValue(":ParentStrategyId", parentStrategyId);
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

uint StrategyDb :: updateStrategy(const StrategyData* data) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Update Strategy Set Name = :Name, Since = :Since, UsedInTrading = :UsedInTrading, "
                  "ParentStrategyId = :ParentStrategyId Where StrategyId = :StrategyId ");
	query.bindValue(":Name", data->name);
	query.bindValue(":Since", data->since);
	query.bindValue(":UsedInTrading", data->usedInTrading);
    query.bindValue(":StrategyId", data->strategyId);
    query.bindValue(":ParentStrategyId", data->parentStrategyId);
	bool result = query.exec();
	if (!result)
        qDebug() << "Could not update table for strategyId " << data->strategyId << endl;
	query.finish();
	db.close();
	return query.size();
}

uint StrategyDb :: deleteStrategy(const uint &id) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from Strategy where StrategyId = :StrategyId ");
    query.bindValue(":StrategyId", id);
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

uint StrategyDb::insertRow(const StrategyData* data)
{
    insertStrategy(data);
}

uint StrategyDb::deleteRow(const StrategyData* data)
{
    deleteStrategy(data->strategyId);
}

uint StrategyDb::updateRow(const StrategyData* data)
{
    updateStrategy(data);
}


