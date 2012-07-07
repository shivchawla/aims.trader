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
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate, ExchangeId, CountryId "
                  "from Instruments where Symbol = :SYMBOL");
	query.bindValue(":SYMBOL", symbol); 
	query.exec();
    //qDebug() << "Got " << query.size() << " rows" << endl;
    if (!query.next()) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch InstrumentData row. Error: " << query.lastError().text() << endl;
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
    i->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
    i->countryId = QUuid::fromRfc4122(query.value(CountryId).toByteArray());

    //qDebug() << i->instrumentId << " " << i->symbol << " " << i->shortName << endl;
    query.finish();
    db.close();

	return i;
}

 QList<InstrumentData*> InstrumentDb::getInstruments() {
    QList<InstrumentData*> instruments;

    if (!openDatabase())
    {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate, ExchangeId, CountryId from Instrument");
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;

    }
    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData *i = new InstrumentData();

        i->instrumentId = QUuid::fromRfc4122(query.value(InstrumentId).toByteArray());
        i->symbol = query.value(Symbol).toString();
        i->shortName = query.value(ShortName).toString();
        i->fullName = query.value(FullName).toString();
        i->type = query.value(Type).toChar();
        i->updatedBy = query.value(UpdatedBy).toString();
        i->updatedDate = query.value(UpdatedDate).toDateTime();
        i->exchangeId = QUuid::fromRfc4122(query.value(ExchangeId).toByteArray());
        i->countryId = QUuid::fromRfc4122(query.value(CountryId).toByteArray());

        instruments.append(i);
    }

    query.finish();
    db.close();

    //qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}

unsigned int InstrumentDb::insertInstrument(const InstrumentData* data) {
    return insertInstrument(data->instrumentId, data->symbol, data->shortName,
                            data->fullName, data->type, data->updatedBy, data->updatedDate,
                            data->exchangeId, data->countryId);
}

unsigned int InstrumentDb::insertInstrument(QUuid instrumentId,QString symbol,QString shortName,QString fullName,
                                            QChar type, QString updatedBy, QDateTime updatedDate, QUuid exchangeId, QUuid countryId) {
    if (!openDatabase())
        return 0;

    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Instrument(InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate, ExchangeId, CountryId)"
                  "values(:InstrumentId, :Symbol, :ShortName, :FullName, :Type, :UpdatedBy, :UpdatedDate, :ExchangeId, :CountryId) ");

    query.bindValue(":InstrumentId", QVariant(instrumentId));
    query.bindValue(":Symbol", symbol);
    query.bindValue(":ShortName", shortName);
    query.bindValue(":FullName", fullName);
    query.bindValue(":Type", type);
    query.bindValue(":UpdatedBy", updatedBy);
    query.bindValue(":UpdatedDate", updatedDate);
    query.bindValue(":ExchangeId", QVariant(exchangeId));
    query.bindValue(":CountryId", QVariant(countryId));

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }
    query.finish();
    db.close();
    return (result ? 1 : 0);
}

unsigned int InstrumentDb::insertInstruments(const QList<InstrumentData*> list) {
    //check database if available to work with
    if (!openDatabase())
    {
        return 0; //to signify zero inserted rows
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Instrument(InstrumentId, Symbol, ShortName, FullName, Type, UpdatedBy, UpdatedDate, ExchangeId, CountryId)"
                  "values(:InstrumentId, :Symbol, :ShortName, :FullName, :Type, :UpdatedBy, :UpdatedDate, :ExchangeId, :CountryId) ");

    QVariantList idList, symbolList, shortNameList, fullNameList, typeList, updatedByList, updatedDateList, exchangeIdList, countryIdList;

    foreach(InstrumentData* iData, list) {
        idList.append(QVariant(QUuid :: createUuid()));
        symbolList.append(iData->symbol);
        shortNameList.append(iData->shortName);
        fullNameList.append(iData->fullName);
        typeList.append(iData->type);
        updatedByList.append(iData->updatedBy);
        updatedDateList.append(iData->updatedDate);
        exchangeIdList.append(QVariant(iData->exchangeId));
        countryIdList.append(QVariant(iData->countryId));
    }

    //bind
    query.addBindValue(idList);
    query.addBindValue(symbolList);
    query.addBindValue(shortNameList);
    query.addBindValue(fullNameList);
    query.addBindValue(typeList);
    query.addBindValue(updatedByList);
    query.addBindValue(updatedDateList);
    query.addBindValue(exchangeIdList);
    query.addBindValue(countryIdList);

    //execute
    bool result = query.execBatch();
    if (!result) {
        qDebug() << "Couldn't insert instrument data rows. "
                 << "Error: " << query.lastError().text() << " " << endl;
        qDebug() << query.lastQuery() << endl;
        return 0;
    }
    db.close();
    return list.count();
}


QDateTime InstrumentDb :: getLastHistoryDate(const QUuid instrumentId) {

    if (!openDatabase()) {
        return QDateTime();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("SELECT ConfigValue FROM StratTrader.InstrumentConfiguration where ConfigParameter = 'DailyBarLastUpdated' "
                  "and InstrumentId = StrToUuid(:InstrumentId) ");
    query.bindValue(":InstrumentId", QVariant(instrumentId));

    bool result = query.exec();
    //qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch InstrumentData row. Error: " << query.lastError().text() << endl;
        db.close();
        return QDateTime();
    }

    query.next();
    QDateTime lastHistoryDate = query.value(0).toDateTime();
    //qDebug() << "Returning " << lastHistoryDate << endl;

    query.finish();
    db.close();

    return lastHistoryDate;
}

void InstrumentDb::updateDailyHistoryBarDate(const QUuid instrumentId)
{
    if (!openDatabase())
    {
        return;
    }

    QSqlQuery query = getBlankQuery();
    QString date = QDateTime::currentDateTime().toString("dd-mmm-yyyy");
    query.prepare("update StratTrader.InstrumentConfiguration set ConfigValue =" + date + "where ConfigParameter = 'DailyBarLastUpdated' "
                  "and InstrumentId = StrToUuid(:InstrumentId)");
    query.bindValue(":InstrumentId", QVariant(instrumentId));
    if(bool result = query.exec())
    {

    }
    query.finish();
}


