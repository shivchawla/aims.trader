//
//  CommissionFactory.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/25/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef CommissionFactory_h
#define CommissionFactory_h
#include "Platform/Commission/Commission.h"

class CommissionFactory 
{
    private:
        static Commission getCommission(const double rate, const double min);

    public:
        CommissionFactory(){}
        ~CommissionFactory(){}
    /**
     * For commissions and fees, see
     * http://individuals.interactivebrokers.com/en/accounts/fees/commission.php?ib_entity=llc
     *
     * @param rate       Commission per contract or per share
     * @param min        Minimum commission per order
     * @param maxPercent Maximum commission as percent of the trade amount
     * @return Commission for the trade
     */
    static Commission getCommission(const double rate, const double min, const double maxPercent);
    
    public:
        static Commission getBundledNorthAmericaStockCommission(); 
        static Commission getBundledNorthAmericaFutureCommission(); 
        static Commission getForexCommission(); 
    
};
#endif
