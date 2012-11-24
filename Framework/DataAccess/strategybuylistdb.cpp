#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "strategybuylistdb.h"
#include "../Shared/AimsTraderDefs/typedefs.h"

StrategyBuyListDb :: StrategyBuyListDb(void)
{
}
StrategyBuyListDb :: ~StrategyBuyListDb(void)
{
}
StrategyBuyListData StrategyBuyListDb :: getStrategyBuyListById(const uint &id) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyBuyListId, StrategyId, InstrumentId, DeactivatedDate from StrategyBuyList "
                  "where StrategyBuyListId = :StrategyBuyListId ");
    query.bindValue(":StrategyBuyListId", id);
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
    StrategyBuyListData item;// = new StrategyBuyListData();
    item.strategyBuyListId = query.value(StrategyBuyListId).toUInt();
    item.strategyId = query.value(StrategyId).toUInt();
    item.instrumentId = query.value(InstrumentId).toUInt();
    item.deactivatedDate = query.value(DeactivatedDate).toDateTime();
    item.printDebug();
	query.finish();
	db.close();
	return item;
}

QList<StrategyBuyListData> StrategyBuyListDb :: getStrategyBuyLists() {
    QList<StrategyBuyListData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select StrategyBuyListId, StrategyId, InstrumentId, DeactivatedDate from StrategyBuyList ");
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyBuyListData item;// = new StrategyBuyListData();
        item.strategyBuyListId = query.value(StrategyBuyListId).toUInt();
        item.strategyId = query.value(StrategyId).toUInt();
        item.instrumentId = query.value(InstrumentId).toUInt();
        item.deactivatedDate = query.value(DeactivatedDate).toDateTime();
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

QList<StrategyBuyListData> StrategyBuyListDb :: getStrategyBuyListsForStrategy(const QString& strategyName) {
    QList<StrategyBuyListData> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select sbl.StrategyId, sbl.StrategyId, sbl.InstrumentId, sbl.DeactivatedDate, s.Name, i.symbol, i.type "
                 "from StrategyBuyList sbl "
                 "inner join Strategy s on sbl.StrategyId = s.StrategyId "
                 "inner join Instrument i on sbl.InstrumentId = i.InstrumentId "
                 "where s.Name = :Name ");
    query.bindValue(":Name", strategyName);
    bool result = query.exec();
    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while(query.next()) {
        StrategyBuyListData item;// = new StrategyBuyListData();
        item.strategyBuyListId = query.value(StrategyBuyListId).toUInt();
        item.strategyId = query.value(StrategyId).toUInt();
        item.instrumentId = query.value(InstrumentId).toUInt();
        item.deactivatedDate = query.value(DeactivatedDate).toDateTime();
        item.strategyName = query.value(StrategyName).toString();
        item.symbol = query.value(Symbol).toString();
        item.instrumentType = query.value(InstrumentType).value<quint8>();
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

uint StrategyBuyListDb :: insertStrategyBuyList(const StrategyBuyListData& data) {
    return insertStrategyBuyList(data.strategyId, data.instrumentId, data.deactivatedDate);
 }

uint StrategyBuyListDb :: insertStrategyBuyList(uint strategyId, uint instrumentId, QDateTime deactivatedDate) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyBuyList( StrategyId, InstrumentId, DeactivatedDate) "
                  "Values( :StrategyId, :InstrumentId, :DeactivatedDate) "
	);

    query.bindValue(":DeactivatedDate", deactivatedDate);
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":DeactivatedDate", deactivatedDate);
	//execute
	bool result = query.exec();
	if (!result) {
        qDebug() << query.executedQuery() << endl;
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
	}
	qDebug() << "Inserted a row" << endl;
    return 1;
}

uint StrategyBuyListDb :: updateStrategyBuyList(const StrategyBuyListData& data) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Update StrategyBuyList Set StrategyId = :StrategyId, InstrumentId = :InstrumentId, DeactivatedDate = :DeactivatedDate "
                  "Where StrategyBuyListId = :StrategyBuyListId ");
    query.bindValue(":StrategyId", data.strategyId);
    query.bindValue(":StrategyBuyListId", data.strategyBuyListId);
    query.bindValue(":InstrumentId", data.instrumentId);
    query.bindValue(":DeactivatedDate", data.deactivatedDate);

	bool result = query.exec();
	if (!result)
        qDebug() << "Could not update table for strategyBuyListId " << data.strategyBuyListId << endl;
	query.finish();
	db.close();
	return query.size();
}

uint StrategyBuyListDb :: deleteStrategyBuyList(const uint &id) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from StrategyBuyList where StrategyBuyListId = :StrategyBuyListId");
    query.bindValue(":StrategyBuyListId", id);
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

QList<ATContract> StrategyBuyListDb :: getATContractsForStrategy(const QString &strategyName) {
    //qDebug() << "Received " << strategyName << endl;
    QList<ATContract> list;

    QList<StrategyBuyListData> buyListViews = getStrategyBuyListsForStrategy(strategyName);
    foreach(StrategyBuyListData tuple, buyListViews) {
        ATContract aContract;// = new ATContract;
        aContract.contractId = tuple.instrumentId;
        Contract c;// = new Contract;
        c.symbol = tuple.symbol.toStdString();
        c.secType = getSecurityTypeForVendor(tuple.instrumentType, 0); //right now default to Active Tick

        aContract.contract = c;
        list.append(aContract);
    }
    return list;
}

std::string StrategyBuyListDb :: getSecurityTypeForVendor(const quint8 &instrumentType, int vendorCode) {
    //right now we assume that we have only one vendor i.e ActiveTick for which vendor code = 0
    //later we can add more here or send this to db
    switch(instrumentType)   {
        case 0:
            return "STK";
            break;

        case 1:
            return "FUTURE";
            break;

        case 2:
            return "OPTION";
            break;
    }
}

//uint StrategyBuyListDb::insertRow(const StrategyBuyListData& data)
//{
//    insertStrategyBuyList(data);
//}

//uint StrategyBuyListDb::deleteRow(const StrategyBuyListData& data)
//{
//    deleteStrategyBuyList(data.strategyBuyListId);
//}

//uint StrategyBuyListDb::updateRow(const StrategyBuyListData& data)
//{
//    updateStrategyBuyList(data);
//}

