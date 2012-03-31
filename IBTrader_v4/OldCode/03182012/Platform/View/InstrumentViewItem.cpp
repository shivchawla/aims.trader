#include "InstrumentViewItem.h"

InstrumentViewItem::InstrumentViewItem()
{
    for(int i=0;i<_numItems;++i)
    {
        _item[i].setText("");
        _item[i].setBackgroundColor(Qt::black);
        _item[i].setForeground(Qt::white);
    }
}

QTableWidgetItem* InstrumentViewItem::getItemPointer()
{
    return _item;
}

const int InstrumentViewItem::getNumItems()
{
    return _numItems;
}
