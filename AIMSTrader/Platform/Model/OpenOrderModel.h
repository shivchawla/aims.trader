#ifndef OPENORDERMODEL_H
#define OPENORDERMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include "Platform/typedefs.h"
#include <map>

enum OpenOrderModelColumn{
       OpenOrderID = 0,
       Strategy,
       SecurityType,
       Symbol,
       OrderStatus,
       OrderType,
       FilledQuantity,
       RemainingQuantity,
       TotalQuantity,
       AvgFillPrice,
       LastFillPrice,

       DateTime
};

/*class OpenOrderModel
{
    private:
        std::map<OrderId, OpenOrder*> _openOrderPtrMap;
        static const int _numColumns = 10;
        int _numRows;
        QStringList columnNames;


    public:
        OpenOrderModel();
        ~OpenOrderModel();


};*/

#endif // OPENORDERMODEL_H
