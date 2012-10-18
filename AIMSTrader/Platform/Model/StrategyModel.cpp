#include "Platform/Model/StrategyModel.h"
#include "Platform/Strategy/Strategy.h"

StrategyModel::StrategyModel():QAbstractTableModel()
{
    _numRows=0;
    _numColumns=8;
}

StrategyModel::~StrategyModel(){}

StrategyModel::StrategyModel(const Strategy* strategyPtr)
{
    _strategyWPtr = strategyPtr;
}

int StrategyModel::rowCount( const QModelIndex& parent ) const
{
    return _numRows;
}

int StrategyModel::columnCount( const QModelIndex& parent ) const
{
       return _numColumns;
}

QVariant StrategyModel::data(const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
    {
           return QVariant();
    }

    if ( role == Qt::TextAlignmentRole )
    {
        return int( Qt::AlignRight | Qt::AlignVCenter );
    }
    else if( role == Qt::DisplayRole)
    {
        /*std::map<StrategyId, Strategy*>::iterator const it = _StrategyPtrMap.begin() +  index.row();
        int i=
        switch ( i )
        {
            case 0 : return _StrategyId;
            case 1 : return _StrategyPtrMap[StrategyId]->
            case 2 : return currentTicks.value( instrument )->getAsk();
            case 3 : return currentTicks.value( instrument )->getBidVolume();
            case 4 : return currentTicks.value( instrument )->getAskVolume();
        }*/
    }
    return QVariant();
}

QVariant StrategyModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
             switch (section)
             {
                 case StrategyID : return QString("StrategyId"); break;
                 case StrategyName : return QString("StrategyName"); break;
                 case TotalTrades : return QString("TotalTrades"); break;
                 case WinningTrades : return QString("WinningTrades"); break;
                 case LosingTrades : return QString("LosingTrades"); break;
                 case ProfitFactor : return QString("ProfitFactor"); break;
                 case NetProfit : return QString("NetProfit"); break;
                 case Investment : return QString("Investment"); break;
             }
         }
     }
     return QVariant();
}

void StrategyModel::updateStrategyPerformance(const StrategyId strategyId)
 {
    QModelIndex startIndex;
    QModelIndex endIndex;
     //we identify the tableIndex wrt StrategyId
    if(_strategyIdToTableIndex.count(strategyId)!=0)
    {
       startIndex = _strategyIdToTableIndex[strategyId];
    }
    else
    {
       //we identify the next available row
       insertRows();
       startIndex = createIndex(rowCount(QModelIndex())-1,0);
       _strategyIdToTableIndex[strategyId] = startIndex;
    }
    endIndex = createIndex(startIndex.row(),_numColumns-1);

     //emit a signal to make the view reread identified data
     emit dataChanged(startIndex, endIndex);
 }

bool StrategyModel::insertRows()
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    endInsertRows();
    _numRows++;
    return true;
}

