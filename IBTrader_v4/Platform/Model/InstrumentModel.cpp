#include "Platform/Model/InstrumentModel.h"
#include "Platform/Position/Instrument.h"

InstrumentModel::~InstrumentModel()
{
}

InstrumentModel::InstrumentModel():QAbstractTableModel()
{
    _numRows = 0;
}

int InstrumentModel::rowCount( const QModelIndex& parent) const
{
    return _numRows;
}

int InstrumentModel::columnCount( const QModelIndex& parent ) const
{
       return _numColumns;
}

QVariant InstrumentModel::data(const QModelIndex& index, int role ) const
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

QVariant InstrumentModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (role == Qt::DisplayRole)
         {
            if (orientation == Qt::Horizontal)
            {
                 switch (section)
                 {
                     case TickerID: return QString("TickerId");break;
                     case InstrumentId: return QString("InstrumentId");break;
                     case Bid: return QString("Bid");break;
                     case Ask: return QString("Ask");break;
                     case Close: return QString("Close");break;
                     case High: return QString("High");break;
                     case Low: return QString("Low");break;
                 }
             }
         }
     return QVariant();
}

bool InstrumentModel::insertRows()
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    endInsertRows();
    _numRows++;
    return true;
}

bool InstrumentModel::removeRows(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    endInsertRows();
    _numRows--;
    return true;
}

void InstrumentModel::removeInstrument(const TickerId tickerId)
{
    if(_tickerIdToTableIndex.count(tickerId)!=0)
    {
       QModelIndex startIndex = _tickerIdToTableIndex[tickerId];
       int row = startIndex.row();
       removeRows(row);
       Instrument* instrumentToBeDeleted = _instrumentPtrMap[tickerId];
       delete instrumentToBeDeleted;
    }
}

void InstrumentModel::addInstrument(Instrument* instrument)
{
    insertRows();
    _instrumentPtrMap[instrument->getTickerId()] = instrument;
    int row = rowCount(QModelIndex())-1;
    QModelIndex startIndex = createIndex(row,0);
    _tickerIdToTableIndex[instrument->getTickerId()] = startIndex;
}

void InstrumentModel::updateInstrument(const TickerId tickerId)
{
    if(_tickerIdToTableIndex.count(tickerId)!=0)
    {
        QModelIndex startIndex = _tickerIdToTableIndex[tickerId];
        QModelIndex endIndex = createIndex(startIndex.row(), _numColumns-1);
        emit dataChanged(startIndex, endIndex);
    }
}
