#ifndef STRATEGYLINKEDPOSITIONDETAILDATA_H
#define STRATEGYLINKEDPOSITIONDETAILDATA_H
#include<QDateTime>

class StrategyLinkedPositionDetailData : public QObject
{
	Q_OBJECT
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

	//ctor
	StrategyLinkedPositionDetailData(void);
	StrategyLinkedPositionDetailData(StrategyLinkedPositionDetailData* copy);
	~StrategyLinkedPositionDetailData(void);

	//methods
	void printDebug();

};

#endif //STRATEGYLINKEDPOSITIONDETAILDATA_H
