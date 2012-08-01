#include "Platform/View/InstrumentViewItem.h"
#include "Platform/View/TableCellItem.h"
#include <QDebug>

InstrumentViewItem::InstrumentViewItem(const int numItems):TableViewItem<InstrumentViewItem>(numItems)
{
    _instrumentId = -1;
    _oldLastPrice=0;
    _oldAskPrice=0;
    _oldBidPrice=0;
}

InstrumentViewItem::~InstrumentViewItem()
{}

void InstrumentViewItem::updateLastPrice(const double lastPrice, const int column)
{
    if(column!=-1)
    {
        _cells[column]->setText(QString::number(lastPrice));

        if(_oldLastPrice>lastPrice)
        {
            _cells[column]->setTextColor(Qt::red);
        }
        else
        {
           _cells[column]->setTextColor(Qt::green);
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
            _cells[column]->setTextColor(QColor(Qt::red));//setForeground(Qt::red);
        }
        else
        {
           _cells[column]->setTextColor(Qt::green);
        }
       // qDebug()<<_oldBidPrice<<" "<<bidPrice<<" "<<_cells[column]->textColor().name();
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
            _cells[column]->setTextColor(Qt::red);
        }
        else
        {
           _cells[column]->setTextColor(Qt::green);
        }
        _oldAskPrice = askPrice;
    }
}


