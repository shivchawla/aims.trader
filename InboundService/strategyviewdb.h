#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "strategyviewdata.h"

class StrategyViewDb : public DbBase
{
private:
	enum StrategyView {
		StrategyId, 
		Name, 
		ParentStrategyId, 
		ParentName, 
		Since, 
        UsedInTrading
	};

public:
	StrategyViewDb(void);
	~StrategyViewDb(void);

	//Methods

    StrategyViewData* getStrategyViewById(QUuid id);
	QList<StrategyViewData*> getStrategyViews();
};
