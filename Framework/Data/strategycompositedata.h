#ifndef STRATEGYCOMPOSITEDATA_H
#define STRATEGYCOMPOSITEDATA_H

//#include <QObject>
#include <QList>
#include "strategylinkedpositiondata.h"
#include "orderdata.h"

class StrategyData;
//class StrategyLinkedPositionData;
//class OrderData;

class StrategyCompositeData // : public QObject
{
    //Q_OBJECT
public:
    //properties
    StrategyData* strategy;
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
