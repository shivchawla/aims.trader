/*
 *  typedefs.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/27/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef typedefs_h
#define typedefs_h

#include <string>
#include "InteractiveBroker/Shared/CommonDefs.h"
#include "InteractiveBroker/Shared/Execution.h"
#include "Enumerations/OrderStatus.h"
#include "Enumerations/OrderType.h"
#include "Enumerations/OrderSide.h"
#include "Enumerations/DataSource.h"
#include "Enumerations/TickType.h"
#include <QString>
#include "InteractiveBroker/Shared/Contract.h"
#include <QUuid>

typedef long StrategyId;
typedef long PositionId;
typedef QString String;

//struct DateTime
//{
//    uint16_t wYear;
//    uint16_t wMonth;
//    uint16_t wDayOfWeek;
//    uint16_t wDay;
//    uint16_t wHour;
//    uint16_t wMinute;
//    uint16_t wSecond;
//    uint16_t wMilliseconds;

//    DateTime()
//    {
//        wYear=0;
//        wMonth=0;
//        wDayOfWeek=0;
//        wDay=0;
//        wHour=0;
//        wMinute=0;
//        wSecond=0;
//        wMilliseconds=0;
//    }

//    DateTime(ATTIME& attime)
//    {
//        wYear = attime.wYear;
//        wMonth = attime.wMonth;
//        wDayOfWeek = attime.wDayOfWeek;
//        wDay = attime.wDay;
//        wHour = attime.wHour;
//        wMinute = attime.wMinute;
//        wSecond = attime.wSecond;
//        wMilliseconds = attime.wMilliseconds;
//    }
//};

//struct TradeUpdate
//{
//    DateTime dateTime;

//    double lastPrice;
//    uint32_t lastSize;
//};

//struct QuoteUpdate
//{
//    DateTime dateTime;

//    double bidPrice;
//    uint32_t bidSize;

//    double askPrice;
//    uint32_t askSize;
//};

struct ExecutionStatus
{
    Execution execution;
    OrderStatus orderStatus;

//    const char* orderStatusToStr() const
//    {
//        switch(orderStatus)
//        {
//            case PendingSubmit: return "PendingSubmit"; break;
//            case PendingCancel: return "PendingCancel";break;
//            case PreSubmitted: return "PreSubmitted";break;
//            case Submitted: return "Submitted";break;
//            case Canceled: return "Canceled";break;
//            case FullyFilled: return "FullyFilled";break;
//            case Inactive: return "InActive"; break;
//            case PartiallyFilled: return "PartiallyFilled";break;
//            case ApiPending: return "ApiPending";break;
//            case ApiCanceled:return "ApiCancelled";break;
//        }
//    }

};

enum DataRequestType
{
    RealTime,
    Snapshot
};

struct ReturnSnapshot
{
    double returns;
    TickerId tickerId;

    ReturnSnapshot()
    {
        returns = 0;
        tickerId = 0;
    }

    bool operator < (const ReturnSnapshot& b) const
    {
        return (returns < b.returns);
    }
};


struct PerformanceStats{

    int trades, profitableTrades;
    double totalBought, totalSold, unRealizedGrossPnL, netPnL;
    double peakNetProfit, maxDrawdown, drawDown, profitFactor;
    double sumTradeProfit, sumTradeProfitSquared;
    long longTrades, shortTrades;
    double totalCommission;
    double realizedGrossPnL;

    PerformanceStats()
    {
        trades = profitableTrades = 0;
        totalBought = totalSold = unRealizedGrossPnL = netPnL = 0;
        peakNetProfit = maxDrawdown = drawDown = profitFactor = 0;
        sumTradeProfit = sumTradeProfitSquared = 0;
        longTrades = shortTrades = 0;
        totalCommission = 0;
        realizedGrossPnL = 0;
    }
};

struct HtmlTags{
static String FIELD_START;
static String FIELD_END;
static String HEADER_START;
static String HEADER_END;
static String ROW_START;
static String ROW_END;
static String FIELD_BREAK;
};

inline const QString getOrderStatusString(OrderStatus status)
{
    switch(status)
    {
        case None: return "None"; break;
        case PendingSubmit:return "PendingSubmit"; break;
        case PendingCancel: return "PendingCancel";break;
        case PreSubmitted: return "PreSubmitted";break;
        case Submitted: return "Submitted";break;
        case Canceled: return "Canceled";break;
        case FullyFilled: return "FullyFilled";break;
        case Inactive: return "InActive";break;
        case PartiallyFilled: return "PartiallyFilled";break;
        case ApiPending: return "ApiPending";break;
        case ApiCanceled: return "ApiCanceled";break;
    }
}

inline const QString getOrderTypeString(OrderType type)
{
    switch(type)
    {
        case MKT: return "MKT"; break;
        case MKTCLS: return "MKTCLS"; break;
        case LMT: return "LMT"; break;
        case LMTCLS: return "LMTCLS"; break;
        case PEGMKT: return "PEGMKT"; break;
        case SCALE: return "SCALE"; break;
        case STP: return "STP"; break;
        case STPLMT: return "STPLMT"; break;
        case TRAIL: return "TRAIL"; break;
        case REL: return "REL"; break;
        case VWAP: return "VWAP"; break;
        case TRAILLIMIT: return "TRAILLIMIT"; break;
    }
}

inline const QString getOrderSideString(OrderSide side)
{
    switch(side)
    {
        case BUY: return "BUY"; break;
        case SELL: return "SELL"; break;
        case SHORTSELL: return "SHORTSELL";break;
    }
}

typedef uint ContractId;

struct ATContract
{
    Contract contract;
    ContractId contractId;
};

enum MessageType
{
    INFO=0,
    ERROR
};

#endif typedefs_h

