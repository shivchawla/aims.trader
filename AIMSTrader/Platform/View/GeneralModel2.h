#ifndef GENERALMODEL2_H
#define GENERALMODEL2_H
#include <QAbstractTableModel>

template <class ModelColumn>
class GeneralModel2 : public QAbstractTableModel
{
    public:
        GeneralModel2(QObject *parent=0);

    public:
        int rowCount(const QModelIndex &parent=QModelIndex()) const;
        int columnCount(const QModelIndex &parent=QModelIndex()) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());

    public:
        const QList<QString> getModelColumnNames();
        const QList<ModelColumn> getModelColumns();
        const bool IsDefault(const ModelColumn);
        const bool IsDefault(const int);

    protected:
        int _numCols;
        int _numRows;
        ModelColumn* _modelColumn;
};

template <class ModelColumn>
GeneralModel2<ModelColumn>::GeneralModel2(QObject *parent):QAbstractTableModel(parent)
{
    _modelColumn = new ModelColumn();
    _numRows = 0;
    _numCols = _modelColumn->getDataModelNumColumns();
}

template <class ModelColumn>
int GeneralModel2<ModelColumn>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _numRows;
}

template <class ModelColumn>
int GeneralModel2<ModelColumn>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _numCols;
}

template <class ModelColumn>
QVariant GeneralModel2<ModelColumn>::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;

    if (orientation == Qt::Horizontal)
        return _modelColumn->getColumnName(section);

    return QVariant();
}

template <class ModelColumn>
bool GeneralModel2<ModelColumn>::insertRows(int position, int rows, const QModelIndex &index)
{
     Q_UNUSED(index);
     beginInsertRows(QModelIndex(), position, position+rows-1);
     endInsertRows();
     return true;
}

template <class ModelColumn>
Qt::ItemFlags GeneralModel2<ModelColumn>::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


template <class ModelColumn>
const QList<QString> GeneralModel2<ModelColumn>::getModelColumnNames()
{
     return _modelColumn->getDataModelColumnNames();
}

template <class ModelColumn>
const QList<ModelColumn> GeneralModel2<ModelColumn>::getModelColumns()
{
     return _modelColumn->getDataModelColumns();
}

template <class ModelColumn>
const bool GeneralModel2<ModelColumn>::IsDefault(const ModelColumn column)
{
    return _modelColumn->IsDefault(column);
}

template <class ModelColumn>
const bool GeneralModel2<ModelColumn>::IsDefault(const int column)
{
    return _modelColumn->IsDefault(column);
}


#endif // GENERALMODEL2_H
