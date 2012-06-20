#include "strategybuylistdata.h"
#include <QDebug>

StrategyBuyListData :: StrategyBuyListData(void)
{
}

StrategyBuyListData :: StrategyBuyListData(StrategyBuyListData* copy)
{
	strategyBuyListId = copy->strategyBuyListId;
	strategyId = copy->strategyId;
	instrumentId = copy->instrumentId;
}
StrategyBuyListData :: ~StrategyBuyListData(void)
{
}

void StrategyBuyListData :: printDebug() {
    qDebug() << strategyBuyListId << " " << strategyId << " " << instrumentId << endl;
}
