#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/positiondetaildata.h"

class PositionDetailDb : public DbBase
{
private:
    enum PositionDetail {
        PositionDetailId,
		SharesBought, 
		SharesSold, 
		AvgBought, 
		AvgSold, 
		Commission, 
		CreatedDateTime, 
        PositionId
	};

public:
    PositionDetailDb(void);
    ~PositionDetailDb(void);

	//Methods

    PositionDetailData getStrategyLinkedPositionDetailById(const uint &id);
    QList<PositionDetailData> getStrategyLinkedPositionDetails(const uint &strategyLinkedPositionId);
    uint insertStrategyLinkedPositionDetail(const PositionDetailData data);
    uint insertStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint updateStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint insertStrategyLinkedPositionDetails(const QList<PositionDetailData> &list);
};
