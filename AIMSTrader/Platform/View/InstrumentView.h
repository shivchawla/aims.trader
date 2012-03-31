#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
#include "Platform/Utils/MarketDataSubscriber.h"
class InstrumentViewItem;

class InstrumentView: public TableView
{
    Q_OBJECT
    private:
        std::map<TickerId, InstrumentViewItem*> _tickerIdToItemMap;

    public:
        InstrumentView(QWidget* parent);
        ~InstrumentView();

    private:
        void setInstrumentView();
        void setHeaders();
        void insertInstrumentItem(InstrumentViewItem* item);


    public:
       /* void addInstrument(const TickerId, const Contract&);
        void updateInstrument(const TickerId, const ContractDetails&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);*/

    public slots:
        void onTradeUpdate(const TickerId, TradeUpdate);
        void onQuoteUpdate(const TickerId, QuoteUpdate);

    private:
        InstrumentViewItem* getInstrumentViewItem(const TickerId);

};

#endif // INSTRUMENTVIEW_H
