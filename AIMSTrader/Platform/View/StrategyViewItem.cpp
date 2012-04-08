#include "Platform/View/StrategyViewItem.h"

void StrategyViewItem::updateStrategyID(const StrategyId strategyId)
{
    _items[StrategyID]->updateItem(strategyId);
}

void StrategyViewItem::updateStrategyName(const QString& name)
{
    _items[StrategyName]->updateItem(name);
}

void StrategyViewItem::updateTotalTrades(const int value)
{
    _items[TotalTrades]->updateItem(value);
}

void StrategyViewItem::updateWinningTrades(const int value)
{
    _items[WinningTrades]->updateItem(value);
}

void StrategyViewItem::updateLosingTrades(const int value)
{
    _items[LosingTrades]->updateItem(value);
}

void StrategyViewItem::updateProfitFactor(const double value)
{
    _items[ProfitFactor]->updateItem(value);
}

void StrategyViewItem::updateNetProfit(const double value)
{
    _items[NetProfit]->updateItem(value);
}

void StrategyViewItem::updateInvestment(const double value)
{
    _items[Investment]->updateItem(value);
}

void StrategyViewItem::updateTotalBought(const double value)
{
    _items[TotalBought]->updateItem(value);
}

void StrategyViewItem::updateTotalSold(const double value)
{
    _items[TotalSold]->updateItem(value);
}

StrategyViewItem::~StrategyViewItem()
{
    for(int i=0;i<_numItems;++i)
    {
        delete _items[i];
    }
}

StrategyViewItem::StrategyViewItem()
{
    for(int i=0;i<_numItems;++i)
    {
        _items[i] = new TableItem();
    }
}

TableItem* StrategyViewItem::getTableItem(const int col)
{
    return _items[col];
}

const String StrategyViewItem::getHeaderName(const StrategyModelColumn col)
{
    switch(col)
    {
        case StrategyID: return "StrategyID"; break;
        case StrategyName: return "StrategyName";break;
        case TotalTrades: return "TotalTrades";break;
        case WinningTrades: return "WinningTrades";break;
        case LosingTrades: return "LosingTrades";break;
        case ProfitFactor: return "ProfitFactor";break;
        case NetProfit: return "NetProfit";break;
        case TotalBought: return "TotalBought";break;
        case TotalSold: return "TotalSold";break;
        case Investment: return "Investment";break;
        default: break;
    }
}





