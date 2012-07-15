#ifndef STRATEGYDATA_H
#define STRATEGYDATA_H
#include<QString>
#include<QDateTime>

class StrategyData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
    uint strategyId;
	QString name;
	QDateTime since;
    bool usedInTrading;
    uint parentStrategyId;
    QString parentStrategyName;

	//ctor
	StrategyData(void);
	StrategyData(StrategyData* copy);
	~StrategyData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYDATA_H
