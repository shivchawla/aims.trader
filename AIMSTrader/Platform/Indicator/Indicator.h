/*
 *  Indicator.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef Indicator_h
#define Indicator_h

#include <string>
#include "Platform/Utils/DataSubscriber.h"
#include <vector>
class Strategy;

/**
  *Indicator Class
  *This is a base class for all indicators.
  *Indicator objects run in their own independent thread.
  */
class Indicator: public DataSubscriber
{
    Q_OBJECT
	public:
        /**
          *Constructor
          * This constructor takes the underlying strategy object as an argument
          */
        //Indicator();

        /**
          *Constructor
          */
        Indicator();

        //Indicator(Strategy* );

        /**
          *Virtual Destructor
          */
        virtual ~Indicator();
    
    protected:
        int _numInstruments;
        String _name; /** <String  Strategy Name */
        double _value; /** <double Indicator Value*/
        int _timeScale;
        int _timerId;
        bool _isIndicatorActive;

    protected:
        /**
          * Underlying strategy parent*/
        Strategy* _strategyWPtr;
        //bool _running;

    public slots:
        virtual void startIndicator();

        virtual void stopIndicator();
        void onTickPriceUpdate(const TickerId, const TickType, const double){}
        void onSnapshotUpdate(const TickerId, const double, const int){}
        void setIndicatorTimer();

    private:
        /**
          *Calculates Indicator Value*/
        virtual void calculate();

        /**
          *reset*/
        void reset();

    private:
        /**
          * Initialize the indicator*/
        void initialize();

    protected:
        /**
          *Places order from an indicator*/

    signals:
        /**
          *This signals is emitted to request closing of all open positions for the indicator/strategy */
           void closeAllPositions();
           void placeOrder(const InstrumentId, const Order&);
           void requestPlaceOrderToStrategy(const TickerId, const Order&);

    public:
        /**
          *Returns the Indicator Value */
        const double getValue();

        /**
          *Return the underlying strategy name*/
        const String& getName();

        void setNumInstruments(const int numInstruments)
        {
            _numInstruments = numInstruments;
        }

        virtual void setIndicator(){}
};

#endif
