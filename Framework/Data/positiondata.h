#ifndef STRATEGYLINKEDPOSITIONDATA_H
#define STRATEGYLINKEDPOSITIONDATA_H
#include<QDateTime>

struct PositionData
{
	//member variables
    QString strategyName;
    uint instrumentId;
    uint sharesBought;
    uint sharesSold;
    double avgBuyPrice;
    double avgSellPrice;
    double commission;
    QDateTime createdDate;
    QDateTime updatedDate;
};

struct SpreadPositionData : public PositionData
{
    uint spreadId;
};


#endif //STRATEGYLINKEDPOSITIONDATA_H
