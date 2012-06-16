#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "orderdb.h"

OrderDb :: OrderDb(void)
{
}
OrderDb :: ~OrderDb(void)
{
}

QList<OrderData*> OrderDb :: getOrders() {
    qDebug() << "Getting all orders from db" << endl;
    QList<OrderData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("Select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                             "AvgFillPrice, FilledQuantity, Commission, PositionAmount, ExchangeId, InstrumentId, "
                             "GoodTillDate from StratTrader.Order");
    qDebug() << "Got " << query.size() << " rows from StratTrader.Order table" << endl;
    if (!result) return list; //empty at this point
    while (query.next()) {
        OrderData *item = new OrderData();
        item->orderId = QUuid::fromRfc4122(query.value(OrderId).toByteArray());
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).toString()[0];
        item->status = query.value(Status).toString()[0];
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).toString()[0];
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        item->goodTillDate = query.value(GoodTillDate).toDateTime();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

QList<OrderData*> OrderDb :: getOrdersForStrategy(QUuid strategyId) {
    qDebug() << "Getting orders from db for StrategyId " << strategyId << endl;
    QList<OrderData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    query.prepare("select o.OrderId, o.LimitPrice, o.Quantity, o.Action, o.Status, o.PlacedDate, o.UpdatedDate, o.OrderType, "
                  "o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, o.ExchangeId, o.InstrumentId, "
                  "o.GoodTillDate from StratTrader.Order o "
                  "inner join Instrument i on o.InstrumentId = i.InstrumentId "
                  "inner join StrategyLinkedPosition p on o.InstrumentId = p.InstrumentId "
                  "inner join Strategy s on p.StrategyId = s.StrategyId "
                  "and s.StrategyId = StrToUuid(:StrategyId) ");
    query.bindValue(":StrategyId", QVariant(strategyId));

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list; //empty at this point
    }
    qDebug() << "Got " << query.size() << " rows from StratTrader.Order table" << endl;

    while (query.next()) {
        OrderData *item = new OrderData();
        item->orderId = QUuid::fromRfc4122(query.value(OrderId).toByteArray());
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).toString()[0];
        item->status = query.value(Status).toString()[0];
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).toString()[0];
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
        item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        item->goodTillDate = query.value(GoodTillDate).toDateTime();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

OrderData* OrderDb :: getOrderById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, AvgFillPrice, FilledQuantity, Commission, PositionAmount, ExchangeId, InstrumentId, GoodTillDate from StratTrader.Order where OrderId = Unhex(Replace(Replace(Replace(:OrderId, '{', ''), '}', ''), '-', ''))");
	query.bindValue(":OrderId", QVariant(id));
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	OrderData *item = new OrderData();
	item->orderId = QUuid::fromRfc4122(query.value(OrderId).toByteArray());
	item->limitPrice = query.value(LimitPrice).toFloat();
	item->quantity = query.value(Quantity).toUInt();
    item->action = query.value(Action).toString()[0];
    item->status = query.value(Status).toString()[0];
	item->placedDate = query.value(PlacedDate).toDateTime();
	item->updatedDate = query.value(UpdatedDate).toDateTime();
    item->orderType = query.value(OrderType).toString()[0];
	item->avgFillPrice = query.value(AvgFillPrice).toFloat();
	item->filledQuantity = query.value(FilledQuantity).toUInt();
	item->commission = query.value(Commission).toFloat();
	item->positionAmount = query.value(PositionAmount).toFloat();
	item->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
	item->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
	item->goodTillDate = query.value(GoodTillDate).toDateTime();
	qDebug() << item->orderId << endl;
	query.finish();
	db.close();
	return item;
}

unsigned int OrderDb :: insertOrder(const OrderData* data) {
    return insertOrder(data->orderId, data->limitPrice, data->quantity, data->action, data->status, data->placedDate, data->updatedDate,
                       data->orderType, data->avgFillPrice, data->filledQuantity, data->commission, data->positionAmount,
                       data->exchangeId, data->instrumentId, data->goodTillDate);
 }

unsigned int OrderDb :: insertOrder(QUuid orderId, float limitPrice, unsigned int quantity, QChar action, QChar status, QDateTime placedDate, QDateTime updatedDate, QChar orderType, float avgFillPrice, unsigned int filledQuantity, float commission, float positionAmount, QUuid exchangeId, QUuid instrumentId, QDateTime goodTillDate) {
 	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
        return 0;
	}

	//prepare statement
	QSqlQuery query;
    query.prepare("insert into StratTrader.Order(OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
              "AvgFillPrice, FilledQuantity, Commission, PositionAmount, ExchangeId, InstrumentId, GoodTillDate) "
              "Values(StrToUuid(:OrderId), :LimitPrice, :Quantity, :Action, :Status, :PlacedDate, :UpdatedDate, :OrderType, "
              ":AvgFillPrice, :FilledQuantity, :Commission, :PositionAmount, StrtoUuid(:ExchangeId), StrToUuid(:InstrumentId), :GoodTillDate)");

    query.bindValue(":OrderId", QVariant(orderId));
    query.bindValue(":LimitPrice", limitPrice);
    query.bindValue(":Quantity", quantity);
    query.bindValue(":Action", action);
    query.bindValue(":Status", status);
    query.bindValue(":PlacedDate", placedDate);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":OrderType", orderType);
    query.bindValue(":AvgFillPrice", avgFillPrice);
    query.bindValue(":FilledQuantity", filledQuantity);
    query.bindValue(":Commission", commission);
    query.bindValue(":PositionAmount", positionAmount);
    query.bindValue(":ExchangeId", QVariant(exchangeId));
    query.bindValue(":InstrumentId", QVariant(instrumentId));
    query.bindValue(":GoodTillDate", goodTillDate);
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

unsigned int OrderDb :: updateOrder(const OrderData* data, QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Update StratTrader.Order Set LimitPrice = :LimitPrice, Quantity = :Quantity, Action = :Action, Status = :Status, "
                  "PlacedDate = :PlacedDate, UpdatedDate = :UpdatedDate, OrderType = :OrderType, AvgFillPrice = :AvgFillPrice, "
                  "FilledQuantity = :FilledQuantity, Commission = :Commission, PositionAmount = :PositionAmount, "
                  "ExchangeId = StrToUuid(:ExchangeId), InstrumentId = StrToUuid(:InstrumentId), GoodTillDate = :GoodTillDate "
                  "Where OrderId = StrToUuid(:OrderId) ");
	query.bindValue(":LimitPrice", data->limitPrice);
	query.bindValue(":Quantity", data->quantity);
	query.bindValue(":Action", data->action);
	query.bindValue(":Status", data->status);
	query.bindValue(":PlacedDate", data->placedDate);
	query.bindValue(":UpdatedDate", data->updatedDate);
	query.bindValue(":OrderType", data->orderType);
	query.bindValue(":AvgFillPrice", data->avgFillPrice);
	query.bindValue(":FilledQuantity", data->filledQuantity);
	query.bindValue(":Commission", data->commission);
	query.bindValue(":PositionAmount", data->positionAmount);
    query.bindValue(":ExchangeId", QVariant(data->exchangeId));
    query.bindValue(":InstrumentId", QVariant(data->instrumentId));
    query.bindValue(":OrderId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not update table for id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

unsigned int OrderDb :: deleteOrder(QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Delete from StratTrader.Order where OrderId = StrTouuid(:OrderId)");
    query.bindValue(":OrderId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

