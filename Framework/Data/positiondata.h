#ifndef STRATEGYLINKEDPOSITIONDATA_H
#define STRATEGYLINKEDPOSITIONDATA_H
#include<QDateTime>

class PositionData
{
public:
	//member variables
    uint positionId;
    uint sharesBought;
    uint sharesSold;
    double avgBuyPrice;
    double avgSellPrice;
    double totalAmountCommission;
    QDateTime createdDate;
    QDateTime updatedDate;
    uint runId;

	//ctor
    PositionData(void);
    PositionData(const PositionData* copy);
    ~PositionData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYLINKEDPOSITIONDATA_H
