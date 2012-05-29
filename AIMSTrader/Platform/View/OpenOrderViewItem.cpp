#include "Platform/View/OpenOrderViewItem.h"

OpenOrderViewItem::OpenOrderViewItem()
{
    for(int i=0;i<_numItems;++i)
    {
        _items[i] = new TableItem();
    }
}

OpenOrderViewItem::~OpenOrderViewItem()
{
    int numItems = OpenOrderViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        delete _items[i];
    }
}

void OpenOrderViewItem::update(const QString& value, const OpenOrderModelColumn column)
{
    //switch is useful for specialized update.
    //we can also use a map to first check whether e have a correpsonding field in the vew or not
    switch(column)
    {
        case OpenOrderID: _items[OpenOrderID]->updateItem(value); break;
        case Strategy: _items[Strategy]->updateItem(value); break;
        case SecurityType: _items[SecurityType]->updateItem(value); break;
        case Symbol: _items[Symbol]->updateItem(value); break;
        case OrderStatus: _items[OrderStatus]->updateItem(value); break;
        case FilledQuantity: _items[FilledQuantity]->updateItem(value); break;
        case RemainingQuantity: _items[RemainingQuantity]->updateItem(value); break;
        case TotalQuantity: _items[TotalQuantity]->updateItem(value); break;
        case AvgFillPrice: _items[AvgFillPrice]->updateItem(value); break;
        case LastFillPrice: _items[LastFillPrice]->updateItem(value); break;
        case DateTime: _items[DateTime]->updateItem(value); break;
        case OrderType: _items[OrderType]->updateItem(value); break;
    }
}

TableItem* OpenOrderViewItem::getTableItem(const int col)
{
    return _items[col];
}
