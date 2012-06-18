#ifndef STRATEGYCOMPOSITEDB_H
#define STRATEGYCOMPOSITEDB_H

#include <QObject>
#include <QString>
#include "strategycompositedata.h"

class StrategyCompositeDb : public QObject
{
    Q_OBJECT
public:
    //explicit StrategyCompositeDb(QObject *parent = 0);
    StrategyCompositeDb();
    ~StrategyCompositeDb();
    StrategyCompositeData* getStrategyComposite(QString strategyName);

signals:
    
public slots:

    
};

#endif // STRATEGYCOMPOSITEDB_H