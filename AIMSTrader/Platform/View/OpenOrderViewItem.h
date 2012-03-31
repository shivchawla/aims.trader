#ifndef OpenOrderViewItem_H
#define OpenOrderViewItem_H
#include <QTableWidgetItem>

class OpenOrderViewItem
{
    const static int _numItems = 11;
    QTableWidgetItem _item[_numItems];

public:
    OpenOrderViewItem();
public:
    QTableWidgetItem* getItemPointer();
    const static int getNumItems();
};

#endif // OpenOrderViewItem_H
