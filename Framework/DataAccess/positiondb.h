#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/positiondata.h"

class PositionDb : public DbBase
{
private:
    enum Position {
        PositionId,
        NumberBought,
        NumberSold,
        AvgAmountBought,
        AvgAmountSold,
        TotalAmountCommission,
        CreatedDate,
        UpdatedDate
	};

public:
    PositionDb(void);
    ~PositionDb(void);

	//Methods

    PositionData getStrategyLinkedPositionById(const uint &id);
    QList<PositionData> getStrategyLinkedPositions();
    QList<PositionData> getOpenStrategyLinkedPositions(uint strategyId);
    QList<PositionData> getPositionsForStrategy(const uint &strategyId);

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
