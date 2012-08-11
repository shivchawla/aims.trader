#pragma once
#ifndef TABLEVIEWITEM_H
#define TABLEVIEWITEM_H
#include <QTableWidgetItem>
#include <QHash>
#include <vector>
#include "AimsTraderDefs/typedefs.h"
#include "Platform/View/TableCellItem.h"

//CRTP pattern
template<class ViewItem>
class TableViewItem
 {
    protected:
        int _numCells;
        std::vector<TableCellItem<ViewItem>* > _cells;

    public:
        TableViewItem(const int);
        ~TableViewItem();

     public:
        TableCellItem<ViewItem>* getTableItem(const int);
        const int getNumItems();
        void update(const String& text, const int itemColumn);
        void update(const std::string& text, const int itemColumn);
        void updateSpecial(const double value, int itemColumn);
        const String getColumnText(const int itemColumn) const;
        void addCell();
        //void show();
        //void hide();
        TableViewItem();
 };

template<class ViewItem>
TableViewItem<ViewItem>::TableViewItem(const int numCells):_numCells(numCells)
{
    _cells.reserve(numCells);
    for(int i=0;i<_numCells;++i)
    {
        _cells.push_back(new TableCellItem<ViewItem>((ViewItem*)this));
    }
}

template<class ViewItem>
const int TableViewItem<ViewItem>::getNumItems()
{
    return _numCells;
}

template<class ViewItem>
TableCellItem<ViewItem>* TableViewItem<ViewItem>::getTableItem(const int num)
{
    if(num<_numCells)
    {
        return _cells[num];
    }
    return NULL;
}

template<class ViewItem>
TableViewItem<ViewItem>::~TableViewItem()
{
    for(int i=0;i<_numCells;++i)
    {
        delete _cells[i];
    }
}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const QString& text, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(text);
    }
}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const std::string& text, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(text);
    }
}


template<class ViewItem>
const String TableViewItem<ViewItem>::getColumnText(const int itemColumn) const
{
    return _cells[itemColumn]->text();
}

template<class ViewItem>
void TableViewItem<ViewItem>::addCell()
{
   _cells.push_back(new TableCellItem<ViewItem>((ViewItem*)this));
   _numCells++;
}

//template<class ViewItem>
//void TableViewItem<ViewItem>::show()
//{
//    for(int i=0;i<_numCells;++i)
//    {
//        _cells[i]>show();
//    }
//}

//template<class ViewItem>
//void TableViewItem<ViewItem>::hide()
//{
//    for(int i=0;i<_numCells;++i)
//    {
//         //hide();
//        //_cells[i]>hide();
//    }
//}

template<class ViewItem>
void TableViewItem<ViewItem>::updateSpecial(const double value, int itemColumn)
{
    if(itemColumn!=-1)
    {
        double oldValue = _cells[itemColumn]->text().toDouble();
        _cells[itemColumn]->updateItem(value);
        if(oldValue <= 0 && value > 0)
        {
            _cells[itemColumn]->setForeground(Qt::green);
        }
        else if(oldValue >= 0 && value < 0)
        {
             _cells[itemColumn]->setForeground(Qt::red);
        }
        else if(value == 0)
        {
            _cells[itemColumn]->setForeground(Qt::white);
        }
    }
}


 #endif // TABLEVIEWITEM_H
