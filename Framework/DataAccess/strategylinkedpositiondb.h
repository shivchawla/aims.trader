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
    QList<StrategyLinkedPositionData*> getOpenStrategyLinkedPositions(uint strategyId);
    QList<StrategyLinkedPositionData*> getPositionsForStrategy(const uint &strategyId);

    //returns the newly inserted primary key
    uint insertStrategyLinkedPosition(const uint runId, const uint strategyId, const uint instrumentId, const uint numberBought, const uint numberSold,
                                      const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& createdDate) ;

    //uint insertStrategyLinkedPosition(const StrategyLinkedPositionData* &data);

    uint updateStrategyLinkedPosition(const uint runId, const uint strategyId,const uint instrumentId,
                                      const uint numberBought, const uint numberSold, const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& updatedDate);

    //uint updateStrategyLinkedPosition(const StrategyLinkedPositionData* &data);
    uint deleteStrategyLinkedPosition(const uint &id);

};
