#include <QtSql/QSqlError>
//#include "stdafx.h"
#include "exchangedb.h"

ExchangeDb :: ExchangeDb(void)
{
}
ExchangeDb :: ~ExchangeDb(void)
{
}
ExchangeData* ExchangeDb :: getExchangeById(QUuid id) {
	qDebug() << "Received " << id << endl;
	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

	QSqlQuery query;
    query.prepare("select ExchangeId, Name from Exchange where ExchangeId = StrToUuid(:ExchangeId)");
	query.bindValue(":ExchangeId", QVariant(id));
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	ExchangeData *item = new ExchangeData();
	item->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
	item->name = query.value(Name).toString();
	qDebug() << item->exchangeId << endl;
	query.finish();
	db.close();
	return item;
}

QList<ExchangeData*> ExchangeDb :: getExchanges() {
    QList<ExchangeData*> list;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query;
    bool result = query.exec("select ExchangeId, Name from Exchange");

    if (!result) {
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        ExchangeData *item = new ExchangeData();
        item->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
        item->name = query.value(Name).toString();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

unsigned int ExchangeDb :: insertExchange(const ExchangeData* data) {
    return insertExchange(data->name);
}

unsigned int ExchangeDb :: insertExchange(QString name) {
 	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
	QSqlQuery query;
    query.prepare("insert into Exchange(ExchangeId, Name) Values(:ExchangeId, :Name )");

	query.bindValue(":ExchangeId", QVariant(QUuid :: createUuid()));
	query.bindValue(":Name", name);
	//execute
	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
	return 0;
	}
	qDebug() << "Inserted a row" << endl;
	return 1;
}

unsigned int ExchangeDb :: updateExchange(const ExchangeData* data, QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare("Update Exchange Set Name = :Name Where ExchangeId = StrToUuid(:ExchangeId) ");
    query.bindValue(":Name", data->name);
    query.bindValue(":ExchangeId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not update table for id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

unsigned int ExchangeDb :: deleteExchange(QUuid id) {
	qDebug() << "Received " << id << endl;

	if (!db.open()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	QSqlQuery query;
    query.prepare(" Delete from Exchange where ExchangeId = StrToUuid(:ExchangeId) ");
    query.bindValue(":ExchangeId", QVariant(id));
	bool result = query.exec();
	if (!result)
		qDebug() << "Could not delete row with id " << id << endl;
	query.finish();
	db.close();
	return query.size();
}

