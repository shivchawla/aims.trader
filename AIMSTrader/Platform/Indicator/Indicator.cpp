/*
 *  Indicator.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Indicator/Indicator.h"
#include "Platform/Utils/ThreadManager.h"

Indicator::~Indicator(){}

Indicator::Indicator(/*Strategy* strategyPtr*/):DataSubscriber()//,_strategyWPtr(strategyPtr)
{
    initialize();
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}

//Indicator::Indicator(Strategy* strategyPtr):DataSubscriber(), _strategyWPtr(strategyPtr)
//{
//    initialize();
//    QThread* thread = ThreadManager::threadManager().requestThread();
//    moveToThread(thread);
//    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
//}


void Indicator::initialize()
{
    _numInstruments = 0;
    _timerId = -1;
}

void Indicator::setIndicatorTimer()
{
    _timerId = startTimer(_timeScale*60000);
    //this is first calculate
    calculate();
}

void Indicator::startIndicator()
{}

void Indicator::stopIndicator()
{
    killTimer(_timerId);
}

void Indicator::calculate()
{}

const double Indicator::getValue()
{
    return _value;
}

const String& Indicator::getName()
{
    return _name;
}
