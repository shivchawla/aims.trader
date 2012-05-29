#ifndef TESTDATAGENERATOR_H
#define TESTDATAGENERATOR_H
#include <QObject>
#include <QReadWriteLock>
#include <map>
#include "Platform/typedefs.h"
#include <QThread>

class DataGenerator: public QObject
{
    Q_OBJECT
    public:
        DataGenerator();
        void reqMarketData(const TickerId);

    private:
        //void startData();

    private slots:
        void startDataGeneration();

    private:
        QReadWriteLock lock;
        std::map<TickerId, int> _instruments;
        std::map<TickerId, double> _seed;
        //QThread* thread;

};

#endif // TESTDATAGENERATOR_H
