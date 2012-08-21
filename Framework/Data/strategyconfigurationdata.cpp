#include <QDebug>
#include "strategyconfigurationdata.h"

StrategyConfigurationData :: StrategyConfigurationData(void)
{
}

StrategyConfigurationData :: StrategyConfigurationData(StrategyConfigurationData* copy)
{
	strategyId = copy->strategyId;
	confKey = copy->confKey;
	confValue = copy->confValue;
}
StrategyConfigurationData :: ~StrategyConfigurationData(void)
{
}

void StrategyConfigurationData :: printDebug() {
	qDebug() << strategyId << " " << confKey << " " << confValue << " "  << endl;
}
