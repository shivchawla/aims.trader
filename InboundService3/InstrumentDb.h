#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "InstrumentData.h"
#include "DbBase.h"

class InstrumentDb : public DbBase
{
private:
	enum Instrument {
	INSTRUMENT_ID,
	SYMBOL,
	SHORT_NAME,
	FULL_NAME,
	UPDATED_BY,
	UPDATED_DATE
	};

	//QSqlDatabase db;
public:
	InstrumentDb(void);
	~InstrumentDb(void);

	//METHODS
	InstrumentData* GetInstrumentBySymbol(QString symbol);
    QList<InstrumentData*> GetInstruments();
};

