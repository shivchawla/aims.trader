#include <QDebug>
#include "strategylinkedpositiondetaildata.h"

StrategyLinkedPositionDetailData :: StrategyLinkedPositionDetailData(void)
{
}

StrategyLinkedPositionDetailData :: StrategyLinkedPositionDetailData(StrategyLinkedPositionDetailData* copy)
{
	strategyLinkedPositionDetailId = copy->strategyLinkedPositionDetailId;
	sharesBought = copy->sharesBought;
	sharesSold = copy->sharesSold;
	avgBought = copy->avgBought;
	avgSold = copy->avgSold;
	commission = copy->commission;
	createdDateTime = copy->createdDateTime;
	strategyLinkedPositionId = copy->strategyLinkedPositionId;
}
StrategyLinkedPositionDetailData :: ~StrategyLinkedPositionDetailData(void)
{
}

void StrategyLinkedPositionDetailData :: printDebug() {
    qDebug() << strategyLinkedPositionDetailId << " " << sharesBought << " " << sharesSold << " " << avgBought << " " << avgSold
             << " " << commission << " " << createdDateTime << " " << strategyLinkedPositionId << " "  << endl;
}
