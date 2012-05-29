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

#endif // OPENORDERMODEL_H
