#ifndef STRATEGYCOMPOSITEDB_H
#define STRATEGYCOMPOSITEDB_H

#include <QObject>
#include <QString>
#include "Data/strategycompositedata.h"

class StrategyCompositeDb : public QObject
{
    Q_OBJECT
public:
    //explicit StrategyCompositeDb(QObject *parent = 0);
    StrategyCompositeDb();
    ~StrategyCompositeDb();
    StrategyCompositeData getStrategyComposite(const QString &strategyName);

signals:
    
public slots:

    
};

#endif // STRATEGYCOMPOSITEDB_H
