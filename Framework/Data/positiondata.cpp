#include "PositionData.h"
#include <QDebug>

PositionData :: PositionData(void)
{}

PositionData :: PositionData(const PositionData* copy)
{
    positionId = copy->positionId;
    sharesBought = copy->sharesBought;
    sharesSold = copy->sharesSold;
    avgBuyPrice = copy->avgBuyPrice;
    avgSellPrice = copy->avgSellPrice;
	totalAmountCommission = copy->totalAmountCommission;
    createdDate = copy->createdDate;
	updatedDate = copy->updatedDate;
    //strategyId = copy->strategyId;
    //instrumentId = copy->instrumentId;
}

PositionData :: ~PositionData(void)
{}

void PositionData :: printDebug() {
    qDebug() << positionId << " " << sharesBought << " " << sharesSold << " " << avgBuyPrice << " "
            << avgSellPrice << " " << totalAmountCommission << " ";
            //<< createdDate << " " << updatedDate << " " << strategyId << " " << instrumentId << endl;
}
