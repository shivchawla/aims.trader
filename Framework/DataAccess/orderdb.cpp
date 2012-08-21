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
                             "GoodTillDate, StrategyId from Orders");
    qDebug() << "Got " << query.size() << " rows from Orders table" << endl;
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
        item->strategyId = query.value(StrategyId).toUInt();

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
                  "o.GoodTillDate, o.StrategyId from Orders o "
                  "where o.StrategyId = :StrategyId ");

    query.bindValue(":StrategyId", strategyId);

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list; //empty at this point
    }
    qDebug() << "Got " << query.size() << " rows from Orders table" << endl;

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
        item->strategyId = query.value(StrategyId).toUInt();

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
                  "o.GoodTillDate, o.StrategyId from Orders o "
                  "inner join Strategy s on o.StrategyId = s.StrategyId "
                  "and Upper(s.Name) = :StrategyName ");

    query.bindValue(":StrategyName", strategyName.toUpper());

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list; //empty at this point
    }
    qDebug() << "Got " << query.size() << " rows from Orders table" << endl;

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
        item->strategyId = query.value(StrategyId).toUInt();

        list.append(item);
    };
    query.finish();
    db.close();

    return list;
}

OrderData* OrderDb :: getOrderById(const uint &id, const QDateTime &placedDate) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                  "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate, StrategyId"
                  " from Orders where OrderId = :OrderId"
                  " and PlacedDate = :PlacedDate");
    query.bindValue(":OrderId", id);
    query.bindValue(":PlacedDate", placedDate);
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
    item->strategyId = query.value(StrategyId).toUInt();

    item->printDebug();
	query.finish();
	db.close();
	return item;
}

uint OrderDb :: insertOrder(const OrderData* data) {
    return insertOrder(data->orderId, data->limitPrice, data->quantity, data->action, data->status, data->placedDate,
                       data->updatedDate,
                       data->orderType, data->avgFillPrice, data->filledQuantity, data->commission, data->positionAmount,
                       data->instrumentId, data->goodTillDate, data->strategyId);
 }

uint OrderDb :: insertOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint strategyId) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
        return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Orders(OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
              "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate, StrategyId) "
              "Values(:OrderId, :LimitPrice, :Quantity, :Action, :Status, :PlacedDate, :UpdatedDate, :OrderType, "
              ":AvgFillPrice, :FilledQuantity, :Commission, :PositionAmount, :InstrumentId, :GoodTillDate, :StrategyId)");

    query.bindValue(":OrderId", orderId);
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
    query.bindValue(":StrategyId", strategyId);
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

uint OrderDb :: updateOrder(const OrderData* data) {
    return updateOrder(data->orderId, data->limitPrice, data->quantity, data->action, data->status, data->placedDate,
                       data->updatedDate,
                       data->orderType, data->avgFillPrice, data->filledQuantity, data->commission, data->positionAmount,
                       data->instrumentId, data->goodTillDate, data->strategyId);
}

uint OrderDb :: updateOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint strategyId) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update Orders Set LimitPrice = :LimitPrice, Quantity = :Quantity, Action = :Action, Status = :Status, "
                  "UpdatedDate = :UpdatedDate, OrderType = :OrderType, AvgFillPrice = :AvgFillPrice, "
                  "FilledQuantity = :FilledQuantity, Commission = :Commission, PositionAmount = :PositionAmount, "
                  "GoodTillDate = :GoodTillDate "
                  "Where OrderId = :OrderId and PlacedDate=:PlacedDate and InstrumentId = :InstrumentId and StrategyId = :StrategyId");
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
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":OrderId", orderId);
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update table for id " << orderId << endl;
    query.finish();
    db.close();
    return query.size();
}
/*
uint OrderDb :: updateOrderBasedOnOriginalId(float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                            uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint strategyId) {

    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update Orders Set LimitPrice = :LimitPrice, Quantity = :Quantity, Action = :Action, Status = :Status, "
                  "PlacedDate = :PlacedDate, UpdatedDate = :UpdatedDate, OrderType = :OrderType, AvgFillPrice = :AvgFillPrice, "
                  "FilledQuantity = :FilledQuantity, Commission = :Commission, PositionAmount = :PositionAmount, "
                  "GoodTillDate = :GoodTillDate "
                  "Where OriginalOrderId = :StrategyId and InstrumentId = :InstrumentId and Date(UpdatedDate) = :LastUpdatedDate");
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

    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":InstrumentId", instrumentId);
    qDebug() << QDate::currentDate() << endl;
    query.bindValue(":LastUpdatedDate", QDate::currentDate());
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update table for original order id " << strategyId << " and instrument id " << instrumentId << endl;
    query.finish();
    db.close();
    return query.size();
}
*/

QList<OrderData*> OrderDb :: getOrders(uint strategyId, uint instrumentId, QDate placedDate) {
    QList<OrderData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }
    QSqlQuery query = getBlankQuery();
    query.prepare("Select OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, "
                  "AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, "
                  "GoodTillDate, StrategyId from Orders "
                  "Where StrategyId = :StrategyId and InstrumentId = :InstrumentId and Date(PlacedDate) = :PlacedDate");
    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":InstrumentId", instrumentId);

    qDebug() << QDate::currentDate() << endl;

    query.bindValue(":PlacedDate", placedDate);
    bool result = query.exec();
    if (!result) {
        qDebug() << "Could not fetch data for strategy id " << strategyId << " and instrument id " << instrumentId << endl;
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
        item->strategyId = query.value(StrategyId).toUInt();

        list.append(item);
    };

    query.finish();
    db.close();
    return list;
}

uint OrderDb :: deleteOrder(const uint &orderId, const QDate &placedDate) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from Orders where OrderId = :OrderId and Date(PlacedDate) = :PlacedDate");
    query.bindValue(":OrderId", orderId);
    query.bindValue(":PlacedDate", placedDate);
	bool result = query.exec();
	if (!result)
        qDebug() << "Could not delete row with order id " << orderId << " and Placed Date " << placedDate << endl;
	query.finish();
	db.close();
	return query.size();
}

uint OrderDb::insertRow(const OrderData* data)
{
    insertOrder(data);
}

uint OrderDb::deleteRow(const OrderData* data)
{
    insertOrder(data);
}

uint OrderDb::updateRow(const OrderData* data)
{
    updateOrder(data);
}


