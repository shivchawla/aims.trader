#pragma once
#ifndef OPENORDERMODEL_H
#define OPENORDERMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include "AimsTraderDefs/typedefs.h"
#include <map>
#include <vector>
#include "Platform/Utils/Singleton.h"
#include "Platform/Model/DataModel.h"

enum OpenOrderModelColumn{
       OpenOrderModelOrderId = 0,
       OpenOrderModelStrategy,
       OpenOrderModelInstrumentSymbol,
       OpenOrderModelInstrumentType,
       OpenOrderModelExchange,
       OpenOrderModelAction,
       OpenOrderModelPlacedDate,
       OpenOrderModelUpdatedDate,
       OpenOrderModelGoodTillDate,
       OpenOrderModelOrderStatus,
       OpenOrderModelOrderType,
       OpenOrderModelFilledQuantity,
       OpenOrderModelRemainingQuantity,
       OpenOrderModelTotalQuantity,
       OpenOrderModelAvgFillPrice,
       OpenOrderModelLastFillPrice,
       OpenOrderModelLimitPrice,
       OpenOrderModelCommission
    };


class OpenOrderModel: public DataModel<OpenOrderModelColumn>, public Singleton<OpenOrderModel>
{
    friend class Singleton<OpenOrderModel>;
    public:
        OpenOrderModel()
        {
            add(OpenOrderModelOrderId, "OrderId", false);
            add(OpenOrderModelStrategy, "Strategy", true);
            add(OpenOrderModelInstrumentSymbol, "Symbol", true);
            add(OpenOrderModelInstrumentType, "SecurityType", false);
            add(OpenOrderModelExchange, "Exchange", false);
            add(OpenOrderModelAction, "Action", true);
            add(OpenOrderModelPlacedDate, "PlacedDate", false);
            add(OpenOrderModelUpdatedDate, "UpdatedDate", false);
            add(OpenOrderModelGoodTillDate, "GoodTillDate", false);
            add(OpenOrderModelOrderStatus, "Status", true);
            add(OpenOrderModelOrderType, "OrderType", true);
            add(OpenOrderModelFilledQuantity, "FilledQuantity", true);
            add(OpenOrderModelRemainingQuantity, "RemainingQuantity", false);
            add(OpenOrderModelTotalQuantity, "TotalQuantity", true);
            add(OpenOrderModelAvgFillPrice, "AvgFillPrice", true);
            add(OpenOrderModelLastFillPrice, "LastFillPrice", false);
            add(OpenOrderModelLimitPrice, "LimitPrice", false);
            add(OpenOrderModelCommission, "Commission", true);

        }

        public:
             ~OpenOrderModel(){}
};



#endif // OPENORDERMODEL_H
