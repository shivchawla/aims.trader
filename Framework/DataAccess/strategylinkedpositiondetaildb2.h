#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategylinkedpositiondetaildata2.h"

class StrategyLinkedPositionDetailDb2 : public DbBase
{
private:
	enum StrategyLinkedPositionDetail {
		StrategyLinkedPositionDetailId, 
		SharesBought, 
		SharesSold, 
		AvgBought, 
		AvgSold, 
		Commission, 
		CreatedDateTime, 
		StrategyLinkedPositionId 
	};

public:
    StrategyLinkedPositionDetailDb2(void);
    ~StrategyLinkedPositionDetailDb2(void);

	//Methods

    StrategyLinkedPositionDetailData2* getStrategyLinkedPositionDetailById(const uint &id);
    QList<StrategyLinkedPositionDetailData2*> getStrategyLinkedPositionDetails(const uint &strategyLinkedPositionId);
    uint insertStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData2* data);
    uint insertStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint updateStrategyLinkedPositionDetail(const uint runId, const uint positionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint insertStrategyLinkedPositionDetails(const QList<StrategyLinkedPositionDetailData2*> &list);
//	uint updateStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData* data);
//	uint deleteStrategyLinkedPositionDetail(uint id);

};
