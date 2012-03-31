#include "Platform/View/StrategyViewItem.h"

void StrategyViewItem::updateStrategyID(const StrategyId strategyId)
{
    _items[StrategyID].updateItem(strategyId);
}

void StrategyViewItem::updateStrategyName(std::string& name)
{
    _items[StrategyName].updateItem(name);
}

void StrategyViewItem::updateTotalTrades(const int value)
{
    _items[TotalTrades].updateItem(value);
}

void StrategyViewItem::updateWinningTrades(const int value)
{
    _items[WinningTrades].updateItem(value);
}

void StrategyViewItem::updateLosingTrades(const int value)
{
    _items[LosingTrades].updateItem(value);
}

void StrategyViewItem::updateProfitFactor(const double value)
{
    _items[ProfitFactor].updateItem(value);
}

void StrategyViewItem::updateNetProfit(const double value)
{
    _items[NetProfit].updateItem(value);
}

void StrategyViewItem::updateInvestment(const double value)
{
    _items[Investment].updateItem(value);
}

StrategyViewItem::~StrategyViewItem()
{}




