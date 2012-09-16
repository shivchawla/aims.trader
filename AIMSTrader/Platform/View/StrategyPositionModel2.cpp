#include "Platform/View/StrategyPositionModel2.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

StrategyPositionModel2::StrategyPositionModel2(QObject *parent) : QAbstractTableModel(parent)
{
    _columns = 0;
}

StrategyPositionModel2::StrategyPositionModel2(QHash<StrategyId , QHash <TickerId, PositionDetail> > positions, QObject *parent) : QAbstractTableModel(parent)
{
     //_positions = positions;
     _columns = 0;
}

int StrategyPositionModel2::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _indices.count();
}

int StrategyPositionModel2::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _columns;
}

QVariant StrategyPositionModel2::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     int row = index.row();
     if (row >= _positions.count() || row < 0)
         return QVariant();

     if (role == Qt::DisplayRole)
     {
         if(PositionDetail* detail = _positions.value(row, NULL))
         {
             switch(index.column())
             {
                case StrategyPositionModelStrategy: return StrategyManager::strategyManager().getStrategyName(_indices[row].first);
                case StrategyPositionModelInstrumentName: return "";
                case StrategyPositionModelBuys: return (int)detail->getSharesBought();
                case StrategyPositionModelSells: return (int)detail->getSharesSold();
                case StrategyPositionModelNet: return (int)detail->getNetShares();
                case StrategyPositionModelAvgBT: return detail->getAvgBought();
                case StrategyPositionModelAvgSLD: return detail->getAvgSold();
                case StrategyPositionModelTotalBT: return detail->getTotalValueBought();
                case StrategyPositionModelTotalSLD: return detail->getTotalValueSold();
                case StrategyPositionModelNetTotal: return detail->getNetValue();
                case StrategyPositionModelLastPrice: return detail->getMarkedPrice();
                case StrategyPositionModelCommission: return detail->getTotalCommission();
                case StrategyPositionModelRunningPL: return detail->getRunningPnl();
                case StrategyPositionModelRealizedPL: return detail->getRealizedPnl();
                case StrategyPositionModelPL: return detail->getPnL();
                case StrategyPositionModelNetInclCommission: return detail->getNetTotalIncCommission();
             }
         }
     }
     return QVariant();
}

QVariant StrategyPositionModel2::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         switch (section) {
             case StrategyPositionModelStrategy: return "Strategy";
             case StrategyPositionModelInstrumentName: return "Instrument";
             case StrategyPositionModelBuys: return "Buys";
             case StrategyPositionModelSells: return "Sells";
             case StrategyPositionModelNet: return "Net";
             case StrategyPositionModelAvgBT: return "AvgBT";
             case StrategyPositionModelAvgSLD: return "AvgSLD";
             case StrategyPositionModelTotalBT: return "TotalBT";
             case StrategyPositionModelTotalSLD: return "TotalSLD";
             case StrategyPositionModelNetTotal: return "NetTotal";
             case StrategyPositionModelLastPrice: return "LastPrice";
             case StrategyPositionModelCommission: return "Commission";
             case StrategyPositionModelRunningPL: return "RunningPL";
             case StrategyPositionModelRealizedPL: return "RealizedPL";
             case StrategyPositionModelPL: return "PnL";
             case StrategyPositionModelNetInclCommission: return "NetInclCommisssion";
             default:
                 return QVariant();
         }
     }
     return QVariant();
}

bool StrategyPositionModel2::insertRows(int position, int rows, const QModelIndex &index)
{
     Q_UNUSED(index);
     beginInsertRows(QModelIndex(), position, position+rows-1);
     endInsertRows();
     return true;
}

bool StrategyPositionModel2::removeRows(int position, int rows, const QModelIndex &index)
{
     Q_UNUSED(index);
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; ++row) {
         _indices.remove(row);
         _positions.remove(row);
     }

     endRemoveRows();
     return true;
}

Qt::ItemFlags StrategyPositionModel2::flags(const QModelIndex &index) const
{
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
