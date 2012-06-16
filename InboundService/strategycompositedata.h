#ifndef STRATEGYCOMPOSITEDATA_H
#define STRATEGYCOMPOSITEDATA_H

//#include <QObject>
#include <QList>
#include "strategyviewdata.h"
#include "strategylinkedpositiondata.h"
#include "orderdata.h"

//class StrategyViewData;
//class StrategyLinkedPositionData;
//class OrderData;

class StrategyCompositeData // : public QObject
{
    //Q_OBJECT
public:
    //properties
    StrategyViewData* strategy;
    QList<StrategyLinkedPositionData*> positions;
    QList<OrderData*> orders;

    //explicit StrategyCompositeData(QObject *parent = 0);
    StrategyCompositeData();
    ~StrategyCompositeData();
    void printDebug();
    
//signals:
    
//public slots:
    
};

#endif // STRATEGYCOMPOSITEDATA_H
