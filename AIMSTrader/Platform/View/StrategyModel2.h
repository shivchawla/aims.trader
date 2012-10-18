#ifndef STRATEGYMODEL2_H
#define STRATEGYMODEL2_H
#include "Platform/View/GeneralModel2.h"
#include "Platform/Model/StrategyModel.h"

class StrategyModelData
{
    private:
        StrategyId _strategyId;
        PerformanceStats _performanceStats;

    public:
        StrategyModelData(const StrategyId strategyId):_strategyId(strategyId)
        {}

        StrategyModelData(const StrategyId strategyId, const PerformanceStats& stats):_strategyId(strategyId), _performanceStats(stats)
        {}

        StrategyModelData(){}

    public:
        const StrategyId getStrategyId() const {return _strategyId;}
        const PerformanceStats& getPerformanceStats() const {return _performanceStats;}
        void setPerformanceStats(const PerformanceStats& stats)
        {
            _performanceStats = stats;
        }
};

class StrategyModel2: public GeneralModel2<StrategyModel>
{
    public:
        StrategyModel2(QObject* parent = 0);

    public:
        QVariant data(const QModelIndex &index, int role) const;

    public:
        bool IsStrategyActive(const QModelIndex&);
        const StrategyId getStrategyId(const QModelIndex&);

    public:
        void updatePerformance(const StrategyId, const PerformanceStats&);

    private:
        QList<StrategyModelData>  _strategyPerformanceData;
        QHash<StrategyId, int> _indices;
};

#endif // STRATEGYMODEL2_H
