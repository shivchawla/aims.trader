#include "strategyviewdata.h"
#include <QDebug>

StrategyViewData :: StrategyViewData(void)
{
}

StrategyViewData :: StrategyViewData(StrategyViewData* copy)
{
	strategyId = copy->strategyId;
	name = copy->name;
	parentStrategyId = copy->parentStrategyId;
	parentName = copy->parentName;
	since = copy->since;
	usedInTrading = copy->usedInTrading;
}

StrategyViewData :: ~StrategyViewData(void)
{
}

void StrategyViewData :: printDebug()
{
    qDebug() << strategyId << " " << name << " " << parentStrategyId << " " << parentName << " "
             << since << " " << usedInTrading << endl;
}
