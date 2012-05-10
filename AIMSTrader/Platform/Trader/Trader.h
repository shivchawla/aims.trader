/*
 *  Trader.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 10/10/11.
 *  Copyright 2011 AIMS inc. All rights reserved.
 *
 */

#ifndef Trader_h
#define Trader_h

#include "Platform/Trader/AdapterWrapper.h"
#include "Platform/PosixSocketClient/EPosixClientSocket.h"
#include "Platform/Enumerations/ServerJob.h"
#include "Platform/typedefs.h"
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

<<<<<<< HEAD
    void reportEvent(const String& message);
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

};

#endif
