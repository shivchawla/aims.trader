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
#include "Platform/Position/OpenOrder.h"
#include "Platform/Position/Instrument.h"
#include "Platform/Startup/Service.h"
#include  "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
//#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/CheckMessageThread.h"
#include <iostream>
#include <QWaitCondition>

//constructor
TraderAssistant::TraderAssistant(Trader* traderPtr):_socketPtr(new EPosixClientSocket(traderPtr)),
                                                    _traderPtr(traderPtr)
{
    init();
}

void TraderAssistant::init()
{
    nextValidID=0;
    //setValidId=false;
    checkMessageThread = new CheckMessageThread();

     //this thread will constantly poll the socket from new messages
    //this is a part of TA object but runs asynchronously
    //checkMessageThread = new QThread();
    //QObject::connect(checkMessageThread,SIGNAL(started()),this,SLOT(checkMessages()));
    //checkMessageThread->start();
}

TraderAssistant::~TraderAssistant()
{
    printf( "Destroying Trader Assistant\n");
    delete checkMessageThread;
    //this->thread()->quit();
}

/*******************************/
//ConnectionManager functions
void TraderAssistant::Connect(const char *host, unsigned int port, int clientID)
{
	//Connecting to IB TWS
	printf("Connecting to %s:%d clientID:%d\n",!(host && *host)? "127.0.0.1":host, port, clientID);
	//bool res = _socketPtr->eConnect(host, port, clientID);
	bool res = _socketPtr->eConnect(host, port, clientID);
	
	if(res)
	{ 
        mutex.lock();
        checkMessageThread->start();
		printf("Connected to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        condition.wait(&mutex);
        mutex.unlock();
    }
	else
	{
		printf( "Cannot connect to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
    }

}

void TraderAssistant::Disconnect() const
{
    _socketPtr->eDisconnect();
	printf ( "Disconnected\n");
}

bool TraderAssistant::IsConnected() const
{
	return _socketPtr->isConnected();
}

void TraderAssistant::getCurrentTime() const
{
	printf( "Requesting Current Time\n");
	_socketPtr->reqCurrentTime();
}

/*******************************/
//OrderManager functions
//this should be synchronized for multiple threads
void TraderAssistant::placeOrder(const OrderId orderId, const Order& order, const Contract& contract)
{
    //lock the socket for outgoing messages
    mutex.lock();
    _requestIdToOrderId[nextValidID] = orderId;
    _socketPtr->placeOrder(nextValidID++, contract, order);
    mutex.unlock();
    Service::Instance()->getOrderManager()->updateOrderStatus(orderId, Submitted);
}

const OrderId TraderAssistant::getOrderId(const long requestId)
{
    OrderId orderId;
    mutex.lock();
    if(_requestIdToOrderId.count(requestId)!=0)
    {
        orderId = _requestIdToOrderId[requestId];
    }
    mutex.unlock();
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
    mutex.lock();
    _socketPtr->cancelOrder(orderId);
    mutex.unlock();
    Service::Instance()->getOrderManager()->updateOrderStatus(orderId, PendingCancel);
}

void TraderAssistant::setRequestId(const OrderId orderId)
{
    nextValidID = orderId;
    condition.wakeAll();

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
    //int tickType = 165;
    std::cout<<"Sending Market Data Request\n";
    mutex.lock();
    _socketPtr->reqMktData(tickerId, contract, "", false);
    mutex.unlock();
}

void TraderAssistant::cancelMarketData(const TickerId tickerId)
{
   _socketPtr->cancelMktData(tickerId);
   Service::Instance()->getInstrumentManager()->mktDataCancelled(tickerId);
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
    //for(;;){
    while(_socketPtr->checkMessages())
    {}
    //}
}






