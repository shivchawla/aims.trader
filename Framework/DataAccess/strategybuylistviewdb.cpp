#include <QtSql/QSqlError>
#include "strategybuylistviewdb.h"

StrategyBuyListViewDb :: StrategyBuyListViewDb(void)
{
}
StrategyBuyListViewDb :: ~StrategyBuyListViewDb(void)
{
}

//Id here is StrtegyBuyListViewId and not StrategyId
StrategyBuyListViewData* StrategyBuyListViewDb :: getStrategyBuyListViewById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select StrategyBuyListId, StrategyId, StrategyName, InstrumentId, Symbol, InstrumentType "
                  "from StrategyBuyListView where StrategyBuyListId = StrToUuid(:StrategyBuyListId) ");
	query.bindValue(":StrategyBuyListId", QVariant(id));
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	StrategyBuyListViewData *item = new StrategyBuyListViewData();
	item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
	item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
	item->strategyName = query.value(StrategyName).toString();
	item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
	item->symbol = query.value(Symbol).toString();
	item->instrumentType = query.value(InstrumentType).toString();
	qDebug() << item->strategyBuyListId << endl;
	query.finish();
	db.close();
	return item;
}

QList<StrategyBuyListViewData*> StrategyBuyListViewDb :: getStrategyBuyListViews() {
    QList<StrategyBuyListViewData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }
    QSqlQuery query;
    bool result = query.exec("select StrategyBuyListId, StrategyId, StrategyName, InstrumentId, Symbol, InstrumentType "
                  "from StrategyBuyListView ");

    if (!result) {
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        StrategyBuyListViewData *item = new StrategyBuyListViewData();
        item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->strategyName = query.value(StrategyName).toString();
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        item->symbol = query.value(Symbol).toString();
        item->instrumentType = query.value(InstrumentType).toString();
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

QList<StrategyBuyListViewData*> StrategyBuyListViewDb :: getStrategyBuyListViewsForStrategy(QString strategyName) {
    qDebug() << "Received " << strategyName << endl;
    QList<StrategyBuyListViewData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }
    QSqlQuery query;
    query.prepare("select StrategyBuyListId, StrategyId, StrategyName, InstrumentId, Symbol, InstrumentType "
                  "from StrategyBuyListView where StrategyName = :StrategyName");
    query.bindValue(":StrategyName", strategyName);
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        StrategyBuyListViewData *item = new StrategyBuyListViewData();
        item->strategyBuyListId = QUuid::fromRfc4122(query.value(StrategyBuyListId).toByteArray());
        item->strategyId = QUuid::fromRfc4122(query.value(StrategyId).toByteArray());
        item->strategyName = query.value(StrategyName).toString();
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        item->symbol = query.value(Symbol).toString();
        item->instrumentType = query.value(InstrumentType).toString();
        list.append(item);
    }
    query.finish();
    db.close();
    return list;
}

QList<ATContract*> StrategyBuyListViewDb :: getATContractsForStrategy(const QString& strategyName) {
    qDebug() << "Received " << strategyName << endl;
    QList<ATContract*> list;

    QList<StrategyBuyListViewData*> buyListViews = getStrategyBuyListViewsForStrategy(strategyName);
    foreach(StrategyBuyListViewData* tuple, buyListViews) {
        ATContract* aContract = new ATContract;
        aContract->contractId = tuple->instrumentId;
        Contract* c = new Contract;
        c->symbol = tuple->symbol.toStdString();
        c->secType = tuple->instrumentType.toStdString();

        aContract->contract = *c;
        list.append(aContract);
    }

    return list;
}
