#include <QDebug>
#include "StrategyLinkedPositionDetailData2.h"

StrategyLinkedPositionDetailData2 :: StrategyLinkedPositionDetailData2(void)
{
}

StrategyLinkedPositionDetailData2 :: StrategyLinkedPositionDetailData2(StrategyLinkedPositionDetailData2* copy)
{
	strategyLinkedPositionDetailId = copy->strategyLinkedPositionDetailId;
	sharesBought = copy->sharesBought;
	sharesSold = copy->sharesSold;
	avgBought = copy->avgBought;
	avgSold = copy->avgSold;
	commission = copy->commission;
	createdDateTime = copy->createdDateTime;
	strategyLinkedPositionId = copy->strategyLinkedPositionId;
    runId = copy->runId;
}

StrategyLinkedPositionDetailData2 :: ~StrategyLinkedPositionDetailData2(void)
{
}

void StrategyLinkedPositionDetailData2 :: printDebug() {
    qDebug() << strategyLinkedPositionDetailId << " " << sharesBought << " " << sharesSold << " " << avgBought << " " << avgSold
             << " " << commission << " " << createdDateTime << " " << strategyLinkedPositionId << " "  << endl;
}
