#include "Platform/View/StrategyModel2.h"
#include "Platform/Strategy/StrategyManager.h"
#include <QColor>

StrategyModel2::StrategyModel2(QObject* parent):GeneralModel2<StrategyModel>(parent)
{}

QVariant StrategyModel2::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    if (row >= _strategyPerformanceData.count() || row < 0)
        return QVariant();

    if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;

     if(role == Qt::TextColorRole)
     {
         const StrategyModelData& data = _strategyPerformanceData.value(row, StrategyModelData());
         const PerformanceStats& stats = data.getPerformanceStats();

         switch(index.column())
         {
            case StrategyModelProfit: return (stats.totalProfit > 0) ? QVariant(QColor(Qt::green)) : QVariant(); break;
            case StrategyModelLoss: return (stats.totalLoss > 0) ? QVariant(QColor(Qt::red)) : QVariant(); break;
            case StrategyModelRunningGrossPnL: return (stats.runningGrossPnL > 0) ? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)); break;
            case StrategyModelRealizedGrossPnL: return (stats.realizedGrossPnL > 0) ? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)) ;break;
            case StrategyModelNetPnL: return (stats.netPnL > 0) ? QVariant(QColor(Qt::green)) : QVariant(QColor(Qt::red)) ;break;
            default: return QVariant();
         }
         return QVariant();
     }

    if (role == Qt::DisplayRole)
    {
        const StrategyModelData& data = _strategyPerformanceData.value(row, StrategyModelData());

        StrategyId strategyId = data.getStrategyId();
        const PerformanceStats& stats = data.getPerformanceStats();

        switch(index.column())
        {
            case StrategyModelStrategyName: return StrategyManager::strategyManager().getStrategyName(strategyId) ;break;
            case StrategyModelTotalTrades: return stats.trades ;break;
            case StrategyModelWinningTrades: return stats.profitableTrades; break;
            case StrategyModelLosingTrades: return stats.trades - stats.profitableTrades; break;
            case StrategyModelProfit: return stats.totalProfit; break;
            case StrategyModelLoss: return stats.totalLoss; break;
            case StrategyModelProfitFactor: return stats.profitFactor ;break;
            case StrategyModelTotalBought: return stats.totalBought ;break;
            case StrategyModelTotalSold: return stats.totalSold; break;
            case StrategyModelRunningGrossPnL: return stats.runningGrossPnL; break;
            case StrategyModelRealizedGrossPnL: return stats.realizedGrossPnL ;break;
            case StrategyModelNetPnL: return stats.netPnL ;break;
            case StrategyModelDrawDown: return stats.drawDown; break;
            case StrategyModelMaxDrawDown: return stats.maxDrawdown; break;
            case StrategyModelTotalCommission: return  stats.totalCommission;   break;
            case StrategyModelInvestment: return 0; break;
        }
    }
    return QVariant();
}

bool StrategyModel2::IsStrategyActive(const QModelIndex& index)
{
    return true;
}

const StrategyId StrategyModel2::getStrategyId(const QModelIndex& index)
{
    int row;
    if(index != QModelIndex())
    {
        row = index.row();
        return _strategyPerformanceData[row].getStrategyId();
    }
    return -1;
}

void StrategyModel2::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    int row;
    if((row = _indices.value(strategyId,-1)) != -1)
    {
        _strategyPerformanceData[row].setPerformanceStats(performanceStats);
        emit dataChanged(createIndex(row,1), createIndex(row,_numCols-1));
    }
    else //add strategy
    {
        int row = _numRows++;
        _indices[strategyId] = row;
        StrategyModelData data(strategyId, performanceStats);
        _strategyPerformanceData.append(data);
        insertRows(row,1);
    }
}

