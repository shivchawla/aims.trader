#pragma once
#ifndef OPENORDERMODEL_H
#define OPENORDERMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include "Platform/typedefs.h"
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
//             _totalNumColumns = 18;
//            //_columns.reserve(_numColumns);
//            _dataModel.push_back(OpenOrderModelOrderId);
//            _dataModel.push_back(OpenOrderModelStrategy);
//            _dataModel.push_back(OpenOrderModelInstrumentSymbol);
//            _dataModel.push_back(OpenOrderModelInstrumentType);
//            _dataModel.push_back(OpenOrderModelAction);
//            _dataModel.push_back(OpenOrderModelOrderStatus);
//            _dataModel.push_back(OpenOrderModelOrderType);
//            _dataModel.push_back(OpenOrderModelFilledQuantity);
//            _dataModel.push_back(OpenOrderModelTotalQuantity);
//            _dataModel.push_back(OpenOrderModelAvgFillPrice);
//            _dataModel.push_back(OpenOrderModelLimitPrice);
//            _dataModel.push_back(OpenOrderModelCommission);


            add(OpenOrderModelOrderId, "OrderId", false);
            add(OpenOrderModelStrategy, "Strategy", true);
            add(OpenOrderModelInstrumentSymbol, "Symbol", true);
            add(OpenOrderModelInstrumentType, "SecurityType", true);
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
