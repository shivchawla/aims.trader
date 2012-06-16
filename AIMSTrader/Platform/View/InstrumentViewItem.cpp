#include "Platform/View/InstrumentViewItem.h"
#include "Platform/View/TableCellItem.h"

InstrumentViewItem::InstrumentViewItem(const int numItems):TableViewItem<InstrumentViewItem>(numItems)
{
//    for(int i=0;i<_numItems;++i)
//    {
//        InstrumentModelColumn col = InstrumentModel::Instance()->getColumns()[i];
//        _map[(int)col] = i;
//    }
    _tickerId = -1;
    _oldLastPrice=0;
    _oldAskPrice=0;
    _oldBidPrice=0;
}

InstrumentViewItem::~InstrumentViewItem()
{}

//void InstrumentViewItem::update(const QString& value, const InstrumentModelColumn column)
//{
//    int col = (int)column;
//    if(_map.contains(col))
//    {
//        _items[_map[col]]->updateItem(value);
//    }
//}

void InstrumentViewItem::updateLastPrice(const double lastPrice, const int column)
{
    if(column!=-1)
    {
        _cells[column]->setText(QString::number(lastPrice));

        if(_oldLastPrice>lastPrice)
        {
            _cells[column]->setForeground(Qt::red);
        }
        else
        {
           _cells[column]->setForeground(Qt::green);
        }
        _oldLastPrice=lastPrice;
    }
}

void InstrumentViewItem::updateBidPrice(const double bidPrice, const int column)
{

    if(column!=-1)
    {
        _cells[column]->setText(QString::number(bidPrice));

        if(_oldBidPrice > bidPrice)
        {
            _cells[column]->setForeground(Qt::red);
        }
        else
        {
           _cells[column]->setForeground(Qt::green);
        }
        _oldBidPrice = bidPrice;
    }
}

void InstrumentViewItem::updateAskPrice(const double askPrice, const int column)
{
    if(column!=-1)
    {
        _cells[column]->setText(QString::number(askPrice));

        if(_oldAskPrice>askPrice)
        {
            _cells[column]->setForeground(Qt::red);
        }
        else
        {
           _cells[column]->setForeground(Qt::green);
        }
        _oldAskPrice = askPrice;
    }
}


