//
//  Commission.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/25/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef Commission_h
#define Commission_h

#include "AimsTraderDefs/typedefs.h"

class Commission
{
    private:
        double _rate, _minimum, _maximumPercent;

/**
 * For commissions and fees, see
 * http://individuals.interactivebrokers.com/en/accounts/fees/commission.php?ib_entity=llc
 *
 * @param rate           Commission per contract or per share
 * @param minimum        Minimum commission per order
 * @param maximumPercent Maximum commission as percent of the trade amount
 */
    public:
        Commission(){}
        ~Commission();
        Commission(const double rate, const double minimum, const double maximumPercent);
        Commission(const double rate, const double minimum);     
    
    public: 
        const String toString(); 
        const double getCommission(const int contracts, const double price);
};

#endif
