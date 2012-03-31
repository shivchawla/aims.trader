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

Trader::Trader():AdapterWrapper()
{
    _traderAssistantPtr = new TraderAssistant(this);
}

Trader::~Trader()
{
    delete _traderAssistantPtr;
}


void Trader:: updateAccountValue(const IBString& key, const IBString& val,
						const IBString& currency, const IBString& accountName)
{}

void Trader:: execDetails(int reqId, const Contract& contract, const Execution& execution) 
{
	//first based on the request id link the openorder
	//ask trader assistant to get the open order for you
	OrderId orderId = reqId;
    _traderAssistantPtr->updateExecution(orderId, contract, execution);
}

void Trader::orderStatus(const OrderId id, const std::string status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string whyHeld)
{
    //_traderAssistantPtr->orderStatus(id, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld);
}

void Trader::contractDetails(long tickerId, const ContractDetails& contractDetails)
{
	_traderAssistantPtr->updateInstrument(tickerId, contractDetails);
}

void Trader::nextValidId(OrderId orderId) 
{
    //_traderAssistantPtr->setOrderId(orderId);
}

void Trader::tickPrice( TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
    _traderAssistantPtr->updateTickPrice(tickerId, field, price, canAutoExecute);
}

void Trader::tickSize( TickerId tickerId, TickType field, int size)
{
    _traderAssistantPtr->updateTickSize(tickerId, field, size);
}

void Trader::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    _traderAssistantPtr->updateTickGeneric(tickerId, tickType, value);
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

TraderAssistant* Trader::getTraderAssistant()
{
    return _traderAssistantPtr;
}

void Trader::error(const int id, const int errorCode, const IBString errorString)
{
    std::cout<<"ID: "<<id<<", Error Code: "<<errorCode<<", Error: "<<errorString<<"\n";
}






