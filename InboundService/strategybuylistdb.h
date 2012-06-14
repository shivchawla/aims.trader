#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "strategybuylistdata.h"

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
	unsigned int insertStrategyBuyList(const StrategyBuyListData* data);
    unsigned int insertStrategyBuyList(QUuid id, QUuid strategyId, QUuid instrumentId);
    unsigned int updateStrategyBuyList(const StrategyBuyListData* data, QUuid id);
	unsigned int deleteStrategyBuyList(QUuid id);

};
