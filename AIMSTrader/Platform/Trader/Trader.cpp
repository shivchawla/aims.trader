/*
 *  Trader.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 10/10/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Trader/Trader.h"
#include "Platform/Trader/TraderAssistant.h"
#include <iostream>
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Trader/InstrumentManager.h"

Trader::Trader():AdapterWrapper()
{
    //this is still called on main function
    _traderAssistantPtr = new TraderAssistant(this);
}

Trader::~Trader()
{
    //this is problematic if trader assistant
    //has been shifted to different thread
    delete _traderAssistantPtr;
}

void Trader:: updateAccountValue(const IBString& key, const IBString& val,
						const IBString& currency, const IBString& accountName)
{}

void Trader:: execDetails(int reqId, const Contract& contract, const Execution& execution) 
{
	//first based on the request id link the openorder
	//ask trader assistant to get the open order for you
    OrderId orderId = _traderAssistantPtr->getOrderId(reqId);
<<<<<<< HEAD
    Service::Instance()->getOrderManager()->updateOpenOrderOnExecution(orderId, /*contract,*/ execution);
=======
    Service::Instance()->getOrderManager()->updateOpenOrderOnExecution(orderId, contract, execution);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    //_traderAssistantPtr->updateExecution(orderId, contract, execution);
}

void Trader::orderStatus(const OrderId id, const std::string status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string whyHeld)
{
    //_traderAssistantPtr->orderStatus(id, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld);
}

void Trader::contractDetails(long tickerId, const ContractDetails& contractDetails)
{
    //TickerId tickerId = _traderAssistantPtr->getTickerId(tickerId);
    Service::Instance()->getInstrumentManager()->setContractDetails(tickerId, contractDetails);
    //_traderAssistantPtr->updateInstrument(tickerId, contractDetails);
}

void Trader::nextValidId(const OrderId orderId)
{
    _traderAssistantPtr->setRequestId(orderId);
}

void Trader::tickPrice(TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
     Service::Instance()->getInstrumentManager()->tickPrice(tickerId, field, price, canAutoExecute);
    /*switch(field)
    {
        case HIGH: updateHigh(tickerId, price); break;
        case LOW: updateLow(tickerId, price); break;
        case BID: updateBid(tickerId, price); break;
        case ASK: updateAsk(tickerId, price); break;
        case OPEN: updateOpen(tickerId, price); break;
        case CLOSE: updateClose(tickerId, price); break;
        case LAST: updateLast(tickerId, price); break;
    }*/
    //_traderAssistantPtr->updateTickPrice(tickerId, field, price, canAutoExecute);
}

void Trader::tickSize( TickerId tickerId, TickType field, int size)
{
     Service::Instance()->getInstrumentManager()->tickSize(tickerId, field, size);
   /*switch(field)
    {
    case LAST_SIZE: updateLastSize(tickerId, size); break;
    case ASK_SIZE: updateAskSize(tickerId, size); break;
    case BID_SIZE: updateBidSize(tickerId, size); break;
    }*/
    //_traderAssistantPtr->updateTickSize(tickerId, field, size);
}

void Trader::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    Service::Instance()->getInstrumentManager()->tickGeneric(tickerId, tickType, value);
    /*switch(tickType)
    {
        case HIGH: updateHigh(tickerId, value); break;
        case LOW: updateLow(tickerId, value); break;
        case BID: updateBid(tickerId, value); break;
        case ASK: updateAsk(tickerId, value); break;
        case OPEN: updateOpen(tickerId, value); break;
        case CLOSE: updateClose(tickerId, value); break;
        case LAST: updateLast(tickerId, value); break;
        case LAST_SIZE: updateLastSize(tickerId, value); break;
        case ASK_SIZE: updateAskSize(tickerId, value); break;
        case BID_SIZE: updateBidSize(tickerId, value); break;
     }*/
    //_traderAssistantPtr->updateTickGeneric(tickerId, tickType, value);
}

void Trader::Connect()
{
    const char* host = "127.0.01";
    unsigned int port = 7496;
    int clientId = 0;

   _traderAssistantPtr->Connect(host, port, clientId);
}

void Trader::Disconnect()
{
   _traderAssistantPtr->Disconnect();
}

bool Trader::IsConnected()
{
    return _traderAssistantPtr->IsConnected();
}

TraderAssistant* Trader::getTraderAssistant()
{
    return _traderAssistantPtr;
}

void Trader::error(const int id, const int errorCode, const IBString errorString)
{
    String message;
    message.append("ID: ").append(QString::number(id)).append(", Error Code: ").append(QString::number(errorCode));
    message.append(", Error: ").append(QString::fromStdString(errorString));
    //std::cout<<"ID: "<<id<<", Error Code: "<<errorCode<<", Error: "<<errorString<<"\n";
    Service::Instance()->getEventReport()->report("Trader", message);
}

void Trader::reportEvent(const String& message)
{
    Service::Instance()->getEventReport()->report("Trader", message);
}




