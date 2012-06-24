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
        UpdatedDate
	};

	//QSqlDatabase db;
public:
	InstrumentDb(void);
	~InstrumentDb(void);

	//METHODS
    InstrumentData* getInstrumentBySymbol(QString symbol);
    QList<InstrumentData*> getInstruments();
};

