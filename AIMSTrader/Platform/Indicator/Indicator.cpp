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

//Indicator::Indicator():DataSubscriber()
//{}

Indicator::Indicator(/*Strategy* strategyPtr*/):DataSubscriber()//,_strategyWPtr(strategyPtr)
{
    initialize();
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void Indicator::initialize()
{
    _timeScale = 0;
    _numInstruments = 0;
}

void Indicator::startIndicator()
{}

void Indicator::stopIndicator()
{
//    if(QThread::currentThread()->isRunning())
//    {
//        QThread::currentThread()->exit();
//    }
    QThread::currentThread()->quit();
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
