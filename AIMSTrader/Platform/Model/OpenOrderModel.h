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
<<<<<<< HEAD
       DateTime
};
=======

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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806


#endif // OPENORDERMODEL_H
