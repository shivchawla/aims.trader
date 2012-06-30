#ifndef STRATEGYBUYLISTVIEWDATA_H
#define STRATEGYBUYLISTVIEWDATA_H
#include<QUuid>
#include<QString>

class StrategyBuyListViewData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid strategyBuyListId;
	QUuid strategyId;
	QString strategyName;
	QUuid instrumentId;
	QString symbol;
	QString instrumentType;

	//ctor
	StrategyBuyListViewData(void);
	StrategyBuyListViewData(StrategyBuyListViewData* copy);
	~StrategyBuyListViewData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYBUYLISTVIEWDATA_H
