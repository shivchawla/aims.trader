#include "strategydata.h"
#include <QDebug>

StrategyData :: StrategyData(void)
{
}

StrategyData :: StrategyData(const StrategyData* copy)
{
	strategyId = copy->strategyId;
	name = copy->name;
	since = copy->since;
	usedInTrading = copy->usedInTrading;
	parentStrategyId = copy->parentStrategyId;
    parentStrategyName = copy->parentStrategyName;
}
StrategyData :: ~StrategyData(void)
{
}

void StrategyData :: printDebug() {
    qDebug() << strategyId << " " << name << " " << since << " " << usedInTrading << " " << parentStrategyId << " " << parentStrategyName << endl;
}

