#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "exchangedata.h"

class ExchangeDb : public DbBase
{
private:
	enum Exchange {
		ExchangeId, 
        Name
	};

public:
	ExchangeDb(void);
	~ExchangeDb(void);

	//Methods

	ExchangeData* getExchangeById(QUuid id);
	QList<ExchangeData*> getExchanges();
	unsigned int insertExchange(const ExchangeData* data);
    unsigned int insertExchange(QString name);
    unsigned int updateExchange(const ExchangeData* data, QUuid id);
	unsigned int deleteExchange(QUuid id);

};
