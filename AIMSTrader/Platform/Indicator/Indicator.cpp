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

Indicator::Indicator():DataSubscriber()
{}

Indicator::Indicator(Strategy* strategyPtr):DataSubscriber(),_strategyWPtr(strategyPtr)
{
    initialize();
    QThread* thread = ThreadManager::Instance()->requestThread();
    moveToThread(thread);
    //QObject::connect(thread, SIGNAL(started()), this, SLOT(startIndicator()));
    QObject::connect(strategyPtr, SIGNAL(startIndicator()), this, SLOT(startIndicator()));
    QObject::connect(strategyPtr, SIGNAL(stopIndicator()), this, SLOT(stopIndicator()));

}

void Indicator::initialize()
{}

void Indicator::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
{}

void Indicator::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
{}

void Indicator::onTickPriceUpdate(const TickerId, const TickType, const double)
{}

void Indicator::updateOneMinuteSnapShot(const TickerId, const double)
{}

void Indicator::startIndicator()
{}

void Indicator::stopIndicator()
{
    if(QThread::currentThread()->isRunning())
    {
        QThread::currentThread()->exit();
    }
}

void Indicator::calculate()
{}

const double Indicator::getValue()
{
    return _value;
}

const std::string& Indicator::getName()
{
    return _name;
}


//right now we are using strtagey function to place order
//we can also send a signal from indicator to strtagey to place an order
//strategy and it's indicator work on a separate thread
void Indicator::placeOrder(const TickerId tickerId, const Order& order)
{
    _strategyWPtr->placeOrder(tickerId, order);
}

void Indicator::closeAllPositions()
{
    _strategyWPtr->closeAllPositions();
}
