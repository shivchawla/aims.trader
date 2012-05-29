#ifndef STRATEGYPOSITIONVIEWITEM_H
#define STRATEGYPOSITIONVIEWITEM_H

#include <QString>
#include "Platform/Model/StrategyPositionModel.h"
#include <vector>
#include <QHash>

class TableItem;
typedef std::vector<TableItem*> TableItems;

class StrategyPositionViewItem
{
    private:
        static int _numItems;
        //TableItem* _items[_numItems];
        TableItems _items;
        QHash<StrategyPositionModelColumn, int> _map;//item number to model column number

    private:
      //double  _oldPositionValue;
      //double  _oldNetProfit;

    public:
        StrategyPositionViewItem();
        ~StrategyPositionViewItem();

    public:
        TableItem* getTableItem(const int);

        static const int getNumItems()
        {
           return _numItems;
        }

     public:
        void update(const QString&, const StrategyPositionModelColumn);
        //void updatePositionValue(const double);
        //void updateNetProfit(const double);

};

#endif // STRATEGYPOSITIONVIEWITEM_H

