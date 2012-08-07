#pragma once
#ifndef TESTDATAGENERATOR_H
#define TESTDATAGENERATOR_H
#include <QObject>
#include <QReadWriteLock>
#include <map>
#include "AimsTraderDefs/typedefs.h"
#include <QThread>

class DataGenerator: public QObject
{
    Q_OBJECT
    public:
        DataGenerator();
        ~DataGenerator();

        void reqMarketData(const InstrumentId);

    private:
        //void startData();
        int _id;
    protected:
        void timerEvent(QTimerEvent* event);

    private slots:
        void startDataGeneration();

    private:
        QReadWriteLock lock;
        std::map<TickerId, int> _instruments;
        std::map<TickerId, double> _seed;
        //QThread* thread;

};

#endif // TESTDATAGENERATOR_H
