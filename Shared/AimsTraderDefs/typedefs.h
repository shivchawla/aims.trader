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
#include "InteractiveBroker/Shared/Order.h"
#include <QDateTime>

typedef long StrategyId;
typedef long PositionId;
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
    ERROR
};

struct Prices
{
    double last;
    double bid;
    double ask;
};

inline QString getMessageType(const MessageType type)
{
    switch(type)
    {
        case INFO: return "INFO"; break;
        case ERROR: return "ERROR"; break;
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
    private:
    long _sharesBought;
    long _latestLegSharesBought;

    long _sharesSold;
    long _latestLegSharesSold;

    double _avgBought;
    double _latestLegBuyPrice;

    double _avgSold;
    double _latestLegSellPrice;

    double _totalValueBought;
    double _totalValueSold;

    double _totalCommission;
    double _latestLegOrderCommission;

    //double _realizedPnl;
    //double _runningPnl;
    //double _netPnl;
    //double _grossPnl;
    double _markedPrice;

    double _transitAvgBuyPrice;
    double _transitAvgSellPrice;

    double _realizedProfit;
    double _realizedLoss;
    double _runningProfit;
    double _runningLoss;
    //double _profit;
    //double _loss;

    QDateTime _createdTime;
    OrderId _latestLegOrderId;
    bool _isNewLeg;

    //indicator.i=0 if net pnl is same direction
    int _indicator;

    public:
        PositionDetail();
        PositionDetail(const StrategyLinkedPositionData*);
        PositionDetail(const PositionDetail&);

    public:
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double commission);
        void update(const OrderId, const Execution&, const double);
        void reset();
//        void swap(const PositionDetail&);

//    private:
//        void swap(PositionDetail**, PositionDetail**);


    public:
        //void setRunningPnL(const double runningPnl) {_runningPnl = runningPnl;}
        //void setPnl(const double pnl){_netPnl = pnl;}

        const long getSharesBought() const{return _sharesBought;}
        const long getSharesSold() const{return _sharesSold;}
        const double getAvgBought() const {return _avgBought; }
        const double getAvgSold() const{return _avgSold;}
        const double getTotalValueBought() const{return _totalValueBought;}
        const double getTotalValueSold() const{return _totalValueSold;}
        const double getNetValue() const
        {
            long x;
            return ((x =_sharesBought-_sharesSold)>0) ? x*_avgBought : -x*_avgSold;
        }
        const double getTotalCommission() const{return _totalCommission;}
        const double getRealizedPnl() const{return _realizedProfit - _realizedLoss;}
        const double getRunningPnl() const{return _runningProfit - _runningLoss;}
        const double getNetPnL() const{return getGrossPnL() - _totalCommission;}
        const double getGrossPnL() const{return getRealizedPnl() + getRunningPnl();}
        const double getNetTotalIncCommission() const{return _totalValueSold-_totalValueBought-_totalCommission;}
        const double getMarkedPrice() const { return _markedPrice;}
        const int getIndicator() const {return _indicator;}

        const double getReturn() const
        {
            return getRunningPnl() * 100/abs(_totalValueBought - _totalValueSold);
        }

        const long getNetShares() const {return _sharesBought - _sharesSold;}

        const double getTotalProfit() const {return _realizedProfit+_runningProfit;}
        const double getTotalLoss() const {return _realizedLoss+_runningLoss;}

        const long getLatestLegQuantityBought() const {return _latestLegSharesBought;}
        const long getLatestLegQuantitySold() const {return _latestLegSharesSold;}
        const double getLatestLegBuyPrice() const {return _latestLegBuyPrice;}
        const double getLatestLegSellPrice() const {return _latestLegSellPrice;}
        const double getLatestLegCommission() const {return _latestLegOrderCommission;}
        const double IsNewLeg() const {return _isNewLeg;}
        const QDateTime getCreatedTime() const {return _createdTime;}
};

class OrderDetail
{
    private:
        Order _order;
        long _filledShares;
        long _lastFilledShares;
        long _pendingShares;
        double _avgFillPrice;
        double _lastFillPrice;
        OrderStatus _status;
        bool _isClosingOrder;
        QDateTime _placedTime;
        QDateTime _lastUpdatedTime;
        Contract _contract;
        double _commission;
        TickerId _tickerId;
        StrategyId _strategyId;

    public:
        OrderDetail();
        OrderDetail(const OrderDetail&);
        OrderDetail(const TickerId, const StrategyId, const Order&, const Contract&);
        OrderDetail(const TickerId, const StrategyId, const Order&);

    public:
        void update(const Execution&);

    public:
        const Order& getOrder() const {return _order;}
        const Contract& getContract() const {return _contract;}
        const OrderStatus getOrderStatus() const {return _status;}
        const long getFilledShares() const {return _filledShares;}
        const long getPendingShares() const {return _pendingShares;}
        const double getAvgFillPrice() const {return _avgFillPrice;}
        const double getLastFillPrice() const {return _lastFillPrice;}
        const long getLastFilledShares() const {return _lastFilledShares;}
        const bool IsClosingOrder() const{ return _isClosingOrder; }
        const QDateTime getPlacedTime() const {return _placedTime;}
        const QDateTime getLastUpdatedTime() const {return _lastUpdatedTime;}
        const double getCommission() const { return  _commission;}
        const StrategyId getStrategyId() const {return _strategyId;}
        const TickerId getTickerId() const {return _tickerId;}
};



#endif typedefs_h

