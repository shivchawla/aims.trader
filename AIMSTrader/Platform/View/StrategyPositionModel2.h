#ifndef STRATEGYPOSITIONMODEL2_H
#define STRATEGYPOSITIONMODEL2_H

#include <QAbstractTableModel>
#include "Platform/Position/Position.h"

class StrategyPositionModel2 : public QAbstractTableModel
{
    Q_OBJECT
    public:
        StrategyPositionModel2(QObject *parent=0);
        StrategyPositionModel2(QHash<StrategyId, QHash<TickerId, PositionDetail> > positions, QObject *parent=0);

        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        //bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
        bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
        //QHash<StrategyId, QHash<TickerId, PositionDetail> > getPositions();

    private:
        QHash<int, PositionDetail*>  _positions;
        int _columns;
        QHash<int, QPair<StrategyId, TickerId> > _indices;

};

#endif // STRATEGYPOSITIONMODEL2_H
