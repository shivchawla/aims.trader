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
<<<<<<< HEAD
    nextValidId=0;
    _isIBReady = false;
    //setValidId=false;
    checkMessageThread = new CheckMessageThread(this);
=======
    nextValidID=0;
    //setValidId=false;
    checkMessageThread = new CheckMessageThread();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

     //this thread will constantly poll the socket from new messages
    //this is a part of TA object but runs asynchronously
    //checkMessageThread = new QThread();
    //QObject::connect(checkMessageThread,SIGNAL(started()),this,SLOT(checkMessages()));
    //checkMessageThread->start();
}

TraderAssistant::~TraderAssistant()
{
    printf( "Destroying Trader Assistant\n");
<<<<<<< HEAD
    checkMessageThread->exit();
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    delete checkMessageThread;
    //this->thread()->quit();
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
<<<<<<< HEAD
    {
        //printf("Connected to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        String message("Connected to IB Host: ");
        message.append(QString::fromLatin1(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
        reportEvent(message);
    }
	else
	{
        //printf( "Cannot connect to %s:%d clientId:%d\n", !( host && *host) ? "127.0.0.1" : host, port, clientID);
        String message("Cannot connect to IB Host: ");
        message.append(QString::fromLatin1(host)).append(" Port: ").append(QString::number(port)).append(" ClientID: ").append(QString::number(clientID));
        reportEvent(message);
    }

    //once the paltform is ready, check messages is put on a new thread
    checkMessageThread->start();
    mutex.lock();
    if(!_isIBReady)
    {
        condition.wait(&mutex);
    }
    mutex.unlock();
=======
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

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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
    mutex.lock();
<<<<<<< HEAD
    _requestIdToOrderId[nextValidId] = orderId;
    _socketPtr->placeOrder(nextValidId++, contract, order);
=======
    _requestIdToOrderId[nextValidID] = orderId;
    _socketPtr->placeOrder(nextValidID++, contract, order);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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
<<<<<<< HEAD
    mutex.lock();
    nextValidId = orderId;
    _isIBReady=true;
    condition.wakeAll();
    mutex.unlock();
=======
    nextValidID = orderId;
    condition.wakeAll();

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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
<<<<<<< HEAD
    QString message("Sending Market Data Request to IB TickerId:");
    message.append(QString::number(tickerId)).append(" Symbol:").append(QString::fromStdString(contract.symbol));
    reportEvent(message);

    mutex.lock();
    _socketPtr->reqMktData(tickerId, contract, "", false);
    mutex.unlock();   
=======
    //int tickType = 165;
    std::cout<<"Sending Market Data Request\n";
    mutex.lock();
    _socketPtr->reqMktData(tickerId, contract, "", false);
    mutex.unlock();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void TraderAssistant::cancelMarketData(const TickerId tickerId)
{
    QString message("Cancelling Market Data from IB TickerId:");
    message.append(QString::number(tickerId)).append(" Symbol:").append(Service::Instance()->getInstrumentManager()->getInstrumentId(tickerId));
    reportEvent(message);

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
<<<<<<< HEAD
}

void TraderAssistant::reportEvent(const String& message)
{
    //String reporter("TraderAssisntant");
    Service::Instance()->getEventReport()->report("TraderAssistant", message);
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}






