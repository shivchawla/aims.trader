/*
 *  Trader.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 10/10/11.
 *  Copyright 2011 AIMS inc. All rights reserved.
 *
 */
#pragma once
#ifndef Trader_h
#define Trader_h

#include "Platform/Trader/AdapterWrapper.h"
#include "Platform/PosixSocketClient/EPosixClientSocket.h"
#include "AimsTraderDefs/typedefs.h"
#include <QObject>
class TraderAssistant;

#include <cstdio>
#include <vector>

class Trader: public AdapterWrapper
{
	private:
        TraderAssistant* _traderAssistantPtr;

	public:
		 Trader();
		~Trader();
	
	void updateAccountValue(const IBString& key, const IBString& val,
							const IBString& currency, const IBString& accountName);
	void execDetails(int reqId, const Contract& contract, const Execution& execution);
	
	void nextValidId(const OrderId);
    void orderStatus(const OrderId id, const std::string status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string whyHeld);
    void contractDetails(long tickerId, const ContractDetails& contractDetails);
    void tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
    void tickSize( TickerId tickerId, TickType field, int size);
    void tickGeneric(TickerId tickerId, TickType tickType, double value);
    void error(const int id, const int errorCode, const IBString errorString);
    void Connect();
    void Disconnect();
    bool IsConnected();
    TraderAssistant* getTraderAssistant();

    void reportEvent(const String& message, const MessageType mType = INFO);

};

#endif
