#include <QtSql/QSqlError>
#include "strategylinkedpositiondb.h"

StrategyLinkedPositionDb :: StrategyLinkedPositionDb(void)
{
}
StrategyLinkedPositionDb :: ~StrategyLinkedPositionDb(void)
{
}
StrategyLinkedPositionData* StrategyLinkedPositionDb :: getStrategyLinkedPositionById(const uint &id) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition where StrategyLinkedPositionId = :StrategyLinkedPositionId ");

    query.bindValue(":StrategyLinkedPositionId", id);
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
    item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
	item->numberBought = query.value(NumberBought).toUInt();
	item->numberSold = query.value(NumberSold).toUInt();
	item->avgAmountBought = query.value(AvgAmountBought).toFloat();
	item->avgAmountSold = query.value(AvgAmountSold).toFloat();
    item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
    item->createdDate = query.value(CreatedDate).toDateTime();
	item->updatedDate = query.value(UpdatedDate).toDateTime();
    item->strategyId = query.value(StrategyId).toUInt();
    item->instrumentId = query.value(InstrumentId).toUInt();
    item->printDebug();
	query.finish();
	db.close();
	return item;
}

QList<StrategyLinkedPositionData*> StrategyLinkedPositionDb :: getStrategyLinkedPositions() {
    QList<StrategyLinkedPositionData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition ");
    //query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyLinkedPositionData *item = new StrategyLinkedPositionData();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->strategyId = query.value(StrategyId).toUInt();
        item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<StrategyLinkedPositionData*> StrategyLinkedPositionDb :: getOpenStrategyLinkedPositions(uint strategyId) {
    QList<StrategyLinkedPositionData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition "
                  " where NumberBought != NumberSold and StrategyId = :StrategyId ");
    query.bindValue(":StrategyId", strategyId);

    bool result = query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    //qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyLinkedPositionData *item = new StrategyLinkedPositionData();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->strategyId = query.value(StrategyId).toUInt();
        item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<StrategyLinkedPositionData*> StrategyLinkedPositionDb :: getPositionsForStrategy(const uint &strategyId) {
    QList<StrategyLinkedPositionData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyLinkedPositionId, NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate, StrategyId, InstrumentId "
                  "from StrategyLinkedPosition where StrategyId = :StrategyId ");

    query.bindValue(":StrategyId", strategyId);
    bool result = query.exec();

    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;

    while(query.next()) {
        StrategyLinkedPositionData *item = new StrategyLinkedPositionData();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->strategyId = query.value(StrategyId).toUInt();
        item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

uint StrategyLinkedPositionDb :: insertStrategyLinkedPosition(const StrategyLinkedPositionData* &data) {
    return insertStrategyLinkedPosition(data->numberBought, data->numberSold,
                                        data->avgAmountBought, data->avgAmountSold,
                                        data->totalAmountCommission, data->createdDate, data->updatedDate,
                                        data->strategyId, data->instrumentId);
 }

//Returns the last inserted primary key
uint StrategyLinkedPositionDb :: insertStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                                float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate, uint strategyId,
                                                uint instrumentId) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyLinkedPosition(NumberBought, NumberSold, AvgAmountBought, "
                  "AvgAmountSold,  TotalAmountCommission, CreatedDate, UpdatedDate, "
                  "StrategyId, InstrumentId) Values(:NumberBought, :NumberSold, "
                  ":AvgAmountBought, :AvgAmountSold,  :TotalAmountCommission, "
                  ":CreatedDate, :UpdatedDate, :StrategyId, :InstrumentId )"
	);

	query.bindValue(":NumberBought", numberBought);
	query.bindValue(":NumberSold", numberSold);
	query.bindValue(":AvgAmountBought", avgAmountBought);
	query.bindValue(":AvgAmountSold", avgAmountSold);
	query.bindValue(":TotalAmountCommission", totalAmountCommission);
    query.bindValue(":CreatedDate", createdDate);
	query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":InstrumentId", instrumentId);
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

uint StrategyLinkedPositionDb :: updateStrategyLinkedPosition(const StrategyLinkedPositionData* &data) {
    return updateStrategyLinkedPosition(data->strategyLinkedPositionId ,data->numberBought, data->numberSold, data->avgAmountBought, data->avgAmountSold,
                                        data->totalAmountCommission, data->updatedDate,
                                        data->strategyId, data->instrumentId);
}

uint StrategyLinkedPositionDb :: updateStrategyLinkedPosition(uint strategyLinkedPositionId, uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                  float totalAmountCommission, QDateTime updatedDate, uint strategyId,
                                  uint instrumentId) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update StrategyLinkedPosition Set NumberBought = :NumberBought, NumberSold = :NumberSold, "
                  "AvgAmountBought = :AvgAmountBought, AvgAmountSold = :AvgAmountSold, "
                  "TotalAmountCommission = :TotalAmountCommission, "
                  " UpdatedDate = :UpdatedDate, StrategyId = :StrategyId, "
                  "InstrumentId = :InstrumentId Where StrategyLinkedPositionId = :StrategyLinkedPositionId ");

    query.bindValue(":StrategyLinkedPositionId", strategyLinkedPositionId);
    query.bindValue(":NumberBought", numberBought);
    query.bindValue(":NumberSold", numberSold);
    query.bindValue(":AvgAmountBought", avgAmountBought);
    query.bindValue(":AvgAmountSold", avgAmountSold);
    query.bindValue(":TotalAmountCommission", totalAmountCommission);
    //query.bindValue(":CreatedDate", createdDate);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":InstrumentId", instrumentId);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Could not update table for strategyLinkedPositionId " << strategyLinkedPositionId << endl;
        qDebug() << query.lastQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }
    query.finish();
    db.close();
    return query.size();
}

uint StrategyLinkedPositionDb :: deleteStrategyLinkedPosition(const uint &id) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from StrategyLinkedPosition where StrategyLinkedPositionId = :StrategyLinkedPositionId");
    query.bindValue(":StrategyLinkedPositionId", id);
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

