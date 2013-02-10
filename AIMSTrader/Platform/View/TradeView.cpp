#include "Platform/View/TradeView.h"
#include "Platform/Startup/Service.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Position/Position.h"

TradeView::TradeView(QWidget* parent) : TableView<TradeView, TradeViewItem, TradeModel, TradeModelColumn>(parent)
{}

void TradeView::addTrade(const OrderDetail& orderDetail)
{
    TradeViewItem* newItem =  addItemInView();

//    newItem->setStrategyId(strategyId);
//    newItem->setTickerId(tickerId);

    //newItem->update(uint(strategyId), TradeModelStrategy);

    //get startegy name from strategyManager
    newItem->update(Service::service().getInstrumentManager()->getSymbol(orderDetail.tickerId), TradeModelInstrumentName);
    newItem->update(StrategyManager::strategyManager().getStrategyName(orderDetail.strategyId), TradeModelStrategyName);

    int lastFilledShares  = orderDetail.lastFilledShares;
    std::string action = orderDetail.order.action;
    int buyQuantity = action.compare("BUY") ? lastFilledShares : 0;
    int sellQuantity = action.compare("SELL") ? lastFilledShares : 0;

    double buyPrice = action.compare("BUY") ? orderDetail.lastFillPrice : 0;
    double sellPrice = action.compare("SELL") ? orderDetail.lastFillPrice : 0;

    newItem->update(buyQuantity, TradeModelBuys);
    newItem->update(sellQuantity, TradeModelSells);
    newItem->update(buyPrice, TradeModelBuyPrice);
    newItem->update(sellPrice, TradeModelSellPrice);
    newItem->update(buyPrice * buyQuantity, TradeModelTotalBT);
    newItem->update(sellPrice * sellQuantity, TradeModelTotalSLD);
    newItem->update(0, TradeModelCommission);
}
