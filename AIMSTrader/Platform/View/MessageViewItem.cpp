#include "Platform/View/MessageViewItem.h"

MessageViewItem::MessageViewItem(const int numItems):TableViewItem<MessageViewItem>(numItems)
{
    for(int i=0; i<numItems;++i)
    {
        _cells[i]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        _cells[i]->setForeground(Qt::black);
    }
}
