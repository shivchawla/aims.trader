#include "strategylinkedpositiondata.h"
#include <QDebug>

StrategyLinkedPositionData :: StrategyLinkedPositionData(void)
{
}

StrategyLinkedPositionData :: StrategyLinkedPositionData(StrategyLinkedPositionData* copy)
{
    strategyLinkedPositionId = copy->strategyLinkedPositionId;
	numberBought = copy->numberBought;
	numberSold = copy->numberSold;
	avgAmountBought = copy->avgAmountBought;
	avgAmountSold = copy->avgAmountSold;
	totalAmountBought = copy->totalAmountBought;
	totalAmountSold = copy->totalAmountSold;
	totalAmountCommission = copy->totalAmountCommission;
	realizedPnl = copy->realizedPnl;
	updatedDate = copy->updatedDate;
	strategyId = copy->strategyId;
	instrumentId = copy->instrumentId;
}
StrategyLinkedPositionData :: ~StrategyLinkedPositionData(void)
{
}

void StrategyLinkedPositionData :: printDebug() {
    qDebug() << strategyLinkedPositionId << " " << numberBought << " " << numberSold << " " << avgAmountBought << " "
            << avgAmountSold << " " << totalAmountBought << " " << totalAmountSold << " " <<  totalAmountCommission << " "
            << realizedPnl << " " << updatedDate << " " << strategyId << " " << instrumentId << endl;
}
