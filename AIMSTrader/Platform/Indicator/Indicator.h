/*
 *  Indicator.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef Indicator_h
#define Indicator_h

#include <string>
/*
This is a base class for all indicators. 
*/
#include "Platform/Utils/DataSubscriber.h"
class Strategy;

class Indicator: public DataSubscriber
{
    Q_OBJECT
	public:
        Indicator(Strategy*);
        Indicator();
        virtual ~Indicator();
    
    private: 
        std::string _name;
        double _value;

    protected://owner
        Strategy* _strategyWPtr;

    public slots:
        void onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate);
        void onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate);
        void onTickPriceUpdate(const TickerId, const TickType, const double);
        void updateOneMinuteSnapShot(const TickerId, const double);

    public slots:
        virtual void startIndicator();
        virtual void stopIndicator();

    private:
        virtual void calculate();

        /*virtual*/ void reset();

    private:
        void initialize();

    protected:
        void placeOrder(const TickerId, const Order&);

    signals:
        void closeAllPositions();

    public:
        const double getValue();
        const std::string& getName();


};

#endif
