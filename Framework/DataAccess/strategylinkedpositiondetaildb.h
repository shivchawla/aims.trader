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
    uint insertStrategyLinkedPositionDetail(uint sharesBought, uint sharesSold, float avgBought,
                                              float avgSold, float commission, QDateTime createdDateTime,
                                              uint strategyLinkedPositionId);
    uint insertStrategyLinkedPositionDetails(const QList<StrategyLinkedPositionDetailData*> &list);
//	uint updateStrategyLinkedPositionDetail(const StrategyLinkedPositionDetailData* data);
//	uint deleteStrategyLinkedPositionDetail(uint id);

};
