#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "strategydata.h"

class StrategyDb : public DbBase
{
private:
	enum Strategy {
		StrategyId, 
		Name, 
		Since, 
		UsedInTrading, 
        ParentStrategyId
	};

public:
	StrategyDb(void);
	~StrategyDb(void);

	//Methods

	StrategyData* getStrategyById(QUuid id);
    QList<StrategyData*> getStrategies();
	unsigned int insertStrategy(const StrategyData* data);
    unsigned int insertStrategy(QUuid id, QString name, QDateTime since, bool usedInTrading, QUuid parentStrategyId);
    unsigned int updateStrategy(const StrategyData* data, QUuid id);
	unsigned int deleteStrategy(QUuid id);

};
