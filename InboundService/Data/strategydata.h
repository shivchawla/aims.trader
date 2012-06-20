#ifndef STRATEGYDATA_H
#define STRATEGYDATA_H
#include<QUuid>
#include<QString>
#include<QDateTime>

class StrategyData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid strategyId;
	QString name;
	QDateTime since;
    bool usedInTrading;
	QUuid parentStrategyId;

	//ctor
	StrategyData(void);
	StrategyData(StrategyData* copy);
	~StrategyData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYDATA_H
