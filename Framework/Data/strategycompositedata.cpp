#include "strategycompositedata.h"
#include "strategydata.h"
#include <QDebug>

// Uncomment this line if you need to inherit this class from QObject
//StrategyCompositeData::StrategyCompositeData(QObject *parent) : QObject(parent)

StrategyCompositeData::StrategyCompositeData()
{
}


StrategyCompositeData::~StrategyCompositeData()
{
    delete strategy;

    foreach(StrategyLinkedPositionData* p, positions) {
        delete p;
    }

    foreach(OrderData* o, orders) {
        delete o;
    }
    positions.clear();
    orders.clear();
}

void StrategyCompositeData::printDebug()
{
    qDebug() << "Strategy Object: ";
    strategy->printDebug();

    qDebug() << "All positions for this Strategy:" << endl;
    foreach(StrategyLinkedPositionData* p, positions) {
        p->printDebug();
    }

    qDebug() << "All orders for this strategy:" << endl;
    foreach(OrderData* o, orders) {
        o->printDebug();
    }

}
