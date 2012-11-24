#ifndef POSITIONDETAILDATA_H
#define POSITIONDETAILDATA_H
#include<QDateTime>

class PositionDetailData
{

public:
	//member variables
    uint positionDetailId;
    uint sharesBought;
    uint sharesSold;
    double avgBuyPrice;
    double avgSellPrice;
    double commission;
	QDateTime createdDateTime;
    uint positionId;
    uint runId;

	//ctor
    PositionDetailData(void);
    PositionDetailData(PositionDetailData* copy);
    ~PositionDetailData(void);

	//methods
	void printDebug();

};

#endif //STRATEGYLINKEDPOSITIONDETAILDATA_H
