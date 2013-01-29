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
#include "Enumerations/Mode.h"
#include <QString>
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Order.h"
#include <QDateTime>
#include "data/positiondata.h"
#include <math.h>

typedef long StrategyId;
typedef long PositionId;
typedef long SpreadId;
typedef QString String;
typedef uint IntrumentId;
typedef uint DbStrategyId;
class StrategyLinkedPositionData;

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

enum StrategyType
{
    SingleStock_StrategyType,
    Spread_StrategyType,
    Portfolio_StrategyType
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
    double totalBought, totalSold, runningGrossPnL, netPnL;
    double peakNetProfit, maxDrawdown, drawDown, profitFactor;
    double sumTradeProfit, sumTradeProfitSquared;
    long longTrades, shortTrades;
    double totalCommission;
    double realizedGrossPnL;
    double totalProfit;
    double totalLoss;

    PerformanceStats()
    {
        trades = profitableTrades = 0;
        totalBought = totalSold = runningGrossPnL = netPnL = 0;
        peakNetProfit = maxDrawdown = drawDown = profitFactor = 0;
        sumTradeProfit = sumTradeProfitSquared = 0;
        longTrades = shortTrades = 0;
        totalCommission = 0;
        realizedGrossPnL = 0;
        totalProfit = 0;
        totalLoss = 0;
    }

    PerformanceStats(const PerformanceStats& stats)
    {
        trades = stats.trades;
        profitableTrades = stats.profitableTrades;
        totalBought = stats.totalBought;
        totalSold = stats.totalSold;
        runningGrossPnL = stats.runningGrossPnL;
        netPnL = stats.netPnL;
        peakNetProfit = stats.peakNetProfit;
        maxDrawdown = stats.maxDrawdown;
        drawDown = stats.drawDown;
        profitFactor = stats.profitFactor;
        sumTradeProfit = stats.sumTradeProfit;
        sumTradeProfitSquared = stats.sumTradeProfitSquared;
        longTrades = stats.longTrades;
        shortTrades = stats.shortTrades;
        totalCommission = stats.totalCommission;
        realizedGrossPnL = stats.realizedGrossPnL;
        totalProfit = stats.totalProfit;
        totalLoss = stats.totalLoss;
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
    DEBUG,
    CRITICAL,
    WARNING
};

struct Prices
{
    double last;
    double bid;
    double ask;
};

inline const String getMessageType(const MessageType type)
{
    switch(type)
    {
        case INFO: return "INFO"; break;
        case DEBUG: return "DEBUG"; break;
        case CRITICAL: return "CRITICAL"; break;
        case WARNING: return "WARNING"; break;

        default: return "INFO";break;
    }
}


enum OutputType
{
    DB,
    GUI,
    ALL
};

enum CommissionType
{
    ValueBased=0,
    PriceBased
};


enum PositionStatus
{
    Open,
    Close,
    PendingClose
};

class PositionDetail
{
    friend class Position;
    public:
        //TickerId _tickerId;
        long sharesBought;
        long latestLegSharesBought;

        long sharesSold;
        long latestLegSharesSold;

        double avgBuyPrice;
        double latestLegBuyPrice;

        double avgSellPrice;
        double latestLegSellPrice;

        double totalValueBought;
        double totalValueSold;
        double netValue;

        double totalCommission;
        double latestLegOrderCommission;

        double markedPrice;

        double transitAvgBuyPrice;
        double transitAvgSellPrice;

        double realizedProfit;
        double realizedLoss;
        double runningProfit;
        double runningLoss;
        double drawDown;
        double peakPositionValue;

        QDateTime createdTime;
        OrderId latestLegOrderId;
        bool isNewLeg;

    //indicator.i=0 if net pnl is same direction
    int indicator;

    public:
        PositionDetail();
        //PositionDetail(const PositionData&);
        PositionDetail(const PositionDetail&);

    public:
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double commission);
        void update(const OrderId, const Execution&, const double);
        void reset();

