/*
 *  typedefs.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/27/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef typedefs_h
#define typedefs_h

#include <string>
#include "Platform/Shared/CommonDefs.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Enumerations/OrderStatus.h"
#include "ActiveTickFeed/Shared/ATServerAPIDefines.h"
<<<<<<< HEAD
#include <QString>

typedef long StrategyId;
typedef long PositionId;
//typedef std::string String;
typedef QString String;
=======

typedef long StrategyId;
typedef std::string String;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

struct DateTime
{
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDayOfWeek;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
    uint16_t wMilliseconds;

    DateTime()
    {
        wYear=0;
        wMonth=0;
        wDayOfWeek=0;
        wDay=0;
        wHour=0;
        wMinute=0;
        wSecond=0;
        wMilliseconds=0;
    }

    DateTime(ATTIME& attime)
    {
        wYear = attime.wYear;
        wMonth = attime.wMonth;
        wDayOfWeek = attime.wDayOfWeek;
        wDay = attime.wDay;
        wHour = attime.wHour;
        wMinute = attime.wMinute;
        wSecond = attime.wSecond;
        wMilliseconds = attime.wMilliseconds;
    }
};

struct TradeUpdate
{
    DateTime dateTime;

    double lastPrice;
    uint32_t lastSize;
};

struct QuoteUpdate
{
    DateTime dateTime;

    double bidPrice;
    uint32_t bidSize;

    double askPrice;
    uint32_t askSize;
};

struct ExecutionStatus
{
    Execution execution;
    OrderStatus orderStatus;

    const char* orderStatusToStr() const
    {
        switch(orderStatus)
        {
            case PendingSubmit: return "PendingSubmit"; break;
            case PendingCancel: return "PendingCancel";break;
            case PreSubmitted: return "PreSubmitted";break;
            case Submitted: return "Submitted";break;
            case Cancelled: return "Cancelled";break;
            case FullyFilled: return "FullyFilled";break;
            case Inactive: return "InActive"; break;
            case PartiallyFilled: return "PartiallyFilled";break;
            case ApiPending: return "ApiPending";break;
            case ApiCancelled:return "ApiCancelled";break;
        }
    }

};

<<<<<<< HEAD
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


=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

#endif typedefs_h

