#pragma once
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "Platform/Utils/Singleton.h"
#include <vector>
#include <QHash>

template <typename DataModelColumn>
class DataModel
{
    typedef QHash<DataModelColumn, QString> DataModelMap;
    typedef  typename QHash<DataModelColumn, QString>::Iterator DataModelMapIterator;
    protected:
         DataModelMap _dataModel;
         QHash<DataModelColumn, bool> _inDefaultModel;
         int _numDefaultColumns;
         int _numColumns;

    protected:
         void add(const DataModelColumn, const QString&, const bool);

   public:
     DataModel()
     {
         _numDefaultColumns=0;
         _numColumns=0;
     }

    public:
        const int getDataModelDefaultNumColumns()
        {
            return _numDefaultColumns;
        }

        const int getDataModelNumColumns()
        {
            return _numColumns;
        }

        const QList<DataModelColumn> getDataModelColumns()
        {
            return _dataModel.keys();
        }

        const QList<QString> getDataModelColumnNames()
        {
            return _dataModel.values();
        }

        const bool IsDefault(const DataModelColumn);
        const bool IsDefault(const int);

        void addInDefault(const DataModelColumn);

        const QString getColumnName(const DataModelColumn);
        const QString getColumnName(const int);
};

template <typename DataModelColumn>
const QString DataModel<DataModelColumn>::getColumnName(const DataModelColumn column)
{
    if(_dataModel.contains(column))
    {
        return _dataModel[column];
    }

    return "";
}

template <typename DataModelColumn>
const QString DataModel<DataModelColumn>::getColumnName(const int col)
{
    DataModelColumn column = DataModelColumn(col);
    if(_dataModel.contains(column))
    {
        return _dataModel[column];
    }

    return "";
}

template <typename DataModelColumn>
const bool DataModel<DataModelColumn>::IsDefault(const DataModelColumn column)
{
    return _inDefaultModel.value(column,false);
}

template <typename DataModelColumn>
const bool DataModel<DataModelColumn>::IsDefault(const int col)
{
    DataModelColumn column = DataModelColumn(col);
    return _inDefaultModel.value(column, false);
}


template <typename DataModelColumn>
void DataModel<DataModelColumn>::addInDefault(const DataModelColumn column)
{
    if(_inDefaultModel.contains(column))
    {
        _inDefaultModel[column] = true;
        _numDefaultColumns++;
    }
}

template <typename DataModelColumn>
void DataModel<DataModelColumn>::add(const DataModelColumn column, const QString& name, const bool defValue)
{
    _dataModel[column] = name;
    _inDefaultModel[column] = defValue;
    _numColumns++;
    if(defValue)
    {
        _numDefaultColumns++;
    }
}

#endif // DATAMODEL_H
