#include "strategybuylistdata.h"
#include <QDebug>

StrategyBuyListData :: StrategyBuyListData(void)
{
}

StrategyBuyListData :: StrategyBuyListData(const StrategyBuyListData* copy)
{
	strategyBuyListId = copy->strategyBuyListId;
	strategyId = copy->strategyId;
	instrumentId = copy->instrumentId;
    deactivatedDate = copy->deactivatedDate;
    strategyName = copy->strategyName;
    symbol = copy->symbol;
    instrumentType = copy->instrumentType;
}

StrategyBuyListData :: ~StrategyBuyListData(void)
{
}

void StrategyBuyListData :: printDebug() {
    qDebug() << strategyBuyListId << " " << strategyId << " " << instrumentId << " " << deactivatedDate
             << strategyName << " " << symbol << " " << instrumentType << endl;
}

