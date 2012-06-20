#ifndef STRATEGYLINKEDPOSITIONDATA_H
#define STRATEGYLINKEDPOSITIONDATA_H
#include<QUuid>
#include<QDateTime>

class StrategyLinkedPositionData //: public QObject
{
    //Q_OBJECT
public:
	//member variables
    QUuid strategyLinkedPositionId;
	unsigned int numberBought;
	unsigned int numberSold;
	float avgAmountBought;
	float avgAmountSold;
    float totalAmountBought;
    float totalAmountSold;
    float totalAmountCommission;
    float realizedPnl;
	QDateTime updatedDate;
	QUuid strategyId;
	QUuid instrumentId;

	//ctor
	StrategyLinkedPositionData(void);
	StrategyLinkedPositionData(StrategyLinkedPositionData* copy);
	~StrategyLinkedPositionData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYLINKEDPOSITIONDATA_H
