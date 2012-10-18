#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategylinkedpositiondetaildata.h"

class StrategyLinkedPositionDetailDb : public DbBase
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
	StrategyLinkedPositionDetailDb(void);
	~StrategyLinkedPositionDetailDb(void);

	//Methods

    StrategyLinkedPositionDetailData* getStrategyLinkedPositionDetailById(const uint &id);
    QList<StrategyLinkedPositionDetailData*> getStrategyLinkedPositionDetails(const uint &strategyLinkedPositionId);
    uint insertStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData* data);
    uint insertStrategyLinkedPositionDetail(const uint runId, const uint strategyId, const uint instrumentId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint updateStrategyLinkedPositionDetail(const uint runId, const uint strategyId, const uint instrumentId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint insertStrategyLinkedPositionDetails(const QList<StrategyLinkedPositionDetailData*> &list);
//	uint updateStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData* data);
//	uint deleteStrategyLinkedPositionDetail(uint id);

};
