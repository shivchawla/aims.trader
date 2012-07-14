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
        InstrumentId,
        Symbol,
        ShortName,
        FullName,
        Type,
        SectorCode,
        ExchangeCode,
        CountryCode
	};

	//QSqlDatabase db;
public:
	InstrumentDb(void);
	~InstrumentDb(void);

	//METHODS
    InstrumentData* getInstrumentBySymbol(QString symbol, uchar type);
    QList<InstrumentData*> getInstruments();
    uint insertInstrument(const InstrumentData* &data);
    uint insertInstrument(QString symbol,QString shortName,QString fullName,quint8 type,QString sectorCode, QString exchangeCode, QString countryCode);
    uint insertInstruments(const QList<InstrumentData*> &list);
    QDateTime getNextHistoryUpdateDate(const uint &instrumentId);
    void updateDailyHistoryBarDate(const uint &instrumentId, const QDateTime &lastDate);
    QHash<uint, QDateTime> getLastHistoryUpdateDateForAllInstruments();

};

