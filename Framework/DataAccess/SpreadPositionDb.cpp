#include "SpreadPositionDb.h"
#include <QtSql/QSqlError>

SpreadPositionDb::SpreadPositionDb():DbBase()
{}

PositionData SpreadPositionDb :: getStrategyLinkedSpreadPositionById(const uint id) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return PositionData();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select SpreadPositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate "
                  "from SpreadPosition where PositionId = :PositionId ");

    query.bindValue(":PositionId", id);
    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return PositionData();
    }
    query.next();
    PositionData item;// = new PositionData();
    item.sharesBought = query.value(1).toUInt();
    item.sharesSold = query.value(2).toUInt();
    item.avgBuyPrice = query.value(3).toDouble();
    item.avgSellPrice = query.value(4).toDouble();
    item.createdDate = query.value(6).toDateTime();
    item.updatedDate = query.value(7).toDateTime();
    query.finish();
    db.close();
    return item;
}

QList<PositionData> SpreadPositionDb :: getStrategyLinkedSpreadPositions() {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select SpreadPositionId, sharesBought, sharesSold, avgBuyPrice, avgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate "
                  "from SpreadPosition ");
    //query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        PositionData item;// = new PositionData();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.commission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<PositionData> SpreadPositionDb :: getOpenStrategyLinkedSpreadPositions(uint strategyId) {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select SpreadPositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate "
                  "from SpreadPosition "
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
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.commission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

QList<PositionData> SpreadPositionDb :: getSpreadPositionsForStrategy(const uint strategyId) {
    QList<PositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select SpreadPositionId, sharesBought, SharesSold, AvgBuyPrice, AvgSellPrice, "
                  " Commission, CreatedDate, UpdatedDate"
                  "from SpreadPosition where StrategyId = :StrategyId ");

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
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.commission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

//Returns the last inserted primary key
uint SpreadPositionDb :: insertStrategyLinkedSpreadPosition(const uint runId, const uint spreadPositionId, const uint sharesBought, const uint sharesSold, const double avgBuyPrice, const double avgSellPrice,
                                                const double totalAmountCommission, const QDateTime& createdDate) {
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into SpreadPosition (RunId, SpreadPositionId, SharesBought, SharesSold, AvgBuyPrice, "
                  "AvgSellPrice, Commission, CreatedDate) Values(:RunId, :SpreadPositionId, :SharesBought, :SharesSold, "
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
    query.bindValue(":SpreadPositionId", spreadPositionId);
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

uint SpreadPositionDb :: updateStrategyLinkedSpreadPosition(const uint runId, const uint spreadPositionId, const uint sharesBought, const uint sharesSold, const double avgBuyPrice, const double avgSellPrice,
                                  const double totalAmountCommission, const QDateTime& updatedDate) {

    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("update SpreadPosition set sharesBought = :SharesBought, SharesSold = :SharesSold, "
                  "AvgBuyPrice = :AvgBuyPrice, AvgSellPrice = :AvgSellPrice, "
                  " Commission = :TotalAmountCommission, "
                  " UpdatedDate = :UpdatedDate "
                  " where SpreadPositionId = :SpreadPositionId and RunId = :RunId ");

    query.bindValue(":RunId", runId);
    query.bindValue(":SharesBought", sharesBought);
    query.bindValue(":SharesSold", sharesSold);
    query.bindValue(":AvgBuyPrice", avgBuyPrice);
    query.bindValue(":AvgSellPrice", avgSellPrice);
    query.bindValue(":TotalAmountCommission", totalAmountCommission);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":SpreadPositionId", spreadPositionId);

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

uint SpreadPositionDb :: deleteStrategyLinkedSpreadPosition(const uint id) {
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from SpreadPosition where SpreadPositionId = :SpreadPositionId");
    query.bindValue(":SpreadPositionId", id);
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not delete row with id " << id << endl;
    query.finish();
    db.close();
    return query.size();
}

QList<SpreadPositionData> SpreadPositionDb :: getOpenStrategyLinkedSpreadPositions(const uint runId, const QString& strategyName) {
    QList<SpreadPositionData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select spos.spreadpositionId, spos.sharesBought, spos.SharesSold, spos.AvgBuyPrice, spos.AvgSellPrice, "
                  " spos.Commission, spos.CreatedDate, spos.UpdatedDate, slsp.instrumentId, slsp.spreadId from SpreadPosition spos "
                  " inner join strategylinkedspreadposition slsp on slsp.spreadpositionId = spos.spreadpositionId and slsp.runId = spos.runId"
                  " inner join strategyrun srun on sRun.strategyId = slsp.strategyId and sRun.RunId = slsp.RunId "
                  " where spos.sharesBought != spos.SharesSold and sRun.StrategyName = :StrategyName and sRun.RunId = :RunId");

    query.bindValue(":StrategyName", strategyName);
    query.bindValue(":RunId", runId);

    bool result = query.exec();

    if (!result) {
        query.finish();
        db.close();
        return list;
    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        SpreadPositionData item;// = new PositionData();
        item.sharesBought = query.value(1).toUInt();
        item.sharesSold = query.value(2).toUInt();
        item.avgBuyPrice = query.value(3).toFloat();
        item.avgSellPrice = query.value(4).toFloat();
        item.commission = query.value(5).toFloat();
        item.createdDate = query.value(6).toDateTime();
        item.updatedDate = query.value(7).toDateTime();
        item.instrumentId = query.value(8).toUInt();
        item.spreadId = query.value(9).toUInt();

        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}


