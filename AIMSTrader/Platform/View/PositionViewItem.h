#ifndef POSITIONVIEWITEM_H
#define POSITIONVIEWITEM_H

#include <QString>
#include "Platform/Model/PositionModel.h"
class TableItem;

class PositionViewItem
{
    private:
        const static int _numItems = 11;
        TableItem* _items[_numItems];

    private:
      double  _oldPositionValue;
      double  _oldNetProfit;

    public:
        PositionViewItem();
        ~PositionViewItem();


    public:
        TableItem* getTableItem(const int);

        const static int getNumItems()
        {
           return _numItems;
        }

     public:
        void update(const QString&, const PositionModelColumn);
        void updatePositionValue(const double);
        void updateNetProfit(const double);

};

#endif // POSITIONVIEWITEM_H

