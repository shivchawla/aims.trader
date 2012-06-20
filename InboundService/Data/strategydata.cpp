#include "strategydata.h"
#include <QDebug>

StrategyData :: StrategyData(void)
{
}

StrategyData :: StrategyData(StrategyData* copy)
{
	strategyId = copy->strategyId;
	name = copy->name;
	since = copy->since;
	usedInTrading = copy->usedInTrading;
	parentStrategyId = copy->parentStrategyId;
}
StrategyData :: ~StrategyData(void)
{
}

void StrategyData :: printDebug() {
    qDebug() << strategyId << " " << name << " " << since << " " << usedInTrading << " " << parentStrategyId << endl;
}

