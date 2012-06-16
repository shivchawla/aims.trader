#ifndef STRATEGYVIEWDATA_H
#define STRATEGYVIEWDATA_H
#include<QUuid>
#include<QString>
#include<QDateTime>

class StrategyViewData //: public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid strategyId;
	QString name;
	QUuid parentStrategyId;
	QString parentName;
	QDateTime since;
    bool usedInTrading;

	//ctor
	StrategyViewData(void);
	StrategyViewData(StrategyViewData* copy);
	~StrategyViewData(void);

	//methods
    void printDebug();
};

#endif //STRATEGYVIEWDATA_H
