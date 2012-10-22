#ifndef STRATEGYLINKEDPOSITIONDETAILDATA_H
#define STRATEGYLINKEDPOSITIONDETAILDATA_H
#include<QDateTime>

class StrategyLinkedPositionDetailData2
{

public:
	//member variables
    uint strategyLinkedPositionDetailId;
    uint sharesBought;
    uint sharesSold;
	float avgBought;
	float avgSold;
	float commission;
	QDateTime createdDateTime;
    uint strategyLinkedPositionId;
    uint runId;

	//ctor
    StrategyLinkedPositionDetailData2(void);
    StrategyLinkedPositionDetailData2(StrategyLinkedPositionDetailData2* copy);
    ~StrategyLinkedPositionDetailData2(void);

	//methods
	void printDebug();

};

#endif //STRATEGYLINKEDPOSITIONDETAILDATA_H
