#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "strategylinkedpositiondb.h"

StrategyLinkedPositionDb :: StrategyLinkedPositionDb(void)
{
}
StrategyLinkedPositionDb :: ~StrategyLinkedPositionDb(void)
{
}
StrategyLinkedPositionData* StrategyLinkedPositionDb :: getStrategyLinkedPositionById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, TotalAmountBought,"
                  " TotalAmountSold, TotalAmountCommission, RealizedPnl, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition where StrategyLinkedPositionId = StrToUuid(:StrategyLinkedPositionId) ");

    query.bindValue(":StrategyLinkedPositionId", QVariant(id));
    bool result = query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
		query.finish();
		db.close();
		return NULL;
	}
    query.next();
	StrategyLinkedPositionData *item = new StrategyLinkedPositionData();
    item->strategyLinkedPositionId = QUuid::fromRfc4122(query.value(StrategyLinkedPositionId).toByteArray());
	item->numberBought = query.value(NumberBought).toUInt();
	item->numberSold = query.value(NumberSold).toUInt();
	item->avgAmountBought = query.value(AvgAmountBought).toFloat();
	item->avgAmountSold = query.value(AvgAmountSold).toFloat();
    item->totalAmountBought = query.value(TotalAmountBought).toFloat();
    item->totalAmountSold = query.value(TotalAmountSold).toFloat();
    item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
    item->realizedPnl = query.value(RealizedPnl).toFloat();
	item->updatedDate = query.value(UpdatedDate).toDateTime();
	item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
	item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
    qDebug() << item->strategyLinkedPositionId << endl;
	query.finish();
	db.close();
	return item;
}

QList<StrategyLinkedPositionData*> StrategyLinkedPositionDb :: getStrategyLinkedPositions() {
    QList<StrategyLinkedPositionData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, TotalAmountBought,"
                  " TotalAmountSold, TotalAmountCommission, RealizedPnl, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition ");
    query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyLinkedPositionData *item = new StrategyLinkedPositionData();
        item->strategyLinkedPositionId = QUuid::fromRfc4122(query.value(StrategyLinkedPositionId).toByteArray());
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountBought = query.value(TotalAmountBought).toFloat();
        item->totalAmountSold = query.value(TotalAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->realizedPnl = query.value(RealizedPnl).toFloat();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

unsigned int StrategyLinkedPositionDb :: insertStrategyLinkedPosition(const StrategyLinkedPositionData* data) {
    return insertStrategyLinkedPosition(data->strategyLinkedPositionId, data->numberBought, data->numberSold,
                                        data->avgAmountBought,
                                        data->avgAmountSold, data->totalAmountBought, data->totalAmountSold,
                                        data->totalAmountCommission, data->realizedPnl, data->updatedDate,
                                        data->strategyId, data->instrumentId);
 }

unsigned int StrategyLinkedPositionDb :: insertStrategyLinkedPosition(QUuid id, unsigned int numberBought,
                        unsigned int numberSold,
                        float avgAmountBought, float avgAmountSold, float totalAmountBought, float totalAmountSold,
                        float totalAmountCommission, float realizedPnl, QDateTime updatedDate, QUuid strategyId,
                        QUuid instrumentId) {
 	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
	QSqlQuery query;
    query.prepare("insert into StrategyLinkedPosition(StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, "
                  "AvgAmountSold, TotalAmountBought, TotalAmountSold, TotalAmountCommission, RealizedPnl, UpdatedDate, "
                  "StrategyId, InstrumentId) Values(StrToUuid(:StrategyLinkedPositionId), :NumberBought, :NumberSold, "
                  ":AvgAmountBought, :AvgAmountSold, :TotalAmountBought, :TotalAmountSold, :TotalAmountCommission, "
                  ":RealizedPnl, :UpdatedDate, StrToUuid(:StrategyId), StrToUuid(:InstrumentId) )"
	);

    query.bindValue(":StrategyLinkedPositionId", QVariant(id));
	query.bindValue(":NumberBought", numberBought);
	query.bindValue(":NumberSold", numberSold);
	query.bindValue(":AvgAmountBought", avgAmountBought);
	query.bindValue(":AvgAmountSold", avgAmountSold);
	query.bindValue(":TotalAmountBought", totalAmountBought);
	query.bindValue(":TotalAmountSold", totalAmountSold);
	query.bindValue(":TotalAmountCommission", totalAmountCommission);
	query.bindValue(":RealizedPnl", realizedPnl);
	query.bindValue(":UpdatedDate", updatedDate);
	query.bindValue(":StrategyId", QVariant(strategyId));
	query.bindValue(":InstrumentId", QVariant(instrumentId));
	//execute
	bool result = query.exec();
	if (!result) {
        qDebug() << query.executedQuery() << endl;
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
	return 0;
	}
	qDebug() << "Inserted a row" << endl;
	return 1;
}

unsigned int StrategyLinkedPositionDb :: updateStrategyLinkedPosition(const StrategyLinkedPositionData* data, QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Update StrategyLinkedPosition Set NumberBought = :NumberBought, NumberSold = :NumberSold, "
                  "AvgAmountBought = :AvgAmountBought, AvgAmountSold = :AvgAmountSold, TotalAmountBought = :TotalAmountBought, "
                  "TotalAmountSold = :TotalAmountSold, TotalAmountCommission = :TotalAmountCommission, "
                  "RealizedPnl = :RealizedPnl, UpdatedDate = :UpdatedDate, StrategyId = StrToUuid(:StrategyId), "
                  "InstrumentId = StrToUuid(:InstrumentId) Where StrategyLinkedPositionId = StrToUuid(:StrategyLinkedPositionId) ");

    query.bindValue(":StrategyLinkedPositionId", QVariant(id));
    query.bindValue(":NumberBought", data->numberBought);
    query.bindValue(":NumberSold", data->numberSold);
    query.bindValue(":AvgAmountBought", data->avgAmountBought);
    query.bindValue(":AvgAmountSold", data->avgAmountSold);
    query.bindValue(":TotalAmountBought", data->totalAmountBought);
    query.bindValue(":TotalAmountSold", data->totalAmountSold);
    query.bindValue(":TotalAmountCommission", data->totalAmountCommission);
    query.bindValue(":RealizedPnl", data->realizedPnl);
    query.bindValue(":UpdatedDate", data->updatedDate);
    query.bindValue(":StrategyId", QVariant(data->strategyId));
    query.bindValue(":InstrumentId", QVariant(data->instrumentId));

	bool result = query.exec();
	if (!result)
		qDebug() << "Could not update table for id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

unsigned int StrategyLinkedPositionDb :: deleteStrategyLinkedPosition(QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Delete from StrategyLinkedPosition where StrategyLinkedPositionId = StrToUUid(:StrategyLinkedPositionId) ");
    query.bindValue(":StrategyLinkedPositionId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

