#include "Platform/Model/OpenOrderModel.h"
#include "Platform/Position/OpenOrder.h"


OpenOrderModel::OpenOrderModel():QAbstractTableModel()
{
    _numRows = 0;
}

OpenOrderModel::~OpenOrderModel()
{}

int OpenOrderModel::rowCount( const QModelIndex& parent ) const
{
    return _openOrderPtrMap.size();
}

int OpenOrderModel::columnCount( const QModelIndex& parent ) const
{
       return _numColumns;
}

QVariant OpenOrderModel::data(const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
    {
           return QVariant();
    }

    if ( role == Qt::TextAlignmentRole )
    {
        return int( Qt::AlignRight | Qt::AlignVCenter );
    }
    else if( role == Qt::DisplayRole)
    {
        /*std::map<OrderId, OpenOrder*>::iterator const it = _openOrderPtrMap.begin() +  index.row();
        int i=
        switch ( i )
        {
            case 0 : return _orderId;
            case 1 : return _openOrderPtrMap[orderId]->
            case 2 : return currentTicks.value( instrument )->getAsk();
            case 3 : return currentTicks.value( instrument )->getBidVolume();
            case 4 : return currentTicks.value( instrument )->getAskVolume();
        }*/
    }
    return QVariant();
}

QVariant OpenOrderModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (role == Qt::DisplayRole)
         {
            if (orientation == Qt::Horizontal)
            {
                 switch (section)
                 {
                     case OpenOrderId: return QString("OpenOrderId");break;
                     case Strategy: return QString("Strategy");break;
                     case SecurityType: return QString("SecurityType");break;
                     case Symbol: return QString("Symbol");break;
                     case OrderStatus: return QString("OrderStatus");break;
                     case FilledQuantity: return QString("FilledQuantity");break;
                     case TotalQuantity: return QString("TotalQuantity");break;
                     case AvgFillPrice: return QString("AvgFillPrice");break;
                     case LastFillPrice: return QString("LastFillPrice");break;
                     case DateTime: return QString("DateTime");break;
                 }
             }
         }
     return QVariant();
}

bool OpenOrderModel::insertRows()
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    endInsertRows();
    _numRows++;
    return true;
}

bool OpenOrderModel::removeRows(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
    _numRows--;
    return true;
}



void OpenOrderModel::removeOpenOrder(const OrderId orderId)
{
    if(_orderIdToTableIndex.count(orderId)!=0)
    {
       QModelIndex startIndex = _orderIdToTableIndex[orderId];
       int row = startIndex.row();
       removeRows(row);
       OpenOrder* openOrderToBeDeleted = _openOrderPtrMap[orderId];
       delete openOrderToBeDeleted;
    }
}

void OpenOrderModel::addOpenOrder(OpenOrder* openOrder)
{
    insertRows();
    OrderId orderId = openOrder->getOrderId();
    _openOrderPtrMap[orderId] = openOrder;
    int row = rowCount(QModelIndex())-1;
    QModelIndex startIndex = createIndex(row,0);
    _orderIdToTableIndex[orderId] = startIndex;
}

void OpenOrderModel::updateOpenOrder(const OrderId orderId)
{
    if(_orderIdToTableIndex.count(orderId)!=0)
    {
        QModelIndex startIndex = _orderIdToTableIndex[orderId];
        QModelIndex endIndex = createIndex(startIndex.row(), _numColumns-1);
        emit dataChanged(startIndex, endIndex);
    }
}
