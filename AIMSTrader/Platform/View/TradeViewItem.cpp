#include "Platform/View/TradeViewItem.h"

TradeViewItem::TradeViewItem(const int numItems):TableViewItem<TradeViewItem>(numItems)
{
    _tickerId = 0;
    _strategyId = 0;
    _isActive = false;
}

