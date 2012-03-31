#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
class QTableWidgetItem;

class InstrumentView: public TableView<InstrumentView>
{
    private:
        std::map<TickerId, QTableWidgetItem*> _tickerIdToItemMap;
        enum InstrumentModelColumn{
               TickerID = 0,
               InstrumentId,
               Exchange,
               Last,
               Bid,
               BidSize,
               Ask,
               AskSize,
               Open,
               Close,
               High,
               Low,
               Volume
        };
        QStringList _header;

    public:
        InstrumentView();
        ~InstrumentView();
        void init();

    public:
        void addInstrument(const TickerId, const Contract&);
        void updateInstrument(const TickerId, const ContractDetails&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);
};

#endif // INSTRUMENTVIEW_H
