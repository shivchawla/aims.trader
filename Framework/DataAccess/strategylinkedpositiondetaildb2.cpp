#include <QtSql/QSqlError>
#include "StrategyLinkedPositionDetailDb2.h"

StrategyLinkedPositionDetailDb2 :: StrategyLinkedPositionDetailDb2(void)
{}

StrategyLinkedPositionDetailDb2 :: ~StrategyLinkedPositionDetailDb2(void)
{}

StrategyLinkedPositionDetailData2* StrategyLinkedPositionDetailDb2 :: getStrategyLinkedPositionDetailById(const uint &id) {
	qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyLinkedPositionDetailId, SharesBought, SharesSold, AvgBought, AvgSold, Commission, CreatedDateTime, "
                  " StrategyLinkedPositionId from StrategyLinkedPositionDetail "
                  " where StrategyLinkedPositionDetailId = :StrategyLinkedPositionDetailId ");
    query.bindValue(":StrategyLinkedPositionDetailId", id);
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
    StrategyLinkedPositionDetailData2 *item = new StrategyLinkedPositionDetailData2();
	item->strategyLinkedPositionDetailId = query.value(StrategyLinkedPositionDetailId).toUInt();
	item->sharesBought = query.value(SharesBought).toUInt();
	item->sharesSold = query.value(SharesSold).toUInt();
	item->avgBought = query.value(AvgBought).toFloat();
	item->avgSold = query.value(AvgSold).toFloat();
	item->commission = query.value(Commission).toFloat();
	item->createdDateTime = query.value(CreatedDateTime).toDateTime();
	item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
	qDebug() << item->strategyLinkedPositionDetailId << endl;
	query.finish();
	db.close();
	return item;
}

uint StrategyLinkedPositionDetailDb2 ::updateStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId, const uint sharesBought,
                                        const uint sharesSold, const double avgBought,
                                        const double avgSold, const double commission, const QDateTime& updatedDateTime)
{
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("update StrategyLinkedPositionDetail set SharesBought = :SharesBought , SharesSold = :SharesSold, AvgBought = :AvgBought,"
                  "AvgSold = :AvgSold, Commission = :Commission, UpdatedDateTime = :UpdatedDateTime"
                  "where runid = :RunId and positionId = :PositionId and PositionDetailId = :PositionDetailId");

    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBought", avgBought);
    query.bindValue(":AvgSold", avgSold);
    query.bindValue(":Commission", commission);
    query.bindValue(":UpdatedDateTime", updatedDateTime);
    query.bindValue(":RunId", runId);
    query.bindValue(":PositionId", positionId);
    //query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":PositionDetailId", detailId);

    //execute
    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
    }
    //qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}


uint StrategyLinkedPositionDetailDb2 :: insertStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId,
                                                                          const uint sharesBought, const uint sharesSold, const double avgBought,
                                                                          const double avgSold, const double commission, const QDateTime& createdDateTime) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyLinkedPositionDetail(RunId, PositionId, PositionDetailId, SharesBought, SharesSold, AvgBought, AvgSold, "
                  " Commission, CreatedDateTime, StrategyLinkedPositionId) Values(:RunId, :PositionId, :PositionDetailId, :SharesBought, "
                  " :SharesSold, :AvgBought, :AvgSold, :Commission, :CreatedDateTime)"
                  );

    //query.bindValue(":StrategyLinkedPositionDetailId", QVariant(QUuid :: createUuid()));
	query.bindValue(":SharesBought", sharesBought);
	query.bindValue(":SharesSold", sharesSold);
	query.bindValue(":AvgBought", avgBought);
	query.bindValue(":AvgSold", avgSold);
	query.bindValue(":Commission", commission);
	query.bindValue(":CreatedDateTime", createdDateTime);
    query.bindValue(":RunId", runId);
    query.bindValue(":PositionId", positionId);
    query.bindValue(":PositionDetailId", detailId);


    //execute
	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
	}
    //qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}

uint StrategyLinkedPositionDetailDb2 :: insertStrategyLinkedPositionDetails(const QList<StrategyLinkedPositionDetailData2*> &list) {
	//check database if available to work with
	if (!openDatabase()) {
		return 0; //to signify zero inserted rows
	}

	//prepare statement
	QSqlQuery query = getBlankQuery();
    db.transaction();
    query.prepare("insert into StrategyLinkedPositionDetail(SharesBought, SharesSold, AvgBought, AvgSold, "
                  " Commission, CreatedDateTime, StrategyLinkedPositionId) "
                  " Values(:SharesBought, :SharesSold, :AvgBought, :AvgSold, :Commission, :CreatedDateTime, :StrategyLinkedPositionId )"
                );
    uint ctr=0;
    foreach(StrategyLinkedPositionDetailData2* item, list) {
        query.bindValue(":SharesBought", item->sharesBought);
        query.bindValue(":SharesSold", item->sharesSold);
        query.bindValue(":AvgBought", item->avgBought);
        query.bindValue(":AvgSold", item->avgSold);
        query.bindValue(":Commission", item->commission);
        query.bindValue(":CreatedDateTime", item->createdDateTime);
        query.bindValue(":StrategyLinkedPositionId", item->strategyLinkedPositionId);

        bool res = query.exec();
//        if (!res) {
//            qDebug() << query.executedQuery() << endl;
//            qDebug() << "Error in bulk insert of Strategy Linked Position Detail data" << endl;
//            qDebug() << query.lastError().text() << endl;
//        }
        ctr += (res ? 1 : 0);
    }

    db.commit();

    if (ctr < list.count()) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Error in bulk insert of Strategy Linked Position Detail data" << endl;
        qDebug() << query.lastError().text() << endl;
    }

    db.close();

    return ctr;
}

QList<StrategyLinkedPositionDetailData2*> StrategyLinkedPositionDetailDb2 :: getStrategyLinkedPositionDetails(const uint &strategyLinkedPositionId) {
    QList<StrategyLinkedPositionDetailData2*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyLinkedPositionDetailId, SharesBought, SharesSold, AvgBought, AvgSold, Commission, CreatedDateTime, "
                  " StrategyLinkedPositionId from StrategyLinkedPositionDetail "
                  " where StrategyLinkedPositionId = :StrategyLinkedPositionId ");
    query.bindValue(":StrategyLinkedPositionId", strategyLinkedPositionId);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    while (query.next()) {
        StrategyLinkedPositionDetailData2 *item = new StrategyLinkedPositionDetailData2();
        item->strategyLinkedPositionDetailId = query.value(StrategyLinkedPositionDetailId).toUInt();
        item->sharesBought = query.value(SharesBought).toUInt();
        item->sharesSold = query.value(SharesSold).toUInt();
        item->avgBought = query.value(AvgBought).toFloat();
        item->avgSold = query.value(AvgSold).toFloat();
        item->commission = query.value(Commission).toFloat();
        item->createdDateTime = query.value(CreatedDateTime).toDateTime();
        item->strategyLinkedPositionId = query.value(StrategyLinkedPositionId).toUInt();
        qDebug() << item->strategyLinkedPositionDetailId << endl;
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}
