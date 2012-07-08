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
        UpdatedBy,
        UpdatedDate,
        ExchangeId,
        CountryId
	};

	//QSqlDatabase db;
public:
	InstrumentDb(void);
	~InstrumentDb(void);

	//METHODS
    InstrumentData* getInstrumentBySymbol(QString symbol);
    QList<InstrumentData*> getInstruments();
    unsigned int insertInstrument(const InstrumentData* data);
    unsigned int insertInstrument(QUuid instrumentId,QString symbol,QString shortName,QString fullName,QChar type, QString updatedBy, QDateTime updatedDate, QUuid exchangeId, QUuid countryId);
    unsigned int insertInstruments(const QList<InstrumentData*> list);
    QDateTime getLastHistoryUpdateDate(const QUuid instrumentId);
    void updateDailyHistoryBarDate(const QUuid instrumentId, const QDateTime lastDate);
    QHash<QUuid, QDateTime> getLastHistoryUpdateDateForAllInstruments();

};

