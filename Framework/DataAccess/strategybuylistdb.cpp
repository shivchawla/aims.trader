#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "strategybuylistdb.h"

StrategyBuyListDb :: StrategyBuyListDb(void)
{
}
StrategyBuyListDb :: ~StrategyBuyListDb(void)
{
}
StrategyBuyListData* StrategyBuyListDb :: getStrategyBuyListById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select StrategyBuyListId, StrategyId, InstrumentId from StrategyBuyList "
                  "where StrategyBuyListId = StrToUuid(:StrategyBuyListId) ");
	query.bindValue(":StrategyBuyListId", QVariant(id));
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	StrategyBuyListData *item = new StrategyBuyListData();
	item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
	item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
	item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
	qDebug() << item->strategyBuyListId << endl;
	query.finish();
	db.close();
	return item;
}

QList<StrategyBuyListData*> StrategyBuyListDb :: getStrategyBuyLists() {
    QList<StrategyBuyListData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("select StrategyBuyListId, StrategyId, InstrumentId from StrategyBuyList ");
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyBuyListData *item = new StrategyBuyListData();
        item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

QList<StrategyBuyListData*> StrategyBuyListDb :: getStrategyBuyListsForStrategy(const QString& strategyName) {
    QList<StrategyBuyListData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    query.prepare("select sbl.StrategyId, sbl.StrategyId, sbl.InstrumentId "
                 "from StrategyBuyList sbl "
                 "inner join Strategy s on sbl.StrategyId = s.StrategyId "
                 "inner join Instrument i on sbl.InstrumentId = i.InstrumentId "
                 "where s.Name=:Name ");
    query.bindValue(":Name", strategyName);
    bool result = query.exec();
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyBuyListData *item = new StrategyBuyListData();
        item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

unsigned int StrategyBuyListDb :: insertStrategyBuyList(const StrategyBuyListData* data) {
    return insertStrategyBuyList(data->strategyBuyListId, data->strategyId, data->instrumentId);
 }

unsigned int StrategyBuyListDb :: insertStrategyBuyList(QUuid id, QUuid strategyId, QUuid instrumentId) {
 	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
	QSqlQuery query;
    query.prepare("insert into StrategyBuyList(StrategyBuyListId, StrategyId, InstrumentId) "
                  "Values(StrToUuid(:StrategyBuyListId), StrToUuid(:StrategyId), StrToUuid(:InstrumentId)) "
	);

    query.bindValue(":StrategyBuyListId", QVariant(id));
	query.bindValue(":StrategyId", QVariant(strategyId));
	query.bindValue(":InstrumentId", QVariant(instrumentId));
	//execute
	bool result = query.exec();
	if (!result) {
        qDebug() << query.executedQuery() << endl;
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
	}
	qDebug() << "Inserted a row" << endl;
    return 1;
}

unsigned int StrategyBuyListDb :: updateStrategyBuyList(const StrategyBuyListData* data, QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Update StrategyBuyList Set StrategyId = StrToUuid(:StrategyId), InstrumentId = StrToUuid(:InstrumentId) "
                  "Where StrategyBuyListId = StrToUuid(:StrategyBuyListId) ");
    query.bindValue(":StrategyId", QVariant(data->strategyId));
    query.bindValue(":StrategyBuyListId", QVariant(id));
    query.bindValue(":InstrumentId", QVariant(data->instrumentId));

	bool result = query.exec();
	if (!result)
		qDebug() << "Could not update table for id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

unsigned int StrategyBuyListDb :: deleteStrategyBuyList(QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Delete from StrategyBuyList where StrategyBuyListId = StrToUuid(:StrategyBuyListId)");
    query.bindValue(":StrategyBuyListId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

