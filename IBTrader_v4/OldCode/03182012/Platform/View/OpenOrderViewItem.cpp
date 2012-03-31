#include "Platform/View/OpenOrderViewItem.h"

OpenOrderViewItem::OpenOrderViewItem()
{
    for(int i=0;i<_numItems;++i)
    {
        _item[i].setText("");
        _item[i].setBackgroundColor(Qt::black);
        _item[i].setForeground(Qt::white);
        _item[i].setTextAlignment(Qt::AlignHCenter);
    }
}

QTableWidgetItem* OpenOrderViewItem::getItemPointer()
{
    return _item;
}

const int OpenOrderViewItem::getNumItems()
{
    return _numItems;
}
