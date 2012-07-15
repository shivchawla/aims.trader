#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategylinkedpositiondata.h"

class StrategyLinkedPositionDb : public DbBase
{
private:
	enum StrategyLinkedPosition {
        StrategyLinkedPositionId,
		NumberBought, 
		NumberSold, 
		AvgAmountBought, 
		AvgAmountSold, 
		TotalAmountCommission, 
        CreatedDate,
		UpdatedDate, 
		StrategyId, 
        InstrumentId
	};

public:
	StrategyLinkedPositionDb(void);
	~StrategyLinkedPositionDb(void);

	//Methods

    StrategyLinkedPositionData* getStrategyLinkedPositionById(const uint &id);
	QList<StrategyLinkedPositionData*> getStrategyLinkedPositions();
    QList<StrategyLinkedPositionData*> getPositionsForStrategy(const uint &strategyId);
    uint insertStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                      float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate, uint strategyId,
                                      uint instrumentId);
    uint insertStrategyLinkedPosition(const StrategyLinkedPositionData* &data);
    uint updateStrategyLinkedPosition(const StrategyLinkedPositionData* &data);
    uint deleteStrategyLinkedPosition(const uint &id);

};
