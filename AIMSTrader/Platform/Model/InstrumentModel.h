#pragma once
#ifndef INSTRUMENTMODEL_H
#define INSTRUMENTMODEL_H

#include <QtCore/QStringList>
#include "AimsTraderDefs/typedefs.h"
#include <map>
#include <vector>
#include "Platform/Utils/Singleton.h"
#include "Platform/Model/DataModel.h"

enum InstrumentModelColumn{
  // InstrumentModelTickerId = 0,
   InstrumentModelSymbol=0,
   InstrumentModelExchange,
   InstrumentModelLast,
   InstrumentModelLastSize,
   InstrumentModelBid,
   InstrumentModelBidSize,
   InstrumentModelAsk,
   InstrumentModelAskSize,
   InstrumentModelOpen,
   InstrumentModelClose,
   InstrumentModelHigh,
   InstrumentModelLow,
   InstrumentModelVolume
};

class InstrumentModel: public DataModel<InstrumentModelColumn>, public Singleton<InstrumentModel>
{
    friend class Singleton<InstrumentModel>;
    public:

        InstrumentModel()
        {
            add(InstrumentModelSymbol, "Symbol", true);
            add(InstrumentModelExchange, "Exchange", false);
            add(InstrumentModelLast, "Last", true);
            add(InstrumentModelLastSize, "LastSize", false);
            add(InstrumentModelBid, "Bid", true);
            add(InstrumentModelBidSize, "BidSize", false);
            add(InstrumentModelAsk, "Ask", true);
            add(InstrumentModelAskSize, "AskSize", false);
            add(InstrumentModelOpen, "Open", false);
            add(InstrumentModelClose, "Close", false);
            add(InstrumentModelHigh, "High", false);
            add(InstrumentModelLow, "Low", false);
            add(InstrumentModelVolume, "Volume", false);
        }

        public:
             ~InstrumentModel(){}
             //static OpenOrderModel* Model();

//        const String getColumnName(const InstrumentModelColumn column)
//        {
//            switch(column)
//            {
//                case InstrumentModelSymbol: return "Symbol", true); break;
//                case InstrumentModelExchange: return "Exchange" ;break;
//                case InstrumentModelLast:return "Last" ;break;
//                case InstrumentModelLastSize:return "LastSize" ;break;
//                case InstrumentModelBid:return "Bid" ;break;
//                case InstrumentModelBidSize:return "BidSize" ;break;
//                case InstrumentModelAsk:return "Ask" ;break;
//                case InstrumentModelAskSize:return "AskSize" ;break;
//                case InstrumentModelOpen:return "Open" ;break;
//                case InstrumentModelClose:return "Close" ;break;
//                case InstrumentModelHigh:return "High" ;break;
//                case InstrumentModelLow:return "Low" ;break;
//                case InstrumentModelVolume:return "Volume" ;break;
//            }
 //       }

//        const String getColumnName(const int column)
//        {
//            InstrumentModelColumn col = InstrumentModelColumn (column);
//            return getColumnName(col);
//        }
};

#endif // INSTRUMENTMODEL_H