    public:
//        const long getSharesBought() const{return _sharesBought;}
//        const long getSharesSold() const{return _sharesSold;}
//        const double getAvgBought() const {return _avgBuyPrice; }
//        const double getAvgSold() const{return _avgSellPrice;}
//        const double getTotalValueBought() const{return _totalValueBought;}
//        const double getTotalValueSold() const{return _totalValueSold;}
//        const double getNetValue() const
//        {
//            return _netValue;
//        }
//        const double getTotalCommission() const{return _totalCommission;}
        const double getRealizedPnl() const{return realizedProfit - realizedLoss;}
        const double getRunningPnl() const{return runningProfit - runningLoss;}
        const double getNetPnL() const{return getGrossPnL() - totalCommission;}
        const double getGrossPnL() const{return getRealizedPnl() + getRunningPnl();}
        const double getNetTotalIncCommission() const{return totalValueSold-totalValueBought-totalCommission;}
        const double getMarkedPrice() const { return markedPrice;}
        const int getIndicator() const {return indicator;}
        const double IsNewLeg() const {return isNewLeg;}

        const double getReturn() const
        {
            double absNetValue = abs(netValue);
            return (absNetValue > 0 ) ? getRunningPnl() * 100/absNetValue : 0;
        }

        const long getNetShares() const {return sharesBought - sharesSold;}

        const double getTotalProfit() const {return realizedProfit+runningProfit;}
        const double getTotalLoss() const {return realizedLoss+runningLoss;}

//        const long getLatestLegQuantityBought() const {return _latestLegSharesBought;}
//        const long getLatestLegQuantitySold() const {return _latestLegSharesSold;}
//        const double getLatestLegBuyPrice() const {return _latestLegBuyPrice;}
//        const double getLatestLegSellPrice() const {return _latestLegSellPrice;}
//        const double getLatestLegCommission() const {return _latestLegOrderCommission;}
//        const double _IsNewLeg() const {return _isNewLeg;}
//        const QDateTime getCreatedTime() const {return _createdTime;}
};


//can make al the members of orderDetail public and remove access functions
//like a structure
struct OrderDetail
{
        Order order;
        long filledShares;
        long lastFilledShares;
        long pendingShares;
        double avgFillPrice;
        double lastFillPrice;
        OrderStatus status;
        bool isClosingOrder;
        QDateTime placedTime;
        QDateTime lastUpdatedTime;
        Contract contract;
        double commission;
        //TickerId tickerId;
        StrategyId strategyId;
        bool isSpreadOrder;

        OrderDetail();
        OrderDetail(const OrderDetail&);
        OrderDetail(const StrategyId, const Order&, const Contract&);
        OrderDetail(const StrategyId, const Order&);

    public:
        void update(const Execution&);

//    public:
//        const Order& getOrder() const {return order;}
//        const Contract& getContract() const {return contract;}
//        const OrderStatus getOrderStatus() const {return status;}
//        const long getFilledShares() const {return filledShares;}
//        const long getPendingShares() const {return pendingShares;}
//        const double getAvgFillPrice() const {return avgFillPrice;}
//        const double getLastFillPrice() const {return lastFillPrice;}
//        const long getLastFilledShares() const {return lastFilledShares;}
//        const bool IsClosingOrder() const{ return isClosingOrder; }
//        const QDateTime getPlacedTime() const {return placedTime;}
//        const QDateTime getLastUpdatedTime() const {return lastUpdatedTime;}
//        const double getCommission() const { return  commission;}
//        const StrategyId getStrategyId() const {return strategyId;}
//        const TickerId getTickerId() const {return tickerId;}
//        const String getOrderAction() const {return QString::fromStdString(order.action);}
};


class SpreadDetail
{
    public:
        SpreadId spreadId;
        double totalValueBought;
        double totalValueSold;
        double netValue;
        double totalCommission;
        double realizedProfit;
        double realizedLoss;
        double runningProfit;
        double runningLoss;
        double drawDown;
        double peakValue;
        QDateTime createdTime;
        QDateTime updatedTime;

    public:
        SpreadDetail(const SpreadDetail&);
        SpreadDetail();
};



#endif typedefs_h

