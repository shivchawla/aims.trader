//
//  CommissionFactory.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/25/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Commission/CommissionFactory.h"

Commission CommissionFactory::getCommission(const double rate, const double min)
{
    return Commission(rate, min, 0);
}
    
Commission CommissionFactory::getCommission(const double rate, const double min, const double costPerContract, const double maxPercent)
{
    return Commission(rate, min, costPerContract, maxPercent);
}

Commission CommissionFactory::getNorthAmericaStockCommission()
{
    return Commission(0, 1.3, 0.013, 0.005);
}
    
Commission CommissionFactory::getBundledNorthAmericaFutureCommission()
{
    getCommission(2.4, 2.4);
}
    
Commission CommissionFactory::getForexCommission()
{
    getCommission(0.00002, 2.5);
}
    
