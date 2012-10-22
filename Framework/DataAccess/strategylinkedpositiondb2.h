#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategylinkedpositiondata2.h"

class StrategyLinkedPositionDb2 : public DbBase
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
    StrategyLinkedPositionDb2(void);
    ~StrategyLinkedPositionDb2(void);

	//Methods

    StrategyLinkedPositionData2* getStrategyLinkedPositionById(const uint &id);
    QList<StrategyLinkedPositionData2*> getStrategyLinkedPositions();
    QList<StrategyLinkedPositionData2*> getOpenStrategyLinkedPositions(uint strategyId);
    QList<StrategyLinkedPositionData2*> getPositionsForStrategy(const uint &strategyId);

    //returns the newly inserted primary key
    uint insertStrategyLinkedPosition(const uint runId, const uint positionId, const uint numberBought, const uint numberSold,
                                      const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& createdDate) ;

    //uint insertStrategyLinkedPosition(const StrategyLinkedPositionData* &data);

    uint updateStrategyLinkedPosition(const uint runId, const uint positionId,
                                      const uint numberBought, const uint numberSold, const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& updatedDate);

    //uint updateStrategyLinkedPosition(const StrategyLinkedPositionData* &data);
    uint deleteStrategyLinkedPosition(const uint &id);

};
