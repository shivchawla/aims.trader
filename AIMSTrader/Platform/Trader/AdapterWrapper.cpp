/*
 *  AdapterWrapper.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Trader/AdapterWrapper.h"

AdapterWrapper::AdapterWrapper() {}
AdapterWrapper::~AdapterWrapper() {}

void AdapterWrapper:: tickPrice( TickerId tickerId, TickType field, double price, int canAutoExecute) {}
void AdapterWrapper:: tickSize( TickerId tickerId, TickType field, int size) {}
void AdapterWrapper:: tickOptionComputation( TickerId tickerId, TickType tickType, double impliedVol, double delta,
									   double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) {}
void AdapterWrapper:: tickGeneric(TickerId tickerId, TickType tickType, double value) {}
void AdapterWrapper:: tickString(TickerId tickerId, TickType tickType, const IBString& value) {}
void AdapterWrapper:: tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const IBString& formattedBasisPoints,
						 double totalDividends, int holdDays, const IBString& futureExpiry, double dividendImpact, double dividendsToExpiry) {}
void AdapterWrapper:: orderStatus( OrderId orderId, const IBString &status, int filled,
							 int remaining, double avgFillPrice, int permId, int parentId,
							 double lastFillPrice, int clientId, const IBString& whyHeld) {}
void AdapterWrapper:: openOrder( OrderId orderId, const Contract&, const Order&, const OrderState&) {}
void AdapterWrapper:: openOrderEnd() {}
void AdapterWrapper:: winError( const IBString &str, int lastError) {}
void AdapterWrapper:: connectionClosed() {}
void AdapterWrapper:: updateAccountValue(const IBString& key, const IBString& val,
									const IBString& currency, const IBString& accountName) {}
void AdapterWrapper:: updatePortfolio( const Contract& contract, int position,
								 double marketPrice, double marketValue, double averageCost,
								 double unrealizedPNL, double realizedPNL, const IBString& accountName) {}
void AdapterWrapper:: updateAccountTime(const IBString& timeStamp) {}
void AdapterWrapper:: accountDownloadEnd(const IBString& accountName) {}
void AdapterWrapper:: nextValidId( OrderId orderId) {}
void AdapterWrapper:: contractDetails( int reqId, const ContractDetails& contractDetails) {}
void AdapterWrapper:: bondContractDetails( int reqId, const ContractDetails& contractDetails) {}
void AdapterWrapper:: contractDetailsEnd( int reqId) {}
void AdapterWrapper:: execDetails( int reqId, const Contract& contract, const Execution& execution) {}
void AdapterWrapper:: execDetailsEnd( int reqId) {}
void AdapterWrapper:: error(const int id, const int errorCode, const IBString errorString) {}
void AdapterWrapper:: updateMktDepth(TickerId id, int position, int operation, int side,
								double price, int size) {}
void AdapterWrapper:: updateMktDepthL2(TickerId id, int position, IBString marketMaker, int operation,
								  int side, double price, int size) {}
void AdapterWrapper:: updateNewsBulletin(int msgId, int msgType, const IBString& newsMessage, const IBString& originExch) {}
void AdapterWrapper:: managedAccounts( const IBString& accountsList) {}
void AdapterWrapper:: receiveFA(faDataType pFaDataType, const IBString& cxml) {}
void AdapterWrapper:: historicalData(TickerId reqId, const IBString& date, double open, double high, 
								double low, double close, int volume, int barCount, double WAP, int hasGaps) {}
void AdapterWrapper:: scannerParameters(const IBString &xml) {}
void AdapterWrapper:: scannerData(int reqId, int rank, const ContractDetails &contractDetails,
							 const IBString &distance, const IBString &benchmark, const IBString &projection,
							 const IBString &legsStr) {}
void AdapterWrapper:: scannerDataEnd(int reqId) {}
void AdapterWrapper:: realtimeBar(TickerId reqId, long time, double open, double high, double low, double close,
							 long volume, double wap, int count) {}
void AdapterWrapper:: currentTime(long time) {}
void AdapterWrapper:: fundamentalData(TickerId reqId, const IBString& data) {}
void AdapterWrapper:: deltaNeutralValidation(int reqId, const UnderComp& underComp) {}
void AdapterWrapper:: tickSnapshotEnd( int reqId) {}


