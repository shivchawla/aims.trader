#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QDateTime>
#include "DataAccess/InstrumentDb.h"
#include <Utils/Log.h>

InstrumentDb::InstrumentDb():DbBase()
{}

InstrumentDb::~InstrumentDb()
{}

InstrumentData InstrumentDb :: getInstrumentBySymbol(QString symbol, quint8 type)
{
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
    InstrumentData i;// = new InstrumentData();

    i.instrumentId = query.value(DbInstrumentId).toUInt();
    i.symbol = query.value(Symbol).toString();
    i.shortName = query.value(ShortName).toString();
    i.fullName = query.value(FullName).toString();
    i.type = query.value(Type).value<quint8>();
    i.sectorCode = query.value(SectorCode).toString();
    i.exchangeCode = query.value(ExchangeCode).toString();
    i.countryCode = query.value(CountryCode).toString();

    //i.printDebug();
    query.finish();
    db.close();

	return i;
}

 QList<InstrumentData> InstrumentDb::getInstruments()
 {
    QList<InstrumentData> instruments;

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
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instruments.append(i);
    }

    query.finish();
    db.close();

    //qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}

 QList<InstrumentData> InstrumentDb::getInstruments(const QList<InstrumentId>& instrumentIdList)
 {
    QList<InstrumentData> instruments;

    if (!openDatabase()) {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode from Instrument"
                  " where InstrumentId  = :InstrumentId");

    foreach(InstrumentId instrumentId, instrumentIdList) {
        query.bindValue(":InstrumentId", instrumentId);
      }

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;

    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instruments.append(i);
    }

    query.finish();
    db.close();

    //qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}

 InstrumentData InstrumentDb::getInstrument(const InstrumentId instrumentId)
 {
    InstrumentData instrument;

    if (!openDatabase()) {
        return instrument;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode from Instrument"
                  " where InstrumentId  = :InstrumentId");

    query.bindValue(":InstrumentId", instrumentId);

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instrument = i;
    }

    query.finish();
    db.close();

    //qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instrument;
}

