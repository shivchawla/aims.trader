#include "Platform/View/StrategyPositionViewItem.h"
#include "Platform/View/TableCellItem.h"
#include <QAction>

StrategyPositionViewItem::StrategyPositionViewItem(const int numItems):TableViewItem<StrategyPositionViewItem>(numItems)
{
    _strategyId = -1;
    _tickerId = -1;
    _isActive = false;
}

StrategyPositionViewItem::~StrategyPositionViewItem()
{}

void StrategyPositionViewItem::setStrategyId(const StrategyId strategyId)
{
    _strategyId = strategyId;
}

void StrategyPositionViewItem::setTickerId(const TickerId tickerId)
{
    _tickerId = tickerId;
}

void StrategyPositionViewItem::setActive(const bool isActive)
{
    _isActive = isActive;
}

bool StrategyPositionViewItem::IsActive() const
{
    return _isActive;
}

//void StrategyPositionViewItem::contextMenuEvent(QContextMenuEvent *event)
//{

//}





