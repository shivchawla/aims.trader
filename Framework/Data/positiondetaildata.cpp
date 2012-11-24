#include <QDebug>
#include "PositionDetailData.h"

PositionDetailData::PositionDetailData(void)
{}

PositionDetailData::PositionDetailData(PositionDetailData* copy)
{
    positionDetailId = copy->positionDetailId;
	sharesBought = copy->sharesBought;
	sharesSold = copy->sharesSold;
    avgBuyPrice = copy->avgBuyPrice;
    avgSellPrice = copy->avgSellPrice;
	commission = copy->commission;
	createdDateTime = copy->createdDateTime;
    positionId = copy->positionId;
    runId = copy->runId;
}

PositionDetailData::~PositionDetailData(void)
{
}

void PositionDetailData :: printDebug() {
    qDebug() << positionDetailId << " " << sharesBought << " " << sharesSold << " " << avgBuyPrice << " " << avgSellPrice
             << " " << commission << " " << createdDateTime << " " << positionId << " "  << endl;
}
