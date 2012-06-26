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

OneMinuteMomentum::OneMinuteMomentum(const String& strategyName):Strategy(strategyName)
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
    setupIndicator();
    //call base class initialization
}

void OneMinuteMomentum::setupIndicator()
{
    _indicatorSPtr = new ShortTermWinnersAndLosers();
    setupIndicatorConnections();
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
    InstrumentManager* im = Service::Instance()->getInstrumentManager();

    int numInstruments = _buyList.length();
    for(int i = 0; i< numInstruments; ++i)
    {
        //contract.symbol = _instruments[i]
        im->requestMarketData(_buyList[i], _indicatorSPtr, IB, Snapshot);
    }
}


