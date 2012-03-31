/*
 *  TraderAssistant.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#ifndef TraderAssistant_h
#define TraderAssistant_h

#include <map>
#include "Platform/Enumerations/OrderStatus.h"
#include "Platform/Trader/Trader.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
#include <string>
#include <QObject>

class EPosixClientSocket;
class Trader;
class ModelViewManager;

typedef std::auto_ptr<EPosixClientSocket> SocketPtr;

class TraderAssistant : public QObject
{
    Q_OBJECT
	private:
		SocketPtr _socketPtr;
    private:
		Trader* _traderPtr;

    public:
		//constructor
		TraderAssistant();
        TraderAssistant(Trader*);
		~TraderAssistant();
    
    private:
        void init();

    //Now what's the job of a Trader Assistant
	//to do things on his behalf
	//to help Trader with a lot of different data element
	/*******************************/
	
	/*******************************/
	//ConnectionManager functions
	public:
		void Connect(const char* host, unsigned int port, int clientID=0);	
		void Disconnect() const;
		bool IsConnected() const;
		void getCurrentTime() const;
	
	/*******************************/
	//OrderManager functions
	public:

		void cancelOrder(const OrderId);

    signals:
        void updateTickerId(const int tickerId);
        void updateBid(const TickerId, const double bid);
        void updateAsk(const TickerId, const double ask);
        void updateLast(const TickerId, const double last);
        void updateHigh(const TickerId, const double high);
        void updateClose(const TickerId, const double close);
        void updateOpen(const TickerId, const double close);
        void updateLow(const TickerId, const double low);
        void updateBidSize(const TickerId, const int size);
        void updateAskSize(const TickerId, const int size);
        void updateLastSize(const TickerId, const int size);

        void updateContractDetails(const TickerId, const ContractDetails&);
        void updateOpenOrder(const OrderId&, const Contract&, const Execution&);
        void requestAddOpenOrder(const Contract& , const Order&);
        void updateOrderStatus(const OrderId, const OrderStatus);
        void mktDataCancelled(const TickerId);

    public slots:
        void placeOrder(const OrderId, const Order&, const Contract&);
        void requestMarketData(const TickerId, const Contract& contract);
        void cancelMarketData(const TickerId);
        void requestExecutions(const OrderId orderId);
        void printThreadId();
        void checkMessages();

    public:
        void updateInstrument(const TickerId, const ContractDetails&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);
        void updateExecution(const OrderId&, const Contract&, const Execution&);
};

#endif





