#include "strategybuylistviewdata.h"
#include <QDebug>

StrategyBuyListViewData :: StrategyBuyListViewData(void)
{
}

StrategyBuyListViewData :: StrategyBuyListViewData(StrategyBuyListViewData* copy)
{
	strategyBuyListId = copy->strategyBuyListId;
	strategyId = copy->strategyId;
	strategyName = copy->strategyName;
	instrumentId = copy->instrumentId;
	symbol = copy->symbol;
	instrumentType = copy->instrumentType;
}
StrategyBuyListViewData :: ~StrategyBuyListViewData(void)
{
}

void StrategyBuyListViewData :: printDebug()
{
    qDebug() << strategyBuyListId << " " << strategyId << " " << strategyName << " "
             << instrumentId << " " << symbol << " " << instrumentType << endl;
}
