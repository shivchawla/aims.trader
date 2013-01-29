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
        int _row;

    public:
        TableViewItem();
        TableViewItem(const int);
        //~TableViewItem();

     public:
        TableCellItem<ViewItem>* getCellItem(const int);
        const int getNumItems();
        void update(const std::string&, const int itemColumn);
        void update(const String&, const int itemColumn);
        //void update(const QVariant&, const int itemColumn);
        void update(const double, const int itemColumn);
        void update(const float, const int itemColumn);
        void update(const uint, const int itemColumn);
        void update(const int, const int itemColumn);
        void update(const QDateTime&, const int itemColumn);
        void updateSpecial(const double, const int itemColumn);
        const String getColumnText(const int itemColumn) const;
        const QVariant getData(const int itemColumn) const;
        void addCell();
        const int getRow();
        void setRow(const int);
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
const int TableViewItem<ViewItem>::getRow()
{
    return _row;
}

template<class ViewItem>
void TableViewItem<ViewItem>::setRow(const int row)
{
   _row = row;
}


template<class ViewItem>
TableCellItem<ViewItem>* TableViewItem<ViewItem>::getCellItem(const int num)
{
    if(num<_numCells)
    {
        return _cells[num];
    }
    return NULL;
}

//template<class ViewItem>
//TableViewItem<ViewItem>::~TableViewItem()
//{
//    for(int i=0;i<_numCells;++i)
//    {
//        delete _cells[i];
//    }
//}

//template<class ViewItem>
//void TableViewItem<ViewItem>::show()
//{
//    for(int i=0;i<_numCells;++i)
//    {
//        _cells[i]->hide(false);
//    }
//}


//template<class ViewItem>
//void TableViewItem<ViewItem>::update(const QVariant& data, const int itemColumn)
//{
//    if(itemColumn != -1)
//    {
//        _cells[itemColumn]->updateItem(data);
//    }
//}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const double val, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(val);
    }
}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const float val, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(val);
    }
}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const uint val, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(val);
    }
}

template<class ViewItem>
void TableViewItem<ViewItem>::update(const int val, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(val);
    }
}


template<class ViewItem>
void TableViewItem<ViewItem>::update(const QDateTime& dateTime, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItem(dateTime);
    }
}


template<class ViewItem>
void TableViewItem<ViewItem>::updateSpecial(const double data, const int itemColumn)
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->updateItemSpecial(data);
    }
}


template<class ViewItem>
const QVariant TableViewItem<ViewItem>::getData(const int itemColumn) const
{
    if(itemColumn != -1)
    {
        _cells[itemColumn]->getData(itemColumn);
    }
    return QVariant();
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
void TableViewItem<ViewItem>::update(const String& text, const int itemColumn)
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




 #endif // TABLEVIEWITEM_H
