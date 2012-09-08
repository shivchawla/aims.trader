//
//  Commission.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/25/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Commission/Commission.h"
#include <algorithm>

Commission::Commission(const double rate, const double minimum, const double costPerContract, const double maximumPercent)
{
    _rate = rate;
    _minimum = minimum;
    _costPerContract = costPerContract;
    _maximumPercent = maximumPercent;
}

Commission::Commission(const double rate, const double minimum, const double costPerContract)
{
    Commission(rate, minimum, costPerContract, 0);
}

Commission::~Commission()
{}

Commission::Commission(const Commission& commission)
{
    _rate = commission._rate;
    _minimum = commission._minimum;
    _costPerContract = commission._costPerContract;
    _maximumPercent = commission._maximumPercent;
}
            
const double Commission::getCommission(const int contracts, const double price, const CommissionType type)
{
    double commission=0;
    switch(type)
    {
        case ValueBased:
        {
            commission = _rate * price * contracts;
            if (_maximumPercent > 0)
            {
                double maximumCommission = contracts * price * _maximumPercent;
                commission = std::min(commission, maximumCommission);
            }

            commission = std::max(commission, _minimum);
            break;
       }

       case PriceBased:
       {
            commission = _costPerContract * contracts;
            if (_maximumPercent > 0)
            {
                double maximumCommission = contracts * price * _maximumPercent;
                commission = std::min(commission, maximumCommission);
            }

            commission = std::max(commission, _minimum);
            break;
       }
    }

    return commission;
}

