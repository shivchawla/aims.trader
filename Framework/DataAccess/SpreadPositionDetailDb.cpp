#include "SpreadPositionDetailDb.h"
#include <QtSql/QSqlError>

SpreadPositionDetailDb::SpreadPositionDetailDb() : DbBase()
{}

PositionDetailData SpreadPositionDetailDb ::  getStrategyLinkedSpreadPositionDetailById(const uint id) {
    qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select spreadPositionDetailId, SharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, Commission, CreatedDateTime, "
                  " positionId from SpreadPositionDetail"
                  " where spreadPositionDetailId = :spreadPositionDetailId ");
    query.bindValue(":PositionDetailId", id);
    query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
        query.finish();
        db.close();
        return NULL;
    }

    PositionDetailData item;// = new PositionDetailData();

    item.positionDetailId = query.value(0).toUInt();
    item.sharesBought = query.value(1).toUInt();
    item.sharesSold = query.value(2).toUInt();
    item.avgBuyPrice = query.value(3).toFloat();
    item.avgSellPrice = query.value(4).toFloat();
    item.commission = query.value(5).toFloat();
    item.createdDateTime = query.value(6).toDateTime();
    item.positionId = query.value(7).toUInt();
    qDebug() << item.positionDetailId << endl;
    query.finish();
    db.close();
    return item;
}

uint SpreadPositionDetailDb :: updateStrategyLinkedSpreadPositionDetail(const uint runId, const uint spreadPositionId, const uint spreadPositiondetailId, const uint sharesBought,
                                        const uint sharesSold, const double AvgBuyPrice,
                                        const double AvgSellPrice, const double commission, const QDateTime& updatedDateTime)
{
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("update SpreadPositionDetail set SharesBought = :SharesBought , SharesSold = :SharesSold, AvgBuyPrice = :AvgBuyPrice, "
                  "AvgSellPrice = :AvgSellPrice, Commission = :Commission, UpdatedDateTime = :UpdatedDateTime "
                  "where runid = :RunId and spreadPositionId = :SpreadPositionId and SpreadPositionDetailId = :SpreadPositionDetailId ");

    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBuyPrice", AvgBuyPrice);
    query.bindValue(":AvgSellPrice", AvgSellPrice);
    query.bindValue(":Commission", commission);
    query.bindValue(":UpdatedDateTime", updatedDateTime);
    query.bindValue(":RunId", runId);
    query.bindValue(":SpreadPositionId", spreadPositionId);
    //query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":SpreadPositionDetailId", spreadPositiondetailId);

    //execute
    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
    }
    //qDebug() << "Inserted a row" << endl;
    return query.lastInsertId().toUInt();
}


uint SpreadPositionDetailDb ::  insertStrategyLinkedSpreadPositionDetail(const uint runId, const uint spreadPositionId, const uint spreadPositionDetailId,
                                                                          const uint sharesBought, const uint sharesSold, const double avgBuyPrice,
                                                                          const double avgSellPrice, const double commission, const QDateTime& createdDate) {
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into SpreadPositionDetail (RunId, SpreadPositionId, SpreadPositionDetailId, SharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate) Values(:RunId, :SpreadPositionId, :SpreadPositionDetailId, :SharesBought, "
                  " :SharesSold, :AvgBuyPrice, :AvgSellPrice, :Commission, :CreatedDate)"
                  );

    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBuyPrice", avgBuyPrice);
    query.bindValue(":AvgSellPrice", avgSellPrice);
    query.bindValue(":Commission", commission);
    query.bindValue(":CreatedDate", createdDate);
    query.bindValue(":RunId", runId);
    query.bindValue(":SpreadPositionId", spreadPositionId);
    query.bindValue(":SpreadPositionDetailId", spreadPositionDetailId);

    //execute
    bool result = query.exec();
    if (!result) {
        qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
    }
    //qDebug() << "Inserted a row" << endl;

    return query.lastInsertId().toUInt();
}

uint SpreadPositionDetailDb ::  insertStrategyLinkedSpreadPositionDetails(const QList<PositionDetailData> &list) {
    //check database if available to work with
    if (!openDatabase()) {
        return 0; //to signify zero inserted rows
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    db.transaction();
    query.prepare("insert into SpreadPositionDetail (SharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDateTime, SpreadPositionId) "
                  " Values(:SharesBought, :SharesSold, :AvgBuyPrice, :AvgSellPrice, :Commission, :CreatedDateTime, :SpreadPositionId )"
                );
    uint ctr=0;
    foreach(PositionDetailData item, list) {
        query.bindValue(":SharesBought", item.sharesBought);
        query.bindValue(":SharesSold", item.sharesSold);
        query.bindValue(":AvgBuyPrice", item.avgBuyPrice);
        query.bindValue(":AvgSellPrice", item.avgSellPrice);
        query.bindValue(":Commission", item.commission);
        query.bindValue(":CreatedDateTime", item.createdDateTime);
        query.bindValue(":SpreadPositionId", item.positionId);

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

QList<PositionDetailData> SpreadPositionDetailDb :: getStrategyLinkedSpreadPositionDetails(const uint spreadPositionId) {
    QList<PositionDetailData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select SpreadPositionDetailId, SharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, Commission, CreatedDateTime, "
                  " SpreadPositionId from SpreadPositionDetail "
                  " where SpreadPositionId = :SpreadPositionId ");
    query.bindValue(":StrategyPositionId", spreadPositionId);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        db.close();
        return list;
    }

    while (query.next()) {
        PositionDetailData item;// = new PositionDetailData();
        item.positionDetailId = query.value(0).toUInt();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.commission = query.value(5).toFloat();
        item.createdDateTime = query.value(6).toDateTime();
        item.positionId = query.value(7).toUInt();
        qDebug() << item.positionDetailId << endl;
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}
