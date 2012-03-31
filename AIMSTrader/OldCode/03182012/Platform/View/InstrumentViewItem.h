#ifndef INSTRUMENTVIEWITEM_H
#define INSTRUMENTVIEWITEM_H
#include <QTableWidgetItem>

class InstrumentViewItem
{
    private:
        const static int _numItems = 13;
        QTableWidgetItem _item[_numItems];

    public:
        InstrumentViewItem();

    public:
        QTableWidgetItem* getItemPointer();
        const static int getNumItems();

};

#endif // INSTRUMENTVIEWITEM_H
