#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "strategylinkedpositiondata.h"

class StrategyLinkedPositionDb : public DbBase
{
private:
	enum StrategyLinkedPosition {
        StrategyLinkedPositionId,
		NumberBought, 
		NumberSold, 
		AvgAmountBought, 
		AvgAmountSold, 
		TotalAmountBought, 
		TotalAmountSold, 
		TotalAmountCommission, 
		RealizedPnl, 
		UpdatedDate, 
		StrategyId, 
        InstrumentId
	};

public:
	StrategyLinkedPositionDb(void);
	~StrategyLinkedPositionDb(void);

	//Methods

	StrategyLinkedPositionData* getStrategyLinkedPositionById(QUuid id);
	QList<StrategyLinkedPositionData*> getStrategyLinkedPositions();
    unsigned int insertStrategyLinkedPosition(QUuid id, unsigned int numberBought, unsigned int numberSold,
                            float avgAmountBought, float avgAmountSold, float totalAmountBought, float totalAmountSold,
                            float totalAmountCommission, float realizedPnl, QDateTime updatedDate, QUuid strategyId,
                            QUuid instrumentId);
	unsigned int insertStrategyLinkedPosition(const StrategyLinkedPositionData* data);
    unsigned int updateStrategyLinkedPosition(const StrategyLinkedPositionData* data, QUuid id);
	unsigned int deleteStrategyLinkedPosition(QUuid id);

};
