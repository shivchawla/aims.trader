#include <QtSql/QSqlError>
#include "StrategyLinkedPositionDb2.h"

StrategyLinkedPositionDb2::StrategyLinkedPositionDb2(void)
{}

StrategyLinkedPositionDb2::~StrategyLinkedPositionDb2(void)
{}

StrategyLinkedPositionData2* StrategyLinkedPositionDb2 :: getStrategyLinkedPositionById(const uint &id) {
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
    StrategyLinkedPositionData2 *item = new StrategyLinkedPositionData2();
    item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
	item->numberBought = query.value(NumberBought).toUInt();
	item->numberSold = query.value(NumberSold).toUInt();
	item->avgAmountBought = query.value(AvgAmountBought).toFloat();
	item->avgAmountSold = query.value(AvgAmountSold).toFloat();
    item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
    item->createdDate = query.value(CreatedDate).toDateTime();
	item->updatedDate = query.value(UpdatedDate).toDateTime();
    //item->strategyId = query.value(StrategyId).toUInt();
    //item->instrumentId = query.value(InstrumentId).toUInt();
    item->printDebug();
	query.finish();
	db.close();
	return item;
}

QList<StrategyLinkedPositionData2*> StrategyLinkedPositionDb2 :: getStrategyLinkedPositions() {
    QList<StrategyLinkedPositionData2*> list;
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
        StrategyLinkedPositionData2* item = new StrategyLinkedPositionData2();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        //item->strategyId = query.value(StrategyId).toUInt();
        //item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<StrategyLinkedPositionData2*> StrategyLinkedPositionDb2 :: getOpenStrategyLinkedPositions(uint strategyId) {
    QList<StrategyLinkedPositionData2*> list;
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
        StrategyLinkedPositionData2* item = new StrategyLinkedPositionData2();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        //item->strategyId = query.value(StrategyId).toUInt();
        //item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<StrategyLinkedPositionData2*> StrategyLinkedPositionDb2 :: getPositionsForStrategy(const uint &strategyId) {
    QList<StrategyLinkedPositionData2*> list;
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
        StrategyLinkedPositionData2 *item = new StrategyLinkedPositionData2();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        item->numberBought = query.value(NumberBought).toUInt();
        item->numberSold = query.value(NumberSold).toUInt();
        item->avgAmountBought = query.value(AvgAmountBought).toFloat();
        item->avgAmountSold = query.value(AvgAmountSold).toFloat();
        item->totalAmountCommission = query.value(TotalAmountCommission).toFloat();
        item->createdDate = query.value(CreatedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        //item->strategyId = query.value(StrategyId).toUInt();
        //item->instrumentId = query.value(InstrumentId).toUInt();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

//Returns the last inserted primary key
uint StrategyLinkedPositionDb2 :: insertStrategyLinkedPosition(const uint runId, const uint positionId, const uint numberBought, const uint numberSold, const double avgBuyPrice, const double avgSellPrice,
                                                const double totalAmountCommission, const QDateTime& createdDate) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyLinkedPosition(RunId, PositionId, NumberBought, NumberSold, AvgAmountBought, "
                  "AvgAmountSold, CreatedDate) Values(:NumberBought, :NumberSold, "
                  ":AvgBuyPrice, :AvgSellPrice,  :TotalAmountCommission, "
                  ":CreatedDate)"
	);

	query.bindValue(":NumberBought", numberBought);
	query.bindValue(":NumberSold", numberSold);
    query.bindValue(":AvgBuyPricet", avgBuyPrice);
    query.bindValue(":AvgSellPrice", avgSellPrice);
	query.bindValue(":TotalAmountCommission", totalAmountCommission);
    query.bindValue(":CreatedDate", createdDate);
    query.bindValue(":PositionId", positionId);
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

uint StrategyLinkedPositionDb2 :: updateStrategyLinkedPosition(const uint runId, const uint positionId, const uint numberBought, const uint numberSold, const double avgAmountBought, const double avgAmountSold,
                                  const double totalAmountCommission, const QDateTime& updatedDate) {

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
                  " UpdatedDate = :UpdatedDate,"
                  "PositionId = :PositionId Where RunId = :runId ");

    query.bindValue(":RunId", runId);
    query.bindValue(":NumberBought", numberBought);
    query.bindValue(":NumberSold", numberSold);
    query.bindValue(":AvgAmountBought", avgAmountBought);
    query.bindValue(":AvgAmountSold", avgAmountSold);
    query.bindValue(":TotalAmountCommission", totalAmountCommission);
    //query.bindValue(":CreatedDate", createdDate);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":PositionId", positionId);
    //query.bindValue(":InstrumentId", instrumentId);

    bool result = query.exec();
    if (!result) {
        //qDebug() << "Could not update table for RunID ="<<runId<<"StrategyId ="<<strategyId<<"InstrumentId = "<<instrumentId<<endl;
        qDebug() << query.lastQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }
    query.finish();
    db.close();
    return query.size();
}

uint StrategyLinkedPositionDb2 :: deleteStrategyLinkedPosition(const uint &id) {
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