uint InstrumentDb::insertInstrument(const InstrumentData& data) {
    return insertInstrument(data.symbol, data.shortName,
                            data.fullName, data.type, data.sectorCode,
                            data.exchangeCode, data.countryCode);
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

uint InstrumentDb :: insertInstruments(const QList<InstrumentData>& list) {
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

    foreach(InstrumentData iData, list) {
        query.bindValue(":Symbol", iData.symbol);
        query.bindValue(":ShortName", iData.shortName);
        query.bindValue(":FullName", iData.fullName);
        query.bindValue(":Type", iData.type);
        query.bindValue(":SectorCode", iData.sectorCode);
        query.bindValue(":ExchangeCode", iData.exchangeCode);
        query.bindValue(":CountryCode", iData.countryCode);

        ctr += (query.exec() ? 1 : 0);
    }

    db.commit(); // commit all records;

    if (ctr < list.count()) {
        qDebug() << "Couldn't insert instrument data rows. "
                 << "Error: " << query.lastError().text() << " " << endl;
        qDebug() << query.lastQuery() << endl;
        Logger::log() << QDateTime::currentDateTime() << " Failed Inserting Instrument Data"<<endl;
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

QHash<uint, QDateTime> InstrumentDb::getLastDailyHistoryUpdateDateForAllInstruments()
{
    if (!openDatabase()) {
        return QHash<uint, QDateTime>();
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, max(HistoryDate) from StratTrader.DailyHistoryBar group by InstrumentId");

    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        Logger::log() << QDateTime::currentDateTime() << "Failed fetching InstrumentConfiguration rows"<<endl;
        db.close();
        return QHash<uint, QDateTime>();
    }

    QHash<uint, QDateTime> lastUpdatedDailyHistoryDateTimeMap;
    while(query.next())
    {
       uint key = query.value(0).toUInt();
       QDateTime value = QDateTime::fromString(query.value(1).toString(), Qt::ISODate);
       lastUpdatedDailyHistoryDateTimeMap.insert(key,value);
    }

    query.finish();
    db.close();

    return lastUpdatedDailyHistoryDateTimeMap;
}


QHash<uint, QDateTime> InstrumentDb::getLastIntradayHistoryUpdateDateForAllInstruments1(const int frequency)
{
    if (!openDatabase()) {
        return QHash<uint, QDateTime>();
    }

    QSqlQuery query = getBlankQuery();

    switch(frequency)
    {
        case 1: query.prepare("select InstrumentId, ConfValue from StratTrader.InstrumentConfiguration where ConfKey = 'IntradayHistoryBarLastUpdated'");
                break;
        case 2: query.prepare("select InstrumentId, ConfValue from StratTrader.InstrumentConfiguration where ConfKey = 'TwoMinuteHistoryBarLastUpdated'");
                break;
        case 5: query.prepare("select InstrumentId, ConfValue from StratTrader.InstrumentConfiguration where ConfKey = 'FiveMinuteHistoryBarLastUpdated'");
                break;
    }


    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        Logger::log() << QDateTime::currentDateTime() << "Failed fetching InstrumentConfiguration rows"<<endl;
        db.close();
        return QHash<uint, QDateTime>();
    }

    QHash<uint, QDateTime> lastUpdatedIntradayHistoryDateTimeMap;
    while(query.next())
    {
       uint key = query.value(0).toUInt();
       QDateTime value = QDateTime::fromString(query.value(1).toString(), Qt::ISODate);
       lastUpdatedIntradayHistoryDateTimeMap.insert(key,value);
    }

    query.finish();
    db.close();

    return lastUpdatedIntradayHistoryDateTimeMap;
}

QHash<uint, QHash<uint, QDateTime> > InstrumentDb::getLastIntradayHistoryUpdateDateForAllInstruments()
{
    if (!openDatabase()) {
        return QHash<uint, QHash<uint, QDateTime> >();
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("select InstrumentId, Frequency, max(HistoryDateTime) from StratTrader.IntradayHistoryBar group by InstrumentId, Frequency");

    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        query.finish();
        qDebug() << query.executedQuery() << endl;
        Logger::log() << QDateTime::currentDateTime() << "Failed fetching InstrumentConfiguration rows"<<endl;
        db.close();
         return QHash<uint, QHash<uint, QDateTime> >();
    }

    QHash<uint, QHash<uint, QDateTime> > lastUpdatedIntradayHistoryDateTimeMap;
    while(query.next())
    {
       uint instrumentId = query.value(0).toUInt();
       uint frequency = query.value(1).toUInt();
       QDateTime value = QDateTime::fromString(query.value(2).toString(), Qt::ISODate);

       lastUpdatedIntradayHistoryDateTimeMap[instrumentId][frequency] = value;
    }

    query.finish();
    db.close();

    return lastUpdatedIntradayHistoryDateTimeMap;
}


bool InstrumentDb::updateDailyHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate)
{
    return updateInstrumentConfiguration(instrumentId, "DailyHistoryBarLastUpdated", lastDate.toString(Qt::ISODate));
}

bool InstrumentDb::updateDailyHistoryBarDate(const QHash<uint, QDateTime>& lastDates)
{
    QHashIterator<uint, QDateTime> i(lastDates);
    while (i.hasNext()) {
        i.next();
        return updateInstrumentConfiguration(i.key(), "DailyHistoryBarLastUpdated", i.value().toString(Qt::ISODate));
    }
}

bool InstrumentDb::updateIntradayHistoryBarDate(const QHash<uint, QDateTime>& lastDates, const int frequency)
{
    QHashIterator<uint, QDateTime> i(lastDates);
    while (i.hasNext()) {
        i.next();
        return updateInstrumentConfiguration(i.key(), "IntradayHistoryBarLastUpdated", i.value().toString(Qt::ISODate));
    }
}

bool InstrumentDb::updateIntradayHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate, const int frequency)
{
    switch(frequency)
    {
        case 1:
            return updateInstrumentConfiguration(instrumentId, "IntradayHistoryBarLastUpdated", lastDate.toString(Qt::ISODate)); break;
        case 2:
            return updateInstrumentConfiguration(instrumentId, "TwoMinuteHistoryBarLastUpdated", lastDate.toString(Qt::ISODate)); break;
        case 5:
            return updateInstrumentConfiguration(instrumentId, "FiveMinuteHistoryBarLastUpdated", lastDate.toString(Qt::ISODate)); break;
    }
}

