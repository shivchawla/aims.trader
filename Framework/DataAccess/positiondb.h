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
        RunId,
        PositionId,
        NumberBought,
        NumberSold,
        AvgBuyPrice,
        AvgSellPrice,
        Commission,
        CreatedDate,
        UpdatedDate
	};

public:
    PositionDb(void);

	//Methods
    PositionData getStrategyLinkedPositionById(const uint &id);
    QList<PositionData> getStrategyLinkedPositions(const uint runId);
    QList<PositionData> getOpenStrategyLinkedPositions(const uint runId, const uint strategyId);
    QList<PositionData> getOpenStrategyLinkedPositions(const uint runId, const QString& strategyName);
    QList<PositionData> getPositionsForStrategy(const uint runId, const uint strategyId);

    //returns the newly inserted primary key
    uint insertStrategyLinkedPosition(const uint runId, const uint positionId, const uint numberBought, const uint numberSold,
                                      const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& createdDate) ;

    uint updateStrategyLinkedPosition(const uint runId, const uint positionId,
                                      const uint numberBought, const uint numberSold, const double avgAmountBought, const double avgAmountSold,
                                      const double totalAmountCommission, const QDateTime& updatedDate);

    uint deleteStrategyLinkedPosition(const uint &id);

};
