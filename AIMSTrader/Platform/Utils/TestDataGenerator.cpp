#include "Platform/Utils/TestDataGenerator.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include <math.h>
//#include <QWaitCondition>
#include <QDebug>

typedef std::map<TickerId, double>::iterator MapIterator;
DataGenerator::DataGenerator():QObject()
{
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(startDataGeneration()));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void DataGenerator::reqMarketData(const InstrumentId instrumentId)
{
    lock.lockForWrite();
    if(_seed.count(instrumentId) == 0)
    {
        //_instruments[tickerId] = 1;
        double x= rand();
        double y = x/RAND_MAX;

        _seed[instrumentId] = 30 * (1 + y);
        double last =  _seed[instrumentId];
        double ask = last + 0.1;
        double bid = last - 0.1;
        //Service::service().getInstrumentManager()->tickGeneric(instrumentId, LAST, last);
        //Service::service().getInstrumentManager()->tickGeneric(instrumentId, BID, bid);
        //Service::service().getInstrumentManager()->tickGeneric(instrumentId, ASK, ask);
    }
    lock.unlock();
}

void DataGenerator::startDataGeneration()
{
    _id = startTimer(700);

    //QWaitCondition condition;
//    while(1)
//    {
//        //QObject::thread()->wait(2000);
//        lock.lockForRead();
//        condition.wait(&lock, 700);
//        lock.unlock();

//        lock.lockForRead();
//        MapIterator end = _seed.end();
//        MapIterator it;
//        for(it=_seed.begin() ;it!=end;++it)
//        {
//            double x= rand();
//            double y = x/RAND_MAX;

//            double last =  (it->second) + y;
//            double ask = last + 0.1;
//            double bid = last - 0.1;
//            Service::service().getInstrumentManager()->tickGeneric(it->first, LAST, last);
//            Service::service().getInstrumentManager()->tickGeneric(it->first, BID, bid);
//            Service::service().getInstrumentManager()->tickGeneric(it->first, ASK, ask);
//        }
//        lock.unlock();
//    }
}

void DataGenerator::timerEvent(QTimerEvent* event)
{
    lock.lockForRead();
    MapIterator end = _seed.end();
    MapIterator it;
    for(it=_seed.begin() ;it!=end;++it)
    {
        double x= rand();
        double y = x/RAND_MAX;

        double last =  (it->second) + y;
        double ask = last + 0.1;
        double bid = last - 0.1;
        Service::service().getInstrumentManager()->tickGeneric(it->first, LAST, last);
        Service::service().getInstrumentManager()->tickGeneric(it->first, BID, bid);
        Service::service().getInstrumentManager()->tickGeneric(it->first, ASK, ask);
    }
    lock.unlock();
}

DataGenerator::~DataGenerator()
{
    killTimer(_id);
    qDebug()<<"deleting datagenerator";
}

