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
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("Select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                             "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, "
                             "GoodTillDate, OriginalOrderId from StratTrader.Order");
    qDebug() << "Got " << query.size() << " rows from StratTrader.Order table" << endl;
    if (!result) return list; //empty at this point
    while (query.next()) {
        OrderData *item = new OrderData();
        item->orderId = query.value(OrderId).toUInt();
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).value<quint8>();
        item->status = query.value(Status).value<quint8>();
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).value<quint8>();
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->instrumentId = query.value(InstrumentId).toUInt();
        item->goodTillDate = query.value(GoodTillDate).toDateTime();
        item->originalOrderId = query.value(OriginalOrderId).toUInt();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

QList<OrderData*> OrderDb :: getOrdersByStrategyId(const uint &strategyId) {
    qDebug() << "Getting orders from db for StrategyId " << strategyId << endl;
    QList<OrderData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select o.OrderId, o.LimitPrice, o.Quantity, o.Action, o.Status, o.PlacedDate, o.UpdatedDate, o.OrderType, "
                  "o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, o.InstrumentId, "
                  "o.GoodTillDate, o.OriginalOrderId from StratTrader.Order o "
                  "inner join Instrument i on o.InstrumentId = i.InstrumentId "
                  "inner join StrategyLinkedPosition p on o.InstrumentId = p.InstrumentId "
                  "inner join Strategy s on p.StrategyId = s.StrategyId "
                  "and s.StrategyId = :StrategyId ");

    query.bindValue(":StrategyId", strategyId);

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
        item->orderId = query.value(OrderId).toUInt();
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).value<quint8>();
        item->status = query.value(Status).value<quint8>();
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).value<quint8>();
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->instrumentId = query.value(InstrumentId).toUInt();
        item->goodTillDate = query.value(GoodTillDate).toDateTime();
        item->originalOrderId = query.value(OriginalOrderId).toUInt();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

QList<OrderData*> OrderDb :: getOrdersByStrategyName(const QString &strategyName) {
    qDebug() << "Getting orders from db for Strategy Name " << strategyName << endl;
    QList<OrderData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select o.OrderId, o.LimitPrice, o.Quantity, o.Action, o.Status, o.PlacedDate, o.UpdatedDate, o.OrderType, "
                  "o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, o.InstrumentId, "
                  "o.GoodTillDate, o.OriginalOrderId from StratTrader.Order o "
                  "inner join Instrument i on o.InstrumentId = i.InstrumentId "
                  "inner join StrategyLinkedPosition p on o.InstrumentId = p.InstrumentId "
                  "inner join Strategy s on p.StrategyId = s.StrategyId "
                  "and s.Name = :StrategyName ");

    query.bindValue(":StrategyName", strategyName);

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
        item->orderId = query.value(OrderId).toUInt();
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).value<quint8>();
        item->status = query.value(Status).value<quint8>();
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).value<quint8>();
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->instrumentId = query.value(InstrumentId).toUInt();
        item->goodTillDate = query.value(GoodTillDate).toDateTime();
        item->originalOrderId = query.value(OriginalOrderId).toUInt();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

OrderData* OrderDb :: getOrderById(const uint &id) {
	qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                  "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate, OriginalOrderId"
                  " from StratTrader.Order where OrderId = :OrderId");
    query.bindValue(":OrderId", id);
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	OrderData *item = new OrderData();
    item->orderId = query.value(OrderId).toUInt();
	item->limitPrice = query.value(LimitPrice).toFloat();
	item->quantity = query.value(Quantity).toUInt();
    item->action = query.value(Action).value<quint8>();
    item->status = query.value(Status).value<quint8>();
	item->placedDate = query.value(PlacedDate).toDateTime();
	item->updatedDate = query.value(UpdatedDate).toDateTime();
    item->orderType = query.value(OrderType).value<quint8>();
	item->avgFillPrice = query.value(AvgFillPrice).toFloat();
	item->filledQuantity = query.value(FilledQuantity).toUInt();
	item->commission = query.value(Commission).toFloat();
	item->positionAmount = query.value(PositionAmount).toFloat();
    item->instrumentId = query.value(InstrumentId).toUInt();
	item->goodTillDate = query.value(GoodTillDate).toDateTime();
    item->originalOrderId = query.value(OriginalOrderId).toUInt();

    item->printDebug();
	query.finish();
	db.close();
	return item;
}

uint OrderDb :: insertOrder(const OrderData* &data) {
    return insertOrder(data->limitPrice, data->quantity, data->action, data->status, data->placedDate,
                       data->updatedDate,
                       data->orderType, data->avgFillPrice, data->filledQuantity, data->commission, data->positionAmount,
                       data->instrumentId, data->goodTillDate, data->originalOrderId);
 }

