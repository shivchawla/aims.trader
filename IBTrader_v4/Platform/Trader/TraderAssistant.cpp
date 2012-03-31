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
#include "Platform/Utils/ThreadManager.h"
#include <iostream>
//constructor
TraderAssistant::TraderAssistant(Trader* traderPtr):_socketPtr(new EPosixClientSocket(traderPtr)),
                                                    _traderPtr(traderPtr)
{
    printf( "Creating Trader Assistant\n");
    init();
    QThread* thread = ThreadManager::Instance()->requestThread();
    connect(thread,SIGNAL(started()),this, SLOT(checkMessages()));
    moveToThread(thread);
}

void TraderAssistant::init()
{}

TraderAssistant::~TraderAssistant()
{
    printf( "Destroying Trader Assistant\n");
    this->thread()->quit();
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
		printf("Connected to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
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
    _socketPtr->placeOrder(orderId, contract, order);
    emit updateOrderStatus(orderId, PendingSubmit);
}

//this should be synchronized for multiple threads
void TraderAssistant::cancelOrder(const OrderId orderId)
{
    emit updateOrderStatus(orderId, PendingCancel);
    _socketPtr->cancelOrder(orderId);
}

//this should be synchronized for multiple threads
void TraderAssistant::updateExecution(const OrderId& orderId, const Contract& contract, const Execution& execution)
{
    emit updateOpenOrder(orderId, contract, execution);
}

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
    _socketPtr->reqMktData(tickerId, contract, "", false);
}

void TraderAssistant::cancelMarketData(const TickerId tickerId)
{
   _socketPtr->cancelMktData(tickerId);
   emit mktDataCancelled(tickerId);
}

void TraderAssistant::updateInstrument(const TickerId tickerId, const ContractDetails& contractDetails)
{
     emit updateContractDetails(tickerId, contractDetails);
}

void TraderAssistant::updateTickPrice(const TickerId tickerId, const TickType field, double price, const int canAutoExecute)
{
    switch(field)
    {
        case HIGH: emit updateHigh(tickerId, price); break;
        case LOW: emit updateLow(tickerId, price); break;
        case BID: emit updateBid(tickerId, price); break;
        case ASK: emit updateAsk(tickerId, price); break;
        case OPEN: emit updateOpen(tickerId, price); break;
        case CLOSE: emit updateClose(tickerId, price); break;
        case LAST: emit updateLast(tickerId, price); break;
    }
}

void TraderAssistant::updateTickSize(const TickerId tickerId, const TickType field, const int size)
{
    switch(field)
    {
    case LAST_SIZE: emit updateLastSize(tickerId, size); break;
    case ASK_SIZE: emit updateAskSize(tickerId, size); break;
    case BID_SIZE: emit updateBidSize(tickerId, size); break;
    }
}

void TraderAssistant::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    switch(tickType)
    {
        case HIGH: emit updateHigh(tickerId, value); break;
        case LOW: emit updateLow(tickerId, value); break;
        case BID: emit updateBid(tickerId, value); break;
        case ASK: emit updateAsk(tickerId, value); break;
        case OPEN: emit updateOpen(tickerId, value); break;
        case CLOSE: emit updateClose(tickerId, value); break;
        case LAST: emit updateLast(tickerId, value); break;
        case LAST_SIZE: emit updateLastSize(tickerId, value); break;
        case ASK_SIZE: emit updateAskSize(tickerId, value); break;
        case BID_SIZE: emit updateBidSize(tickerId, value); break;
     }
}

void TraderAssistant::printThreadId()
{
    printf("\nTrader Assistant Thread \t");
    std::cout<<QThread::currentThreadId();

    /*while(1){
        sleep(2);

    printf("\nTrader Assistant Thread  is Active\t");

    }*/
}

//run this function on a separate thread
void TraderAssistant::checkMessages()
{
    /*for(;;){
    while(_socketPtr->checkMessages())
    {}
    }*/
}






