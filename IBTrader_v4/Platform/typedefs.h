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
typedef long StrategyId;
typedef std::string String;
#include "ActiveTickFeed/Shared/ATServerAPIDefines.h"

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

    String symbol;
    String Exchange;

    double lastPrice;
    uint32_t lastSize;
};

struct QuoteUpdate
{
    DateTime dateTime;

    String symbol;
    String Exchange;

    double bidPrice;
    uint32_t bidSize;

    double askPrice;
    uint32_t askSize;
};

#endif typedefs_h

