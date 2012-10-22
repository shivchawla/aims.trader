#include "StrategyLinkedPositionData2.h"
#include <QDebug>

StrategyLinkedPositionData2 :: StrategyLinkedPositionData2(void)
{}

StrategyLinkedPositionData2 :: StrategyLinkedPositionData2(const StrategyLinkedPositionData2* copy)
{
    strategyLinkedPositionId = copy->strategyLinkedPositionId;
	numberBought = copy->numberBought;
	numberSold = copy->numberSold;
	avgAmountBought = copy->avgAmountBought;
	avgAmountSold = copy->avgAmountSold;
	totalAmountCommission = copy->totalAmountCommission;
    createdDate = copy->createdDate;
	updatedDate = copy->updatedDate;
    //strategyId = copy->strategyId;
    //instrumentId = copy->instrumentId;
}

StrategyLinkedPositionData2 :: ~StrategyLinkedPositionData2(void)
{}

void StrategyLinkedPositionData2 :: printDebug() {
    qDebug() << strategyLinkedPositionId << " " << numberBought << " " << numberSold << " " << avgAmountBought << " "
            << avgAmountSold << " " << totalAmountCommission << " ";
            //<< createdDate << " " << updatedDate << " " << strategyId << " " << instrumentId << endl;
}
