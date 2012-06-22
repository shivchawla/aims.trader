#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "Data/strategybuylistdata.h"

class StrategyBuyListDb : public DbBase
{
private:
	enum StrategyBuyList {
		StrategyBuyListId, 
		StrategyId, 
        InstrumentId
	};

public:
	StrategyBuyListDb(void);
	~StrategyBuyListDb(void);

	//Methods

	StrategyBuyListData* getStrategyBuyListById(QUuid id);
	QList<StrategyBuyListData*> getStrategyBuyLists();
    QList<StrategyBuyListData*> getStrategyBuyListsForStrategy(const QString& strategyName);
	unsigned int insertStrategyBuyList(const StrategyBuyListData* data);
    unsigned int insertStrategyBuyList(QUuid id, QUuid strategyId, QUuid instrumentId);
    unsigned int updateStrategyBuyList(const StrategyBuyListData* data, QUuid id);
	unsigned int deleteStrategyBuyList(QUuid id);

};