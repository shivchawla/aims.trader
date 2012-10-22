#ifndef STRATEGYPOSITIONDETAILMODEL_H
#define STRATEGYPOSITIONDETAILMODEL_H

#include "Platform/View/GeneralModel2.h"
#include "Platform/Model/StrategyPositionModel.h"


class StrategyPositionDetailModel : GeneralModel2<StrategyPositionModel>
{
public:
    StrategyPositionDetailModel(QObject* parent = 0);

private:
    QHash<StrategyId, QHash<TickerId, int > > _indices;
    //QList<int, StrategyLinkedPositionDetailData> _detailPositions;
};

#endif // STRATEGYPOSITIONDETAILMODEL_H
