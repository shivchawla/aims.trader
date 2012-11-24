#include "Platform/View/StrategyViewItem2.h"

StrategyViewItem2::StrategyViewItem2(const StrategyId strategyId):TableViewItem2()
{
   _strategyId = strategyId;
}

//StrategyViewItem2::StrategyViewItem2(QWidget* parent):TableViewItem2(parent)
//{
//    _strategyId = -1;
//}

void StrategyViewItem2::setStrategyId(const StrategyId strategyId)
{
    _strategyId = strategyId;
}

