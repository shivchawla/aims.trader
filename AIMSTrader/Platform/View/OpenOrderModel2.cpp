#include "Platform/View/OpenOrderModel2.h"
#include "Platform/Model/OpenOrderModel.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include <QDebug>

OpenOrderModel2::OpenOrderModel2(QObject *parent):GeneralModel2<OpenOrderModel>(parent)
{}

QVariant OpenOrderModel2::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     int row = index.row();
     if (row >= _orders.count() || row < 0)
         return QVariant();

     if (role == Qt::TextAlignmentRole)
             return Qt::AlignCenter;

     if (role == Qt::DisplayRole)
     {
         const OpenOrderModelData& data = _orders.value(row, OpenOrderModelData());
         const OrderDetail& detail = data.getOrderDetail();
         OrderId orderId = data.getOrderId();

         switch(index.column())
         {
                case OpenOrderModelOrderId: return (uint)orderId; break;
                case OpenOrderModelStrategy: return StrategyManager::strategyManager().getStrategyName(detail.getStrategyId()); break;
                case OpenOrderModelInstrumentSymbol: return Service::service().getInstrumentManager()->getSymbol(detail.getTickerId()); break;
                case OpenOrderModelInstrumentType: return "EQ"; break;
                case OpenOrderModelExchange: return QString::fromStdString(Service::service().getInstrumentManager()->getIBContract(detail.getTickerId()).exchange); break;
                case OpenOrderModelAction: return QString::fromStdString(detail.getOrder().action); break;
                case OpenOrderModelPlacedDate: return detail.getPlacedTime(); break;
                case OpenOrderModelUpdatedDate: return detail.getLastUpdatedTime(); break;
                case OpenOrderModelGoodTillDate: return QString::fromStdString(detail.getOrder().goodTillDate); break;
                case OpenOrderModelOrderStatus: return detail.getOrderStatus(); break;
                case OpenOrderModelOrderType: return QString::fromStdString(detail.getOrder().orderType); break;
                case OpenOrderModelFilledQuantity: return (uint)detail.getFilledShares(); break;
                case OpenOrderModelRemainingQuantity: return (uint)detail.getPendingShares(); break;
                case OpenOrderModelTotalQuantity: return (uint)detail.getOrder().totalQuantity; break;
                case OpenOrderModelAvgFillPrice: return detail.getAvgFillPrice(); break;
                case OpenOrderModelLastFillPrice: return detail.getLastFillPrice(); break;
                case OpenOrderModelLimitPrice: return detail.getOrder().lmtPrice; break;
                case OpenOrderModelCommission: return detail.getCommission(); break;
         }
     }
     return QVariant();
}


void OpenOrderModel2::updateOrder(const OrderId orderId, const OrderDetail& orderDetail, const int tab)
{
   //qDebug()<<"Count ="<<_count;
   int row;
   if((row = _indices.value(orderId, -1)) != -1)
   {
       _orders[row].setOrderDetail(orderDetail);
   }
}

void OpenOrderModel2::onStatusUpdate(const OrderId orderId, const OrderStatus status, const int tab)
{
    int row;
    if((row = _indices.value(orderId, -1)) != -1)
    {

    }
}

void OpenOrderModel2::addOrder(const OrderId orderId, const OrderDetail& orderDetail)
{
    int row;
    //check if the position already exists in the model
    if((row = _indices.value(orderId, -1)) != -1)
    {
        return;
    }

    _indices[orderId] = _numRows;
    OpenOrderModelData data(orderId, orderDetail);
    _orders.append(data);
    insertRow(_numRows++);
    emit dataChanged(createIndex(_numRows,0), createIndex(_numRows,_numCols-1));
}

void OpenOrderModel2::removeOrder(const OrderId orderId)
{
    //no such order exists
    int row;
    if((row = _indices.value(orderId, -1)) == -1)
    {
        return;
    }

//    _orders.removeAt(row);
//    _indices.remove(orderId)
//    removeRow(row);
//    _numRows--;
}

bool OpenOrderModel2::getOrderId(const QModelIndex& idx)
{
      int row = idx.row();
      if(row!=-1)
      {
        return _orders.at(row).getOrderId();
      }

      return -1;
}

const OrderDetail& OpenOrderModel2::getOrderDetail(const QModelIndex& idx)
{
      int row = idx.row();
      if(row!=-1)
      {
        return _orders.at(row).getOrderDetail();
      }

      return OrderDetail();
}
