bool InstrumentDb::updateInstrumentConfiguration(const uint instrumentId, const QString& confKey, const QString& confValue)
{
    if (!openDatabase()) {
        return false;
    }

    QSqlQuery query = getBlankQuery();

    query.prepare("insert into StratTrader.InstrumentConfiguration (InstrumentId, ConfKey, ConfValue)"
                  "values(:InstrumentId,:ConfKey,:ConfValue)"
                  "on duplicate key update ConfValue = :ConfValue1" );


    query.bindValue(":InstrumentId", instrumentId);
    query.bindValue(":ConfValue", confValue);
    query.bindValue(":ConfValue1", confValue);
    query.bindValue(":ConfKey",confKey);
    bool result;

    if(!(result = query.exec())) {

        Logger::log() << QDateTime::currentDateTime() << " Instrument configuration upsert failed for instrumentid " << instrumentId << endl;
        //qDebug() << query.lastError().text() << endl;
    }

    query.finish();
    db.close();

    return result;
}

QList<InstrumentData> InstrumentDb :: getInstrumentsFromStrategyBuyList(const uint strategyId) {
    QList<InstrumentData> instruments;

    if (!openDatabase()) {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select i.InstrumentId, i.Symbol, i.ShortName, i.FullName, i.Type, i.SectorCode, i.CountryCode, i.ExchangeCode "
                 "from StrategyBuyList sbl "
                 "inner join Strategy s on sbl.StrategyId = s.StrategyId "
                 "inner join Instrument i on sbl.InstrumentId = i.InstrumentId "
                 "where s.StrategyId = :StrategyId ");
    query.bindValue(":StrategyId", strategyId);
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instruments.append(i);
    }

    query.finish();
    db.close();

    qDebug() << "Found " << instruments.count() << " instruments for strategy Id " << strategyId << " in strategy buy list" << endl;
    return instruments;
}

QList<InstrumentData> InstrumentDb :: getInstrumentsFromStrategyBuyList(const QString& strategyName) {
    QList<InstrumentData> instruments;

    if (!openDatabase()) {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select i.InstrumentId, i.Symbol, i.ShortName, i.FullName, i.Type, i.SectorCode, i.CountryCode, i.ExchangeCode "
                 "from StrategyBuyList sbl "
                 "inner join Strategy s on sbl.StrategyName = s.StrategyName "
                 "inner join Instrument i on sbl.InstrumentId = i.InstrumentId "
                 "where s.StrategyName = :StrategyName ");
    query.bindValue(":StrategyName", strategyName);
    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instruments.append(i);
    }

    query.finish();
    db.close();

    //qDebug() << "Found " << instruments.count() << " instruments for strategy Id " << strategyId << " in strategy buy list" << endl;
    return instruments;
}



QList<InstrumentData> InstrumentDb::getInstrumentsWithSimilarSymbol(const QString& symbol)
{
    QList<InstrumentData> instruments;

    if (!openDatabase()) {
        return instruments;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select InstrumentId, Symbol, ShortName, FullName, Type, SectorCode, ExchangeCode, CountryCode from Instrument "
                  "where Symbol like :Symbol order by Symbol limit 5");

    QString sym = symbol + "%";
    query.bindValue(":Symbol", sym);

    bool result = query.exec();
    if (!result) {
        qDebug() << query.executedQuery() << endl;
        qDebug() << query.lastError().text() << endl;
    }

    //qDebug() << "Got " << query.size() << " rows" << endl;
    while (query.next()) {
        InstrumentData i;// = new InstrumentData();

        i.instrumentId = query.value(DbInstrumentId).toUInt();
        i.symbol = query.value(Symbol).toString();
        i.shortName = query.value(ShortName).toString();
        i.fullName = query.value(FullName).toString();
        i.type = query.value(Type).value<quint8>();
        i.sectorCode = query.value(SectorCode).toString();
        i.exchangeCode = query.value(ExchangeCode).toString();
        i.countryCode = query.value(CountryCode).toString();

        instruments.append(i);
        qDebug() << "List has " << instruments.count() << " instruments" << endl;
    }

    query.finish();
    db.close();

    qDebug() << "List has " << instruments.count() << " instruments" << endl;
    return instruments;
}

