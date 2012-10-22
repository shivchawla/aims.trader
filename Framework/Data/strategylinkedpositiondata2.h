#ifndef STRATEGYLINKEDPOSITIONDATA_H
#define STRATEGYLINKEDPOSITIONDATA_H
#include<QDateTime>

class StrategyLinkedPositionData2
{
public:
	//member variables
    uint strategyLinkedPositionId;
    uint numberBought;
    uint numberSold;
	float avgAmountBought;
	float avgAmountSold;
    float totalAmountCommission;
    QDateTime createdDate;
    QDateTime updatedDate;
    uint positionId;
    uint runId;

	//ctor
    StrategyLinkedPositionData2(void);
    StrategyLinkedPositionData2(const StrategyLinkedPositionData2* copy);
    ~StrategyLinkedPositionData2(void);

	//methods
    void printDebug();
};

#endif //STRATEGYLINKEDPOSITIONDATA_H
