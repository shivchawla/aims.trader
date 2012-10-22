#include "Platform/View/StrategyPositionModel2.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

StrategyPositionModel2::StrategyPositionModel2(QObject *parent):GeneralModel2<StrategyPositionModel>(parent)
{
    count = 0;
}

QVariant StrategyPositionModel2::data(const QModelIndex &index, int role) const
{
    //qDebug()<<"Hello";
     if (!index.isValid())
         return QVariant();

     int row = index.row();
     if (row >= _numRows || row < 0)
         return QVariant();

     if (role == Qt::TextAlignmentRole)
             return Qt::AlignCenter;

     if(role == Qt::TextColorRole)
     {
         const StrategyPositionModelData& data = _positions.value(row, StrategyPositionModelData());
         const PositionDetail& detail = data.getPositionDetail();

         switch(index.column())
         {
            //case StrategyPositionModelLastPrice: return detail.getMarkedPrice(); break;
            case StrategyPositionModelRunningPL: return (detail.getRunningPnl() > 0) ? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)); break;
            case StrategyPositionModelRealizedPL: return (detail.getRealizedPnl() > 0)? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)); break;
            case StrategyPositionModelPL: return (detail.getNetPnL() >0)? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)); break;
            default: return QVariant();
         }
         //return QVariant();
     }

     if (role == Qt::DisplayRole)
     {
         const StrategyPositionModelData& data = _positions.value(row, StrategyPositionModelData());
         const PositionDetail& detail = data.getPositionDetail();
         StrategyId strategyId = data.getStrategyId();
         TickerId tickerId = data.getTickerId();

         switch(index.column())
         {
            case StrategyPositionModelStrategy: return StrategyManager::strategyManager().getStrategyName(strategyId);break;
            case StrategyPositionModelInstrumentName: return Service::service().getInstrumentManager()->getSymbol(tickerId);break;
            case StrategyPositionModelBuys: return (int)detail.getSharesBought();break;
            case StrategyPositionModelSells: return (int)detail.getSharesSold();break;
            case StrategyPositionModelNet: return (int)detail.getNetShares();break;
            case StrategyPositionModelAvgBT: return detail.getAvgBought();break;
            case StrategyPositionModelAvgSLD: return detail.getAvgSold();break;
            case StrategyPositionModelTotalBT: return detail.getTotalValueBought();break;
            case StrategyPositionModelTotalSLD: return detail.getTotalValueSold();break;
            case StrategyPositionModelNetTotal: return detail.getNetValue();break;
            case StrategyPositionModelLastPrice: return detail.getMarkedPrice();break;
            case StrategyPositionModelCommission: return detail.getTotalCommission();break;
            case StrategyPositionModelRunningPL: return detail.getRunningPnl();break;
            case StrategyPositionModelRealizedPL: return detail.getRealizedPnl();break;
            case StrategyPositionModelPL: return detail.getNetPnL();break;
            case StrategyPositionModelNetInclCommission: return detail.getNetTotalIncCommission();break;
         }
     }
     return QVariant();
}

//QVariant StrategyPositionModel2::headerData(int section, Qt::Orientation orientation, int role) const
//{
//     if (role != Qt::DisplayRole)
//         return QVariant();

//     if (role == Qt::TextAlignmentRole)
//             return Qt::AlignCenter;

//     if (orientation == Qt::Horizontal) {
//         return _modelColumns->getColumnName(section);
//     }
//     return QVariant();
//}

//bool StrategyPositionModel2::insertRows(int position, int rows, const QModelIndex &index)
//{
//     Q_UNUSED(index);
//     beginInsertRows(QModelIndex(), position, position+rows-1);
//     endInsertRows();
//     return true;
//}

//bool StrategyPositionModel2::removeRows(int position, int rows, const QModelIndex &index)
//{
////     Q_UNUSED(index);
////     beginRemoveRows(QModelIndex(), position, position+rows-1);

////     int data;

////     QList<StrategyPositionModelData>::iterator begin = _positions.begin() + position;
////     QList<StrategyPositionModelData>::iterator end = _positions.begin() + position + rows;

////     _positions.erase(begin, end);

////     for (int row=0; row < rows; ++row)
////     {
////         //remove the row from map
////         int trueRow = row + position;

////         StrategyPositionModelData data = _positions[trueRow]
////         _indices.remove(row);
////     }

////     endRemoveRows();
//     return true;
//}

//Qt::ItemFlags StrategyPositionModel2::flags(const QModelIndex &index) const
//{
//     if (!index.isValid())
//         return Qt::ItemIsEnabled;

//     return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
//}

void StrategyPositionModel2::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
        int row;
        //check if the position already exists in the model
        if((row = _indices.value(strategyId, QHash<TickerId, int>()).value(tickerId, -1)) != -1)
        {
            return;
        }

        row = _numRows++;
        _indices[strategyId][tickerId] = row;
        StrategyPositionModelData data(strategyId, tickerId);
        _positions.append(data);
        insertRows(row, 1);
        emit dataChanged(createIndex(row, StrategyPositionModelStrategy), createIndex(row, StrategyPositionModelInstrumentName));
}


void StrategyPositionModel2::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    int row;
    if((row = _indices.value(strategyId, QHash<TickerId, int>()).value(tickerId, -1)) != -1)
    {
        _positions[row].setPositionDetail(positionDetail);
        emit dataChanged(createIndex(row, 2), createIndex(row, _numCols-1));
    }
}

void StrategyPositionModel2::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{  
    int row;
    if((row = _indices.value(strategyId, QHash<TickerId, int>()).value(tickerId, -1)) != -1)
    {
        //const StrategyPositionModelData& data = _positions[row];
        //const PositionDetail& detail = data.getPositionDetail();
        emit dataChanged(createIndex(row,StrategyPositionModelRunningPL), createIndex(row,StrategyPositionModelRunningPL));
        emit dataChanged(createIndex(row,StrategyPositionModelPL), createIndex(row,StrategyPositionModelPL));
    }
}

void StrategyPositionModel2::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& detail)
{
    int row;
    if((row = _indices.value(strategyId, QHash<TickerId, int>()).value(tickerId, -1)) != -1)
    {
        //StrategyPositionModelData data = _positions[row];
        _positions[row].setPositionDetail(detail);

        QModelIndex idx = createIndex(row,StrategyPositionModelRunningPL);
        emit dataChanged(idx, idx);
        idx = createIndex(row,StrategyPositionModelPL);
        emit dataChanged(idx, idx);
        //emit dataChanged(createIndex(row,StrategyPosition), createIndex(row,StrategyPositionModelPL));
    }
}


void StrategyPositionModel2::removePosition(const StrategyId strategyId, const TickerId tickerId)
{}

bool StrategyPositionModel2::IsPositionActive(const QModelIndex& idx)
{
      int row = idx.row();
      if(row!=-1)
      {
        const PositionDetail& detail = _positions.at(row).getPositionDetail();
        return (detail.getNetShares()!=0);
      }
      return false;
}

const StrategyId StrategyPositionModel2::getStrategyId(const QModelIndex& idx)
{
      int row = idx.row();
      if(row!=-1)
      {
        return _positions.at(row).getStrategyId();
      }

      return -1;
}

const TickerId StrategyPositionModel2::getTickerId(const QModelIndex& idx)
{
      int row = idx.row();
      if(row!=-1)
      {
        return _positions.at(row).getTickerId();
      }

      return -1;
}

