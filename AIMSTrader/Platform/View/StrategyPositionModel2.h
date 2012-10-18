#ifndef STRATEGYPOSITIONMODEL2_H
#define STRATEGYPOSITIONMODEL2_H

#include <QAbstractTableModel>
#include "Platform/Position/Position.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/GeneralModel2.h"
struct StrategyPositionModelData
{
    private:
        StrategyId _strategyId;
        TickerId _tickerId;
        PositionDetail _positionDetail;

    public:
        StrategyPositionModelData(const StrategyId strategyId, const TickerId tickerId):_strategyId(strategyId), _tickerId(tickerId)
        {}

        StrategyPositionModelData()
        {}

    public:
        const PositionDetail& getPositionDetail() const {return _positionDetail;}
        const StrategyId getStrategyId() const {return _strategyId;}
        const TickerId getTickerId() const {return _tickerId;}
        //const int getRowInModel() const {return _rowInModel;}
        void setStrategyId(const StrategyId strategyId)
        {
            _strategyId = strategyId;
        }

        void setTickerId(const TickerId tickerId)
        {
            _tickerId = tickerId;
        }

        void setPositionDetail(const PositionDetail& detail)
        {
            _positionDetail = detail;
        }
};

class StrategyPositionModel2 : public GeneralModel2<StrategyPositionModel>
{
    public:
        StrategyPositionModel2(QObject *parent=0);

    public:
        QVariant data(const QModelIndex &index, int role) const;
        bool IsPositionActive(const QModelIndex&);
        const StrategyId getStrategyId(const QModelIndex&);
        const TickerId getTickerId(const QModelIndex&);

    public:
        qint64 count;
        void updatePositionForLastPrice(const StrategyId, const TickerId, const double runningPnl, const double pnl);
        void updatePositionForLastPrice(const StrategyId, const TickerId, const PositionDetail&);
        void removePosition(const StrategyId, const TickerId);
        void updatePositionForExecution(const StrategyId, const TickerId, const PositionDetail&);
        void addPosition(const StrategyId, const TickerId);

    private:
        QList<StrategyPositionModelData>  _positions;
        QHash<StrategyId, QHash<TickerId, int> > _indices;
};

#endif // STRATEGYPOSITIONMODEL2_H
