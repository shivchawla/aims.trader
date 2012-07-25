#ifndef STRATEGYLINKEDPOSITIONDATA_H
#define STRATEGYLINKEDPOSITIONDATA_H
#include<QDateTime>

class StrategyLinkedPositionData //: public QObject
{
    //Q_OBJECT
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
    uint strategyId;
    uint instrumentId;

	//ctor
	StrategyLinkedPositionData(void);
    StrategyLinkedPositionData(const StrategyLinkedPositionData* copy);
	~StrategyLinkedPositionData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYLINKEDPOSITIONDATA_H
