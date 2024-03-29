#include "Strategy/OneMinuteMomentum.h"
#include "Platform/Utils/TimeZone.h"
#include "Platform/Utils/TradingSchedule.h"
#include <iostream>
#include "Indicator/ShortTermWinnersAndLosers.h"
#include <QDir>
#include <QTextStream>
#include <QFile>
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

OneMinuteMomentum::~OneMinuteMomentum(){}

OneMinuteMomentum::OneMinuteMomentum(const String& strategyName):StrategyImpl<OneMinuteMomentum>(strategyName)
{
    initialize();
}

OneMinuteMomentum::OneMinuteMomentum():StrategyImpl<OneMinuteMomentum>()
{
    initialize();
}

//OneMinuteMomentum::OneMinuteMomentum():Strategy("OneMinuteMomentum")
//{
//    initialize();
//}

void OneMinuteMomentum::initialize()
{
    QString start = "10:00";
    QString end = "15:30";
    _tradingSchedule = new TradingSchedule(start, end , EST);
    connectIndicatorSignals();
    //call base class initialization
}

void OneMinuteMomentum::connectIndicatorSignals()
{
    _indicatorPtr = new ShortTermWinnersAndLosers();
    connectIndicatorSignals();
}

//void OneMinuteMomentum::setup(const QList<String>& instruments, const int time)
//{
//    _instruments = instruments;
//    _timeScale = time;
//    ShortTermWinnersAndLosers* indicator = static_cast<ShortTermWinnersAndLosers*>(_indicatorSPtr);
//    indicator->setNumInstruments(_numInstruments);
//    //indicator->setup("");
//}

void OneMinuteMomentum::startStrategy()
{
    InstrumentManager* im = Service::service().getInstrumentManager();

    int numInstruments = _buyList.length();
    for(int i = 0; i< numInstruments; ++i)
    {
        im->requestMarketData(_buyList[i], _indicatorPtr, IB, Snapshot);
    }
}


