#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/exchangedata.h"

class ExchangeDb : public DbBase
{
private:
    enum Exchange
    {
        Code,
        Name
	};

public:
    ExchangeDb();

	//Methods
    ExchangeData getExchangeByCode(const QString &code);
    QList<ExchangeData> getExchanges();
    uint insertExchange(const ExchangeData& data);
    uint insertExchange(const QString &code, const QString &name);
    uint updateExchange(const ExchangeData& data);
    uint deleteExchange(const QString &code);

};
