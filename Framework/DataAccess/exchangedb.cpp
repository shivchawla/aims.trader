#include <QtSql/QSqlError>
#include "exchangedb.h"

ExchangeDb :: ExchangeDb(void)
{
}
ExchangeDb :: ~ExchangeDb(void)
{
}
ExchangeData* ExchangeDb :: getExchangeByCode(const QString &code) {
    //qDebug() << "Received " << id << endl;
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return NULL;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select Code, Name from Exchange where Code = :Code");
    query.bindValue(":Code", code);
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	ExchangeData *item = new ExchangeData();
    item->code = query.value(Code).toString();
    item->name = query.value(Name).toString();
    item->printDebug();
	query.finish();
	db.close();
	return item;
}

QList<ExchangeData*> ExchangeDb :: getExchanges() {
    QList<ExchangeData*> list;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return list;
    }

    QSqlQuery query = getBlankQuery();
    bool result = query.exec("select Code, Name from Exchange");

    if (!result) {
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return list;
    }
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        ExchangeData *item = new ExchangeData();
        item->code = query.value(Code).toString();
        item->name = query.value(Name).toString();
        list.append(item);
    }

    query.finish();
    db.close();
    return list;
}

uint ExchangeDb :: insertExchange(const ExchangeData* &data) {
    return insertExchange(data->code, data->name);
}

uint ExchangeDb :: insertExchange(const QString &code,const QString &name) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Exchange(Code, Name) Values(:Code, :Name )");

    query.bindValue(":Code", code);
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

uint ExchangeDb :: updateExchange(const ExchangeData* &data) {
    //qDebug() << "Received " << id << endl;

    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Update Exchange Set Name = :Name Where Code = :Code ");
    query.bindValue(":Name", data->name);
    query.bindValue(":Code", data->code);
	bool result = query.exec();
    if (!result)
        qDebug() << "Could not update table for exchange code " << data->code << endl;

    query.finish();
	db.close();
	return query.size();
}

uint ExchangeDb :: deleteExchange(const QString &code) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare(" Delete from Exchange where Code = :Code ");
    query.bindValue(":Code", code);
	bool result = query.exec();
	if (!result)
        qDebug() << "Could not delete row with exchange code " << code << endl;
	query.finish();
	db.close();
	return query.size();
}

