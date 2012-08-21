#include <QtSql/QSqlError>
#include "strategylinkedpositiondetaildb.h"

StrategyLinkedPositionDetailDb :: StrategyLinkedPositionDetailDb(void)
{
}
StrategyLinkedPositionDetailDb :: ~StrategyLinkedPositionDetailDb(void)
{
}
StrategyLinkedPositionDetailData* StrategyLinkedPositionDetailDb :: getStrategyLinkedPositionDetailById(const uint &id) {
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
	StrategyLinkedPositionDetailData *item = new StrategyLinkedPositionDetailData();
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

// returns the newly inserted ID
uint StrategyLinkedPositionDetailDb :: insertStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData* data) {
    return insertStrategyLinkedPositionDetail(data->sharesBought, data->sharesSold, data->avgBought, data->avgSold, data->commission,
                                              data->createdDateTime, data->strategyLinkedPositionId);
}

uint StrategyLinkedPositionDetailDb :: insertStrategyLinkedPositionDetail(uint sharesBought, uint sharesSold, float avgBought,
                                                                          float avgSold, float commission, QDateTime createdDateTime,
                                                                          uint strategyLinkedPositionId) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyLinkedPositionDetail(SharesBought, SharesSold, AvgBought, AvgSold, "
                  " Commission, CreatedDateTime, StrategyLinkedPositionId) Values(:SharesBought, "
                  " :SharesSold, :AvgBought, :AvgSold, :Commission, :CreatedDateTime, :StrategyLinkedPositionId )"
                    );

    //query.bindValue(":StrategyLinkedPositionDetailId", QVariant(QUuid :: createUuid()));
	query.bindValue(":SharesBought", sharesBought);
	query.bindValue(":SharesSold", sharesSold);
	query.bindValue(":AvgBought", avgBought);
	query.bindValue(":AvgSold", avgSold);
	query.bindValue(":Commission", commission);
	query.bindValue(":CreatedDateTime", createdDateTime);
	query.bindValue(":StrategyLinkedPositionId", strategyLinkedPositionId);
	//execute
	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
	}
    //qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}

uint StrategyLinkedPositionDetailDb :: insertStrategyLinkedPositionDetails(const QList<StrategyLinkedPositionDetailData*> &list) {
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
    foreach(StrategyLinkedPositionDetailData* item, list) {
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

QList<StrategyLinkedPositionDetailData*> StrategyLinkedPositionDetailDb :: getStrategyLinkedPositionDetails(const uint &strategyLinkedPositionId) {
    QList<StrategyLinkedPositionDetailData*> list;
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
        StrategyLinkedPositionDetailData *item = new StrategyLinkedPositionDetailData();
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
