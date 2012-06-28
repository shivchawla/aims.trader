/*
 *  SecurityType.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 9/22/11.
 *  Copyright 2011 AIMS inc. All rights reserved.
 *
 */
#pragma once
#ifndef SECURITYTYPE_H
#define SECURITYTYPE_H

//! SecurityType Enum
enum SecurityType 
{
    None, /*< Enum value None*/
    Stock, /*< Enum value Stock*/
    Future, /*< Enum value Future*/
    Forex, /*< Enum value Forex*/
    Option, /*< Enum value Option*/
    Index, /*< Enum value Index*/
    FutureOption, /*< Enum value Future Option*/
    Bag, /*< Enum value Bag*/
    Bond /*< Enum value Bond*/
};
	
#endif
