#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategybuylistdata.h"

class ATContract;

class StrategyBuyListDb : public DbBase
{
private:
	enum StrategyBuyList {
		StrategyBuyListId, 
		StrategyId, 
        InstrumentId,
        DeactivatedDate,
        StrategyName,
        Symbol,
        InstrumentType
	};

public:
	StrategyBuyListDb(void);
	~StrategyBuyListDb(void);

	//Methods

    StrategyBuyListData* getStrategyBuyListById(const uint &id);
	QList<StrategyBuyListData*> getStrategyBuyLists();
    QList<StrategyBuyListData*> getStrategyBuyListsForStrategy(const QString& strategyName);
    uint insertStrategyBuyList(const StrategyBuyListData* &data);
    uint insertStrategyBuyList(uint strategyId, uint instrumentId, QDateTime deactivatedDate);
    uint updateStrategyBuyList(const StrategyBuyListData* &data);
    uint deleteStrategyBuyList(const uint &id);

    QList<ATContract*> getATContractsForStrategy(const QString &strategyName);
    std::string getSecurityTypeForVendor(const quint8 &instrumentType, int vendorCode);

};
