#include "Platform/View/InstrumentViewItem.h"
#include "Platform/View/TableItem.h"

InstrumentViewItem::InstrumentViewItem()
{
    _oldLastPrice=0;
    _oldAskPrice=0;
    _oldBidPrice=0;
}

InstrumentViewItem::~InstrumentViewItem()
{}

const int InstrumentViewItem::getNumItems()
{
    return _numItems;
}

void InstrumentViewItem::updateLastPrice(const double lastPrice)
{
    printf("%f\n",lastPrice);
    _items[Last].setText(QString::number(lastPrice));

    if(_oldLastPrice>lastPrice)
    {
        _items[Last].setForeground(Qt::red);
    }
    else
    {
       _items[Last].setForeground(Qt::green);
    }
    _oldLastPrice=lastPrice;
}

void InstrumentViewItem::updateLastSize(const int lastSize)
{
    _items[LastSize].setText(QString::number(lastSize));
}

void InstrumentViewItem::updateAskSize(const int askSize)
{
    _items[AskSize].setText(QString::number(askSize));
}

void InstrumentViewItem::updateBidSize(const int bidSize)
{
    _items[BidSize].setText(QString::number(bidSize));
}

void InstrumentViewItem::updateBidPrice(const double bidPrice)
{
   _items[Bid].setText(QString::number(bidPrice));

   if(_oldBidPrice>bidPrice)
   {
      _items[Bid].setForeground(Qt::red);
   }
   else
   {
      _items[Bid].setForeground(Qt::green);
   }
   _oldBidPrice=bidPrice;
}

void InstrumentViewItem::updateAskPrice(const double askPrice)
{
   _items[Ask].setText(QString::number(askPrice));

   if(_oldAskPrice>askPrice)
   {
       _items[Ask].setForeground(Qt::red);
   }
   else
   {
      _items[Ask].setForeground(Qt::green);
   }
   _oldAskPrice=askPrice;
}

void InstrumentViewItem::setExchange(const std::string& exchange)
{
    _items[Exchange].updateItem(exchange);
}

void InstrumentViewItem::setInstrumentID(const std::string& symbol)
{
    _items[InstrumentId].updateItem(symbol);
}

void InstrumentViewItem::setTickerId(const TickerId tickerId)
{
    _items[TickerID].updateItem(tickerId);
}

TableItem* InstrumentViewItem::getTableItem(const int col)
{
    return _items+col;
}




