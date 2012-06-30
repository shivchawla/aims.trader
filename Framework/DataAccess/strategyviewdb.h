#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategyviewdata.h"

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
    StrategyViewData* getStrategyViewByName(QString name);
	QList<StrategyViewData*> getStrategyViews();
};
