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

//int y = OneMinuteMomentum::addStrategy();

OneMinuteMomentum::~OneMinuteMomentum(){}

OneMinuteMomentum::OneMinuteMomentum(const String& strategyName):Strategy(strategyName)
{}

OneMinuteMomentum::OneMinuteMomentum():Strategy("OneMinuteMomentum")
{}

void OneMinuteMomentum::initialize()
{
    Strategy::initialize();

    QString start = "10:00";
    QString end = "15:30";
    _tradingSchedule = new TradingSchedule(start, end , EST);
    setupIndicator();
    //call base class initialization
}

void OneMinuteMomentum::setupIndicator()
{
    //now setup things pertinent to this strategy
    _indicatorSPtr = new ShortTermWinnersAndLosers(this);
    //send the index
    //ShortTermWinnersAndLosers* indicator = static_cast<ShortTermWinnersAndLosers*>(_indicatorSPtr);
    //indicator->setup("Indices - Indices S&P500", 1);
    //indicator->setup();
}

void OneMinuteMomentum::setup(const String &index, const int time)
{
    QDir directory("/Users/shivkumarchawla/aims.trader/AIMSTrader");

    QString fileName = directory.path() +"/Symbol Lists/" + index + ".txt";
    QFile file(fileName);
    std::cout<<fileName.toStdString();

    String symbol;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!file.atEnd())
        {
            in >> symbol;
            _instruments.push_back(symbol);
        }
    }
    file.close();

    _numInstruments = _instruments.size();
    _timeScale = time;
}

void OneMinuteMomentum::setup(const std::vector<String>& instruments, const int time)
{
    _numInstruments = instruments.size();
    for(int i=0;i<_numInstruments;++i)
    {
        _instruments.push_back(instruments[i]);
    }
    _timeScale = time;
    ShortTermWinnersAndLosers* indicator = static_cast<ShortTermWinnersAndLosers*>(_indicatorSPtr);
    indicator->setNumInstruments(_numInstruments);
    //indicator->setup("");
}

void OneMinuteMomentum::startStrategy()
{
    InstrumentManager* im = Service::Instance()->getInstrumentManager();
    for(int i = 0; i<_numInstruments; ++i)
    {
        //contract.symbol = _instruments[i]
        im->requestMarketData(_instruments[i], _indicatorSPtr, IB, Snapshot);
    }
}

const std::vector<String>& OneMinuteMomentum::getInstruments()
{
    return _instruments;
}


