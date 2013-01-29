#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "Data/InstrumentData.h"
#include "DbBase.h"

class InstrumentDb : public DbBase
{
private:
	enum Instrument {
        DbInstrumentId,
        Symbol,
        ShortName,
        FullName,
        Type,
        SectorCode,
        ExchangeCode,
        CountryCode
	};

private:
    bool updateInstrumentConfiguration(const uint instrumentId, const QString& confKey, const QString& confValue);

public:
    InstrumentDb();
    ~InstrumentDb();

	//METHODS
    InstrumentData getInstrumentBySymbol(QString symbol, uchar type);
    QList<InstrumentData> getInstruments();
    QList<InstrumentData> getInstruments(const QList<InstrumentId>&);
    InstrumentData getInstrument(const InstrumentId);

    uint insertInstrument(const InstrumentData& data);
    uint insertInstrument(QString symbol,QString shortName,QString fullName,quint8 type,QString sectorCode, QString exchangeCode, QString countryCode);
    uint insertInstruments(const QList<InstrumentData>& list);
    QDateTime getNextHistoryUpdateDate(const uint &instrumentId);
    bool updateDailyHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate);
    QHash<uint, QDateTime> getLastDailyHistoryUpdateDateForAllInstruments();
    QHash<uint, QDateTime> getLastIntradayHistoryUpdateDateForAllInstruments1(const int frequency = 1);
    bool updateIntradayHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate, const int frequency = 1);
    bool updateIntradayHistoryBarDate(const QHash<uint, QDateTime>&, const int frequency = 1);
    bool updateDailyHistoryBarDate(const QHash<uint, QDateTime>&);
    QHash<uint, QHash<uint, QDateTime> > getLastIntradayHistoryUpdateDateForAllInstruments();

    QList<InstrumentData> getInstrumentsFromStrategyBuyList(const uint strategyId);

    QList<InstrumentData> getInstrumentsWithSimilarSymbol(const QString&);

};

