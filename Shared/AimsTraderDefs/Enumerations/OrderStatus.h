/*
 *  OrderStatus.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 10/4/11.
 *  Copyright 2011 AIMS inc. All rights reserved.
 *
 */
#pragma once
#ifndef OrderStatus_h
#define OrderStatus_h

//! OrderStatus Enum
enum OrderStatus
{
    None, /*!< Enum value None */
    PendingSubmit, /*!< Enum value PendingSubmit */
    PendingCancel, /*!< Enum value PendingCancel */
    PreSubmitted, /*!< Enum value PreSubmitted */
    Submitted, /*!< Enum value Submitted */
    Canceled, /*!< Enum value Canceled */
    FullyFilled, /*!< Enum value FullyFilled */
    Inactive, /*!< Enum value Inactive */
    PartiallyFilled, /*!< Enum value PartiallyFilled */
    ApiPending, /*!< Enum value ApiPending */
    ApiCanceled /*!< Enum value ApiCanceled */
};

#endif
