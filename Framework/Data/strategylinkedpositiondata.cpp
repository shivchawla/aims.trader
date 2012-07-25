#include "strategylinkedpositiondata.h"
#include <QDebug>

StrategyLinkedPositionData :: StrategyLinkedPositionData(void)
{
}

StrategyLinkedPositionData :: StrategyLinkedPositionData(const StrategyLinkedPositionData* copy)
{
    strategyLinkedPositionId = copy->strategyLinkedPositionId;
	numberBought = copy->numberBought;
	numberSold = copy->numberSold;
	avgAmountBought = copy->avgAmountBought;
	avgAmountSold = copy->avgAmountSold;
	totalAmountCommission = copy->totalAmountCommission;
    createdDate = copy->createdDate;
	updatedDate = copy->updatedDate;
	strategyId = copy->strategyId;
	instrumentId = copy->instrumentId;
}

StrategyLinkedPositionData :: ~StrategyLinkedPositionData(void)
{
}

void StrategyLinkedPositionData :: printDebug() {
    qDebug() << strategyLinkedPositionId << " " << numberBought << " " << numberSold << " " << avgAmountBought << " "
            << avgAmountSold << " " << totalAmountCommission << " "
            << createdDate << " " << updatedDate << " " << strategyId << " " << instrumentId << endl;
}
