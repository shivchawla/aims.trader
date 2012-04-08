#include "Platform/View/PositionViewItem.h"
#include "Platform/View/TableItem.h"

PositionViewItem::PositionViewItem()
{
     for(int i=0;i<_numItems;++i)
     {
         _items[i] = new TableItem();
     }
     _oldPositionValue=0;
     _oldNetProfit=0;
}

PositionViewItem::~PositionViewItem()
{
     for(int i=0;i<_numItems;++i)
     {
         delete _items[i];
     }
}

void PositionViewItem::update(const QString& text, const PositionModelColumn column)
{
    switch(column)
    {
        case Strategy: _items[Strategy]->updateItem(text);break;
        case PositionID:_items[PositionID]->updateItem(text);break;
        case InstrumentId:_items[InstrumentId]->updateItem(text);break;
        case Exchange:_items[Exchange]->updateItem(text);break;
        case TradePrice: _items[TradePrice]->updateItem(text);break;
        case LastPrice:_items[LastPrice]->updateItem(text);break;
        case Quantity:_items[Quantity]->updateItem(text);break;
        case PositionValue:_items[PositionValue]->updateItem(text);break;
        case GrossProfit:_items[GrossProfit]->updateItem(text);break;
        case NetProfit:_items[NetProfit]->updateItem(text);break;
    }
}

void PositionViewItem::updateNetProfit(const double netProfit)
{
    _items[NetProfit]->setText(QString::number(netProfit));
    if(netProfit >= 0)
    {
        _items[NetProfit]->setForeground(Qt::green);
    }
    else
    {
        _items[NetProfit]->setForeground(Qt::red);
    }
    _oldNetProfit = netProfit;

}

void PositionViewItem::updatePositionValue(const double positionValue)
{
    _items[PositionValue]->setText(QString::number(positionValue));
    /*if(positionValue >= _oldPositionValue)
    {
        _items[PositionValue]->setForeground(Qt::green);
    }
    else
    {
        _items[PositionValue]->setForeground(Qt::red);
    }*/
    _oldPositionValue = positionValue;
}

TableItem* PositionViewItem::getTableItem(const int col)
{
    return _items[col];
}




