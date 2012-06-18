#include "Platform/View/StrategyViewItem.h"


StrategyViewItem::StrategyViewItem(const int numItems):TableViewItem<StrategyViewItem>(numItems)
{
      _strategyId = -1;
}

void StrategyViewItem::setStrategyId(const StrategyId strategyId)
{
    _strategyId = strategyId;
}


//void StrategyViewItem::updateSpecial(const double value, int itemColumn)
//{
//    if(itemColumn!=-1)
//    {
//        double oldValue = _cells[itemColumn]->text().toDouble();
//        _cells[itemColumn]->updateItem(value);
//        if(oldValue < 0 && value > 0)
//        {
//            _cells[itemColumn]->setForeground(Qt::green);
//        }
//        else if(oldValue > 0 && value < 0)
//        {
//             _cells[itemColumn]->setForeground(Qt::red);
//        }
//        else
//        {
//            _cells[itemColumn]->setForeground(Qt::white);
//        }
//    }
//}

