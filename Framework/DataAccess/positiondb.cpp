#include <QtSql/QSqlError>
#include "PositionDb.h"

PositionDb::PositionDb(void)
{}

PositionDb::~PositionDb(void)
{}

PositionData PositionDb :: getStrategyLinkedPositionById(const uint &id) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select PositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate "
                  "from Position2 where PositionId = :PositionId ");

    query.bindValue(":PositionId", id);
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
    PositionData item;// = new PositionData();
    item.positionId = query.value(0).toUInt();
    item.sharesBought = query.value(1).toUInt();
    item.sharesSold = query.value(2).toUInt();
    item.avgBuyPrice = query.value(3).toDouble();
    item.avgSellPrice = query.value(4).toDouble();
    item.totalAmountCommission = query.value(5).toDouble();
    item.createdDate = query.value(6).toDateTime();
    item.updatedDate = query.value(7).toDateTime();
    item.printDebug();
	query.finish();
	db.close();
	return item;
}

QList<PositionData> PositionDb :: getStrategyLinkedPositions() {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select PositionId, sharesBought, sharesSold, avgBuyPrice, avgSellPrice, "
                  " TotalAmountCommission, CreatedDate, UpdatedDate "
                  "from Position2 ");
    //query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        PositionData item;// = new PositionData();
        item.positionId = query.value(0).toUInt();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.totalAmountCommission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<PositionData> PositionDb :: getOpenStrategyLinkedPositions(uint strategyId) {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select PositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate "
                  "from Position2 "
                  " where sharesBought != SharesSold and StrategyId = :StrategyId ");
    query.bindValue(":StrategyId", strategyId);

    bool result = query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    //qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        PositionData item;// = new PositionData();
        item.positionId = query.value(0).toUInt();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.totalAmountCommission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<PositionData> PositionDb :: getPositionsForStrategy(const uint &strategyId) {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select PositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate"
                  "from Position2 where StrategyId = :StrategyId ");

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
        PositionData item;// = new PositionData();
        item.positionId = query.value(0).toUInt();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.totalAmountCommission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

//Returns the last inserted primary key
uint PositionDb :: insertStrategyLinkedPosition(const uint runId, const uint positionId, const uint sharesBought, const uint sharesSold, const double avgBuyPrice, const double avgSellPrice,
                                                const double totalAmountCommission, const QDateTime& createdDate) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Position2 (RunId, PositionId, SharesBought, SharesSold, AvgBuyPrice, "
                  "AvgSellPrice, Commission, CreatedDate) Values(:RunId, :PositionId, :SharesBought, :SharesSold, "
                  ":AvgBuyPrice, :AvgSellPrice,  :TotalAmountCommission, "
                  ":CreatedDate)"
	);

    query.bindValue(":RunId", runId);
    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBuyPrice", avgBuyPrice);
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

uint PositionDb :: updateStrategyLinkedPosition(const uint runId, const uint positionId, const uint sharesBought, const uint sharesSold, const double avgBuyPrice, const double avgSellPrice,
                                  const double totalAmountCommission, const QDateTime& updatedDate) {

    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("update Position2 set sharesBought = :SharesBought, SharesSold = :SharesSold, "
                  "AvgBuyPrice = :AvgBuyPrice, AvgSellPrice = :AvgSellPrice, "
                  " Commission = :TotalAmountCommission, "
                  " UpdatedDate = :UpdatedDate "
                  " where PositionId = :PositionId and RunId = :RunId ");

    query.bindValue(":RunId", runId);
    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBuyPrice", avgBuyPrice);
    query.bindValue(":AvgSellPrice", avgSellPrice);
    query.bindValue(":TotalAmountCommission", totalAmountCommission);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":PositionId", positionId);

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

uint PositionDb :: deleteStrategyLinkedPosition(const uint &id) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from Position2 where PositionId = :PositionId");
    query.bindValue(":PositionId", id);
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

