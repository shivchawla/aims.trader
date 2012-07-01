#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QUuid>

#include "DataAccess/InstrumentDb.h"
//#include "BootStrapper.h"

InstrumentDb::InstrumentDb(void):DbBase()
{
}


InstrumentDb::~InstrumentDb(void)
{
}

InstrumentData* InstrumentDb :: getInstrumentBySymbol(QString symbol) {
	qDebug() << "Received " << symbol << endl;

    if (!openDatabase())
    {
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate from Instruments where Symbol = :SYMBOL");
	query.bindValue(":SYMBOL", symbol); 
	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
		query.finish();
        db.close();
		return NULL;
	}
	InstrumentData *i = new InstrumentData();

    i->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
    i->symbol = query.value(Symbol).toString();
    i->shortName = query.value(ShortName).toString();
    i->fullName = query.value(FullName).toString();
    i->type = query.value(Type).toChar();
    i->updatedBy = query.value(UpdatedBy).toString();
    i->updatedDate = query.value(UpdatedDate).toDateTime();

    qDebug() << i->instrumentId << " " << i->symbol << " " << i->shortName << endl;
    query.finish();
    db.close();

	return i;
}

 QList<InstrumentData*> InstrumentDb::getInstruments() {
    QList<InstrumentData*> instruments;

    if (!openDatabase())
    {
        return;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate from Instrument");
    query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData *i = new InstrumentData();

        i->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        i->symbol = query.value(Symbol).toString();
        i->shortName = query.value(ShortName).toString();
        i->fullName = query.value(FullName).toString();
        i->type = query.value(Type).toChar();
        i->updatedBy = query.value(UpdatedBy).toString();
        i->updatedDate = query.value(UpdatedDate).toDateTime();

        instruments.append(i);
    }

    query.finish();
    db.close();

    qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}
