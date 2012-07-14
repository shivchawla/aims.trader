/*
 *  TraderAssistant.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Trader/TraderAssistant.h"
#include "Platform/PosixSocketClient/EPosixClientSocket.h"
#include "Platform/Startup/Service.h"
#include  "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Trader/CheckMessageThread.h"
#include <iostream>
#include <QWaitCondition>
#include "Platform/View/OutputInterface.h"
#include <QDateTime>

//constructor
TraderAssistant::TraderAssistant(Trader* traderPtr):_socketPtr(new EPosixClientSocket(traderPtr)),
                                                    _traderPtr(traderPtr)
{
    init();
}

void TraderAssistant::init()
{
    _isIBReady = false;
    _checkMessageThread = new CheckMessageThread(this);
    _nextValidId=0;
}

TraderAssistant::~TraderAssistant()
{
    printf( "Destroying Trader Assistant\n");
    _checkMessageThread->exit();
    delete _checkMessageThread;
}

/*******************************/
//ConnectionManager functions
void TraderAssistant::Connect(const char *host, unsigned int port, int clientID)
{
	//Connecting to IB TWS
    //printf("Connecting to %s:%d clientID:%d\n",!(host && *host)? "127.0.0.1":host, port, clientID);
    String message("Connecting to IB Host: ");
    message.append(QString::fromLatin1(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
    reportEvent(message);

    bool res = _socketPtr->eConnect(host, port, clientID);
	
	if(res)
    {
        //printf("Connected to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        String message("Connected to IB Host: ");
        message.append(QString::fromLatin1(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
        reportEvent(message);

        //once the paltform is ready, check messages is put on a new thread
        _checkMessageThread->start();
        _mutex.lock();
        if(!_isIBReady)
        {
            _condition.wait(&_mutex);
        }
        else
        {
            printf( "Cannot connect to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        }
        _mutex.unlock();

    }
	else
	{
        //printf( "Cannot connect to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        String message("Cannot connect to IB Host: ");
        message.append(QString::fromLatin1(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
        reportEvent(message);
    }
}

void TraderAssistant::Disconnect()
{
    _socketPtr->eDisconnect();
    String message("Disconnected from IB ");
    //message.append(QString::number(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
    reportEvent(message);
}

bool TraderAssistant::IsConnected() const
{
	return _socketPtr->isConnected();
}

void TraderAssistant::getCurrentTime() const
{
    //printf( "Requesting Current Time\n");
	_socketPtr->reqCurrentTime();
}

/*******************************/
//OrderManager functions
//this should be synchronized for multiple threads
void TraderAssistant::placeOrder(const OrderId orderId, const Order& order, const Contract& contract)
{
    //lock the socket for outgoing messages
    _mutex.lock();
    _requestIdToOrderId[_nextValidId] = orderId;
    _socketPtr->placeOrder(_nextValidId++, contract, order);
    _mutex.unlock();
    Service::Instance()->getOrderManager()->updateOrderStatus(orderId, Submitted);
}

const OrderId TraderAssistant::getOrderId(const long requestId)
{
    OrderId orderId;
    _mutex.lock();
    if(_requestIdToOrderId.count(requestId)!=0)
    {
        orderId = _requestIdToOrderId[requestId];
    }
    _mutex.unlock();
    return orderId;
}
const TickerId TraderAssistant::getTickerId(const long requestId)
{
    /*TickerId tickerId;
    mutex.lock();
    if(_requestIdToTickerId.count(requestId)!=0)
    {
        tickerId = _requestIdToTickerId[requestId];
    }
    mutex.unlock();
    return tickerId;*/
}


//this should be synchronized for multiple threads
void TraderAssistant::cancelOrder(const OrderId orderId)
{
    _mutex.lock();
    _socketPtr->cancelOrder(orderId);
    _mutex.unlock();
    Service::Instance()->getOrderManager()->updateOrderStatus(orderId, PendingCancel);
}

void TraderAssistant::setRequestId(const OrderId orderId)
{
    _mutex.lock();
    _isIBReady=true;
    _condition.wakeAll();
    _nextValidId = orderId;
    _mutex.unlock();
}

void TraderAssistant::updateExecution(const OrderId& orderId, const Contract& contract, const Execution& execution)
{}

//this should be synchronized for multiple threads

/**
 * While TWS was disconnected from the IB server, some order executions may have occurred.
 * To detect executions, request them explicitly after the reconnection.
 */
void TraderAssistant::requestExecutions(const OrderId orderId)
{
    ExecutionFilter filter;
    _socketPtr->reqExecutions(orderId ,filter);
}

void TraderAssistant::requestMarketData(const TickerId tickerId, const Contract& contract)
{
    //now request the data corresponding to the ticker/Contract combination
    QString message("Sending Market Data Request to IB TickerId:");
    message.append(QString::number(tickerId)).append(" Symbol:").append(QString::fromStdString(contract.symbol));
    reportEvent(message);

    //int tickType = 165;
    std::cout<<"Sending Market Data Request\n";
    _mutex.lock();
    _socketPtr->reqMktData(tickerId, contract, "", false);
    _mutex.unlock();
}

void TraderAssistant::cancelMarketData(const TickerId tickerId)
{
    QString message("Cancelling Market Data from IB TickerId:");
    message.append(QString::number(tickerId)).append(" Symbol:").append(Service::Instance()->getInstrumentManager()->getInstrumentId(tickerId));
    reportEvent(message);

   _socketPtr->cancelMktData(tickerId);
   Service::Instance()->getInstrumentManager()->mktDataCanceled(tickerId);
}

void TraderAssistant::updateInstrument(const TickerId tickerId, const ContractDetails& contractDetails)
{
     //Service::Instance()->getInstrumentManager()->setContractDetails(tickerId, contractDetails);
}

void TraderAssistant::updateTickPrice(const TickerId tickerId, const TickType field, double price, const int canAutoExecute)
{

}

void TraderAssistant::updateTickSize(const TickerId tickerId, const TickType field, const int size)
{

}

void TraderAssistant::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{

}

void TraderAssistant::printThreadId()
{
    //printf("\nTrader Assistant Thread \t");
    //std::cout<<QThread::currentThreadId();

    /*while(1){
        sleep(2);

    printf("\nTrader Assistant Thread  is Active\t");

    }*/
}

//run this function on a separate thread
void TraderAssistant::checkMessages()
{
    while(_socketPtr->checkMessages())
    {}
}

void TraderAssistant::reportEvent(const String& message, const MessageType mType)
{
   OutputInterface::Instance()->reportEvent("TraderAssistant", message, mType);
}






