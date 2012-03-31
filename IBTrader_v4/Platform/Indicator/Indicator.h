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

class Indicator
{
	public:
        Indicator(){}
        ~Indicator();	
    
    private: 
        std::string _name;
        double _value;
    
    public:
        /*virtual*/ void calculate();
        /*virtual*/ void reset();
        
    public:
        const double getValue() 
        {
            return _value;
        }
    
        const std::string& getName() 
        {
            return _name;
        }
};

#endif
