#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategybuylistviewdata.h"
#include "typedefs.h"

class StrategyBuyListViewDb : public DbBase
{
private:
	enum StrategyBuyListView {
		StrategyBuyListId, 
		StrategyId, 
		StrategyName, 
		InstrumentId, 
		Symbol, 
        InstrumentType
	};

public:
	StrategyBuyListViewDb(void);
	~StrategyBuyListViewDb(void);

	//Methods

	StrategyBuyListViewData* getStrategyBuyListViewById(QUuid id);
	QList<StrategyBuyListViewData*> getStrategyBuyListViews();
    QList<StrategyBuyListViewData*> getStrategyBuyListViewsForStrategy(QString strategyName);
    QList<ATContract*> getATContractsForStrategy(const QString& strategyName);
};
