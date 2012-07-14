#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QDateTime>

#include "DataAccess/InstrumentDb.h"
//#include "BootStrapper.h"

InstrumentDb::InstrumentDb():DbBase()
{
}


InstrumentDb::~InstrumentDb()
{
}

InstrumentData* InstrumentDb :: getInstrumentBySymbol(QString symbol, quint8 type) {
    //qDebug() << "Received " << symbol << endl;

    if (!openDatabase()) {
        return NULL;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode "
                  "from Instrument where Symbol = :SYMBOL and Type = :TYPE");
	query.bindValue(":SYMBOL", symbol); 
    query.bindValue(":TYPE", type);
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

    i->instrumentId = query.value(InstrumentId).toUInt();
    i->symbol = query.value(Symbol).toString();
    i->shortName = query.value(ShortName).toString();
    i->fullName = query.value(FullName).toString();
    i->type = query.value(Type).value<quint8>();
    i->sectorCode = query.value(SectorCode).toString();
    i->exchangeCode = query.value(ExchangeCode).toString();
    i->countryCode = query.value(CountryCode).toString();

    //i->printDebug();
    query.finish();
    db.close();

	return i;
}

 QList<InstrumentData*> InstrumentDb::getInstruments() {
    QList<InstrumentData*> instruments;

    if (!openDatabase()) {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode from Instrument");
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;

    }
    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData *i = new InstrumentData();

        i->instrumentId = query.value(InstrumentId).toUInt();
        i->symbol = query.value(Symbol).toString();
        i->shortName = query.value(ShortName).toString();
        i->fullName = query.value(FullName).toString();
        i->type = query.value(Type).value<quint8>();
        qDebug() << " SectorCode: " << query.value(SectorCode).toString() << endl;
        i->sectorCode = query.value(SectorCode).toString();
        i->exchangeCode = query.value(ExchangeCode).toString();
        i->countryCode = query.value(CountryCode).toString();

        instruments.append(i);
    }

    query.finish();
    db.close();

    //qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}

uint InstrumentDb::insertInstrument(const InstrumentData* &data) {
    return insertInstrument(data->symbol, data->shortName,
                            data->fullName, data->type, data->sectorCode,
                            data->exchangeCode, data->countryCode);
}

uint InstrumentDb::insertInstrument(QString symbol,QString shortName, QString fullName,
                                    quint8 type, QString sectorCode, QString exchangeCode, QString countryCode) {
    if (!openDatabase())
        return 0;

    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode)"
                  "values(:Symbol, :ShortName, :FullName, :Type, :SectorCode, :ExchangeCode, :CountryCode) ");

    query.bindValue(":Symbol", symbol);
    query.bindValue(":ShortName", shortName);
    query.bindValue(":FullName", fullName);
    query.bindValue(":Type", type);
    query.bindValue(":SectorCode", sectorCode);
    query.bindValue(":ExchangeCode", exchangeCode);
    query.bindValue(":CountryCode", countryCode);

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }
    query.finish();
    db.close();
    return (result ? 1 : 0);
}

uint InstrumentDb :: insertInstruments(const QList<InstrumentData*> &list) {
    //check database if available to work with
    if (!openDatabase()) {
        return 0; //to signify zero inserted rows
    }

    //prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode)"
                  "values(:Symbol, :ShortName, :FullName, :Type, :SectorCode, :ExchangeCode, :CountryCode) ");

    //QVariantList symbolList, shortNameList, fullNameList, typeList, sectorCodeList, exchangeCodeList, countryCodeList;
    uint ctr=0;

    db.transaction(); //start a transaction

    foreach(InstrumentData* iData, list) {
        query.bindValue(":Symbol", iData->symbol);
        query.bindValue(":ShortName", iData->shortName);
        query.bindValue(":FullName", iData->fullName);
        query.bindValue(":Type", iData->type);
        query.bindValue(":SectorCode", iData->sectorCode);
        query.bindValue(":ExchangeCode", iData->exchangeCode);
        query.bindValue(":CountryCode", iData->countryCode);

        ctr += (query.exec() ? 1 : 0);
    }

    db.commit(); // commit all records;

    if (ctr < list.count()) {
        qDebug() << "Couldn't insert instrument data rows. "
                 << "Error: " << query.lastError().text() << " " << endl;
        qDebug() << query.lastQuery() << endl;
        return ctr;
    }
    db.close();
    return ctr;
}


QDateTime InstrumentDb :: getNextHistoryUpdateDate(const uint &instrumentId) {

    if (!openDatabase()) {
        return QDateTime();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select ConfValue from StratTrader.InstrumentConfiguration where ConfKey = 'DailyBarLastUpdated' "
                  "and InstrumentId = :InstrumentId ");
    query.bindValue(":InstrumentId", instrumentId);

    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch InstrumentConfiguration row. Error: " << query.lastError().text() << endl;
        db.close();
        return QDateTime();
    }

    query.next();

    qDebug()<<query.value(0);
    QDateTime lastHistoryDate = QDateTime::fromString(query.value(0).toString(), "dd-MMM-yyyy");
    //qDebug() << "Returning " << lastHistoryDate << endl;

    query.finish();
    db.close();

    return lastHistoryDate.addDays(1);
}

QHash<uint, QDateTime> InstrumentDb::getLastHistoryUpdateDateForAllInstruments()
{

    if (!openDatabase()) {
        return QHash<uint, QDateTime>();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, ConfValue from StratTrader.InstrumentConfiguration where ConfKey = 'DailyBarLastUpdated'");

    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        qDebug() << "Could not fetch InstrumentConfiguration rows. Error: " << query.lastError().text() << endl;
        db.close();
        return QHash<uint, QDateTime>();
    }

    QHash<uint, QDateTime> lastUpdatedHistoryDateTimeMap;
    while(query.next())
    {
       uint key = query.value(0).toUInt();
       QDateTime value = QDateTime::fromString(query.value(1).toString(),"dd-MMM-yyyy");
       lastUpdatedHistoryDateTimeMap.insert(key,value);
    }

    query.finish();
    db.close();

    return lastUpdatedHistoryDateTimeMap;
}



void InstrumentDb::updateDailyHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate)
{
    if (!openDatabase()) {
        return;
    }

    QSqlQuery query = getBlankQuery();
    QString dateTime = lastDate.toString("dd-MMM-yyyy");

    query.prepare("insert into StratTrader.InstrumentConfiguration (InstrumentId, ConfKey, ConfValue)"
                  "values(:InstrumentId,:ConfKey,:ConfValue)"
                  "on duplicate key update ConfValue = :ConfValue1" );


    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":ConfValue", dateTime);
    query.bindValue(":ConfValue1", dateTime);
    query.bindValue(":ConfKey","DailyHistoryBarLastUpdated");
    bool result;


    if(!(result = query.exec())) {

        qDebug() <<"Instrument configuration update failed for instrumentid " << instrumentId << endl;
    }

    qDebug() << query.executedQuery();

    query.finish();
    db.close();
}


