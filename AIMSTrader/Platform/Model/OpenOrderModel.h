#ifndef OPENORDERMODEL_H
#define OPENORDERMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include "Platform/typedefs.h"
#include <map>
class OpenOrder;

class OpenOrderModel: public QAbstractTableModel
{
    private:
        std::map<OrderId, OpenOrder*> _openOrderPtrMap;
        static const int _numColumns = 10;
        int _numRows;
        QStringList columnNames;
        std::map<OrderId, QModelIndex> _orderIdToTableIndex;
        enum OpenOrderModelColumn{
               OpenOrderId = 0,
               Strategy,
               SecurityType,
               Symbol,
               OrderStatus,
               FilledQuantity,
               TotalQuantity,
               AvgFillPrice,
               LastFillPrice,
               DateTime
        };

    public:
        OpenOrderModel();
        ~OpenOrderModel();

    public:
       int rowCount( const QModelIndex& parent ) const;
       int columnCount( const QModelIndex& parent ) const;
       QVariant data( const QModelIndex& index, int role ) const;
       QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
       bool insertRows();
       bool removeRows(int row);

    public:
       void removeOpenOrder(const OrderId);
       void addOpenOrder(OpenOrder*);
       void updateOpenOrder(const OrderId);
};

#endif // OPENORDERMODEL_H
