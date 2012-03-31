#ifndef STRATEGYMODEL_H
#define STRATEGYMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include <map>

enum StrategyModelColumn{
        StrategyID=0,
        StrategyName,
        TotalTrades,
        WinningTrades,
        LosingTrades,
        ProfitFactor,
        NetProfit,
        Investment
};

#include "Platform/typedefs.h"
class Strategy;

class StrategyModel: public QAbstractTableModel
{
    private:
        const Strategy* _strategyWPtr;
        int _numColumns;
        int _numRows;
        QStringList columnNames;
        std::map<int, QModelIndex> _strategyIdToTableIndex;

    public:
        StrategyModel(const Strategy*);
        StrategyModel();
        ~StrategyModel();

    public:
       //void setStrategyMap(const Strategy*);
       int rowCount( const QModelIndex& parent ) const;
       int columnCount( const QModelIndex& parent ) const;
       QVariant data( const QModelIndex& index, int role ) const;
       QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
       bool insertRows();

    public:
      void updateStrategyPerformance(const StrategyId strategyId);

};

#endif // STRATEGYMODEL_H
