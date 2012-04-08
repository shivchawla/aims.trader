#ifndef OPENORDERVIEWITEM_H
#define OPENORDERVIEWITEM_H

#include "Platform/Model/OpenOrderModel.h"
#include "Platform/View/TableItem.h"

class OpenOrderViewItem
{
    private:
        const static int _numItems=11;
        TableItem _items[_numItems];
    public:
        OpenOrderViewItem();
        ~OpenOrderViewItem();
    public:
        //void updateOpenOrderID(cons OrderId);
        //void updateStrategyName(const sts::string&);
        void update(const QString&, const OpenOrderModelColumn);

    public:
        const static int getNumItems()
        {
            return _numItems;
        }
        TableItem* getTableItem(const int col);

};

#endif // OPENORDERVIEWITEM_H