//returns the new inserted primary key value and now the count of rows inserted
uint OrderDb :: insertOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint originalOrderId) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
        return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StratTrader.Order( LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
              "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate, OriginalOrderId) "
              "Values( :LimitPrice, :Quantity, :Action, :Status, :PlacedDate, :UpdatedDate, :OrderType, "
              ":AvgFillPrice, :FilledQuantity, :Commission, :PositionAmount, :InstrumentId, :GoodTillDate, :OriginalOrderId)");

    //query.bindValue(":OrderId", orderId);
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
    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":GoodTillDate", goodTillDate);
    query.bindValue(":OriginalOrderId", originalOrderId);
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

uint OrderDb :: updateOrder(const OrderData* &data) {
    return updateOrder(data->orderId, data->limitPrice, data->quantity, data->action, data->status, data->placedDate,
                       data->updatedDate,
                       data->orderType, data->avgFillPrice, data->filledQuantity, data->commission, data->positionAmount,
                       data->instrumentId, data->goodTillDate, data->originalOrderId);
}

uint OrderDb :: updateOrder(uint id, float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint originalOrderId) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update StratTrader.Order Set LimitPrice = :LimitPrice, Quantity = :Quantity, Action = :Action, Status = :Status, "
                  "PlacedDate = :PlacedDate, UpdatedDate = :UpdatedDate, OrderType = :OrderType, AvgFillPrice = :AvgFillPrice, "
                  "FilledQuantity = :FilledQuantity, Commission = :Commission, PositionAmount = :PositionAmount, "
                  "InstrumentId = :InstrumentId, GoodTillDate = :GoodTillDate, OriginalOrderId = :OriginalOrderId "
                  "Where OrderId = :OrderId ");
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
    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":GoodTillDate", goodTillDate);
    query.bindValue(":OriginalOrderId", originalOrderId);
    query.bindValue(":OrderId", id);
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update table for id " << id << endl;
    query.finish();
    db.close();
    return query.size();
}

uint OrderDb :: updateOrderBasedOnOriginalId(float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint originalOrderId) {

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update StratTrader.Order Set LimitPrice = :LimitPrice, Quantity = :Quantity, Action = :Action, Status = :Status, "
                  "PlacedDate = :PlacedDate, UpdatedDate = :UpdatedDate, OrderType = :OrderType, AvgFillPrice = :AvgFillPrice, "
                  "FilledQuantity = :FilledQuantity, Commission = :Commission, PositionAmount = :PositionAmount, "
                  "GoodTillDate = :GoodTillDate "
                  "Where OriginalOrderId = :OriginalOrderId and InstrumentId = :InstrumentId and Date(UpdatedDate) = :LastUpdatedDate");
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
    query.bindValue(":GoodTillDate", goodTillDate);

    query.bindValue(":OriginalOrderId", originalOrderId);
    query.bindValue(":InstrumentId", instrumentId);
    qDebug() << QDate::currentDate() << endl;
    query.bindValue(":LastUpdatedDate", QDate::currentDate());
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update table for original order id " << originalOrderId << " and instrument id " << instrumentId << endl;
    query.finish();
    db.close();
    return query.size();
}

QList<OrderData*> OrderDb :: getOrders(uint originalOrderId, uint instrumentId) {
    QList<OrderData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }
    QSqlQuery query = getBlankQuery();
    query.prepare("Select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                  "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, "
                  "GoodTillDate, OriginalOrderId from StratTrader.Order "
                  "Where OriginalOrderId = :OriginalOrderId and InstrumentId = :InstrumentId and Date(UpdatedDate) = :LastUpdatedDate");
    query.bindValue(":OriginalOrderId", originalOrderId);
    query.bindValue(":InstrumentId", instrumentId);

    qDebug() << QDate::currentDate() << endl;

    query.bindValue(":LastUpdatedDate", QDate::currentDate());
    bool result = query.exec();
    if (!result) {
        qDebug() << "Could not update table for original order id " << originalOrderId << " and instrument id " << instrumentId << endl;
        qDebug() << query.lastQuery() << endl;
        qDebug() << query.lastError().text();
    }

    while (query.next()) {
        OrderData *item = new OrderData();
        item->orderId = query.value(OrderId).toUInt();
        item->limitPrice = query.value(LimitPrice).toFloat();
        item->quantity = query.value(Quantity).toUInt();
        item->action = query.value(Action).value<quint8>();
        item->status = query.value(Status).value<quint8>();
        item->placedDate = query.value(PlacedDate).toDateTime();
        item->updatedDate = query.value(UpdatedDate).toDateTime();
        item->orderType = query.value(OrderType).value<quint8>();
        item->avgFillPrice = query.value(AvgFillPrice).toFloat();
        item->filledQuantity = query.value(FilledQuantity).toUInt();
        item->commission = query.value(Commission).toFloat();
        item->positionAmount = query.value(PositionAmount).toFloat();
        item->instrumentId = query.value(InstrumentId).toUInt();
        item->goodTillDate = query.value(GoodTillDate).toDateTime();
        item->originalOrderId = query.value(OriginalOrderId).toUInt();

        list.append(item);
    };

    query.finish();
    db.close();
    return list;
}

uint OrderDb :: deleteOrder(const uint &id) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from StratTrader.Order where OrderId = :OrderId");
    query.bindValue(":OrderId", id);
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

