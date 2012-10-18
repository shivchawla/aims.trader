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
#include "Platform/Strategy/Strategy.h"

Indicator::~Indicator(){}

Indicator::Indicator(/*Strategy* strategyPtr*/):DataSubscriber()//,_strategyWPtr(strategyPtr)
{
    initialize();
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void Indicator::initialize()
{
    //_timeScale = _strategyWPtr->_timeScale;
    _numInstruments = 0;
    //_running = false;
    _timerId = -1;
}

void Indicator::startIndicator()
{
    //_running = true;
}

void Indicator::stopIndicator()
{
//    if(QThread::currentThread()->isRunning())
//    {
//        QThread::currentThread()->exit();
//    }
    //_running = false;
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

//right now we are using strtagey function to place order
//we can also send a signal from indicator to strtagey to place an order
//strategy and it's indicator work on a separate thread
//void Indicator::placeOrder(const InstrumentId instrumentId, const Order& order)
//{
//    _strategyWPtr->placeOrder(instrumentId, order);
//}
