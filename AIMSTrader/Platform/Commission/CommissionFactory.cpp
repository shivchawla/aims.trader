//
//  CommissionFactory.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/25/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Commission/CommissionFactory.h"

Commission getCommission(const double rate, const double min)
{
    return Commission(rate, min);
}
    
Commission getCommission(const double rate, const double min, const double maxPercent)
{
    return Commission(rate, min, maxPercent);
}

Commission getBundledNorthAmericaStockCommission()
{
    getCommission(0.005, 1, 0.005);
}
    
Commission getBundledNorthAmericaFutureCommission()
{
    getCommission(2.4, 2.4);
}
    
Commission getForexCommission()
{
    getCommission(0.00002, 2.5);
}
    
