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

Commission::Commission(const double rate, const double minimum, const double maximumPercent):_rate(rate)
                                                                                            ,_minimum(minimum)
                                                                                            ,_maximumPercent(maximumPercent)
{}

Commission::Commission(const double rate, const double minimum)
{
    Commission(rate, minimum, 0);
}

Commission::~Commission()
{}
            
const double Commission::getCommission(const int contracts, const double price)
{
    double commission = _rate * contracts;
    if (_maximumPercent > 0) 
    {
        double maximumCommission = contracts * price * _maximumPercent;
        commission = std::min(commission, maximumCommission);
    }
    
    commission = std::max(commission, _minimum);
    return commission;
}

