#ifndef STRATEGYBUYLISTDATA_H
#define STRATEGYBUYLISTDATA_H
#include<QUuid>

class StrategyBuyListData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid strategyBuyListId;
	QUuid strategyId;
	QUuid instrumentId;

	//ctor
	StrategyBuyListData(void);
	StrategyBuyListData(StrategyBuyListData* copy);
	~StrategyBuyListData(void);

	//methods
    void printDebug();

};

#endif //STRATEGYBUYLISTDATA_H
