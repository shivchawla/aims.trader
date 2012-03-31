#ifndef INSTRUMENTMODEL_H
#define INSTRUMENTMODEL_H

#include <QtCore/QStringList>
#include <QAbstractTableModel>
#include <map>
#include "Platform/typedefs.h"

class Instrument;
class InstrumentModel: public QAbstractTableModel
{
    private:
        std::map<TickerId, Instrument*> _instrumentPtrMap;
        static const int _numColumns = 7;
        int _numRows;
        QStringList columnNames;
        std::map<TickerId, QModelIndex> _tickerIdToTableIndex;
        enum InstrumentModelColumn{
               TickerID = 0,
               InstrumentId,
               Bid,
               Ask,
               Close,
               High,
               Low
        };

    public:
        InstrumentModel();
        ~InstrumentModel();

    public:
       int rowCount( const QModelIndex& parent ) const;
       int columnCount( const QModelIndex& parent ) const;
       QVariant data( const QModelIndex& index, int role ) const;
       QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
       bool insertRows();
       bool removeRows(int row);

    public:
       void removeInstrument(const TickerId);
       void addInstrument(Instrument*);
       void updateInstrument(const TickerId);
};

#endif // INSTRUMENTMODEL_H
