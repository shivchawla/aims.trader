#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QUuid>

#include "stdafx.h"
#include "InstrumentDb.h"
//#include "BootStrapper.h"

InstrumentDb::InstrumentDb(void)
{
}


InstrumentDb::~InstrumentDb(void)
{
}

InstrumentData* InstrumentDb :: GetInstrumentBySymbol(QString symbol) {
	qDebug() << "Received " << symbol << endl;

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }
    else qDebug() << "Database connected!!" << endl;

    QSqlQuery query;
    query.prepare("select instrument_id, symbol, short_name, full_name, updated_by, updated_date from Instruments where SYMBOL = :SYMBOL");
	query.bindValue(":SYMBOL", symbol); 
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	InstrumentData *i = new InstrumentData();

    i->instrumentId = QUuid::fromRfc4122(query.value(INSTRUMENT_ID).toByteArray());
	i->symbol = query.value(SYMBOL).toString();
	i->shortName = query.value(SHORT_NAME).toString();
	i->fullName = query.value(FULL_NAME).toString();
	i->updatedBy = query.value(UPDATED_BY).toString();
	i->updatedDate = query.value(UPDATED_DATE).toDateTime();

    qDebug() << i->instrumentId << " " << i->symbol << " " << i->shortName << endl;
    query.finish();
    db.close();

	return i;
}

 QList<InstrumentData*> InstrumentDb::GetInstruments() {
    QList<InstrumentData*>* instruments = new QList<InstrumentData*>;
    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return *instruments;
    }
    else qDebug() << "Database connected!!" << endl;

    QSqlQuery query;
    query.prepare("select instrument_id, symbol, short_name, full_name, updated_by, updated_date from Instruments");
    query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData *i = new InstrumentData();

        i->instrumentId = QUuid::fromRfc4122(query.value(INSTRUMENT_ID).toByteArray());
        i->symbol = query.value(SYMBOL).toString();
        i->shortName = query.value(SHORT_NAME).toString();
        i->fullName = query.value(FULL_NAME).toString();
        i->updatedBy = query.value(UPDATED_BY).toString();
        i->updatedDate = query.value(UPDATED_DATE).toDateTime();

        instruments->append(i);
    }

    query.finish();
    db.close();

    qDebug() << "List has " << instruments->count() << " instruments" << endl;
    return *instruments;
}
