#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategydata.h"

class StrategyDb : public DbBase
{
private:
	enum Strategy {
		StrategyId, 
		Name, 
		Since, 
		UsedInTrading, 
        ParentStrategyId,
        ParentStrategyName
	};

public:
	StrategyDb(void);
	~StrategyDb(void);

	//Methods

    StrategyData* getStrategyById(const uint &id);
    QList<StrategyData*> getStrategies();
    StrategyData* getStrategyViewByName(const QString &strategyName);
    unsigned int insertStrategy(const StrategyData* &data);
    unsigned int insertStrategy(QString name, QDateTime since, bool usedInTrading, uint parentStrategyId);
    unsigned int updateStrategy(const StrategyData* &data);
    unsigned int deleteStrategy(const uint &id);

};
