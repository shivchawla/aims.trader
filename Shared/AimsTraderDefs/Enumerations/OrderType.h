#pragma once
#ifndef ORDERTYPE_H
#define ORDERTYPE_H

//! OrderStatus Enum
enum OrderType {

    MKT = 0, /*!< Enum value Market*/
    MKTCLS, /*!< Enum value Market */
    LMT, /*!< Enum value Limit */
    LMTCLS, /*!< Enum value None */
    PEGMKT, /*!< Enum value Peg-To-Market */
    SCALE, /*!< Enum value Scale */
    STP, /*!< Enum value Stop */
    STPLMT, /*!< Enum value Stop-Limit */
    TRAIL, /*!< Enum value Trailing */
    REL, /*!< Enum value Relative */
    VWAP, /*!< Enum value VWAP */
    TRAILLIMIT /*!< Enum value Trail-Limit */
};

#endif // ORDERTYPE_H
