#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
#include "Platform/Utils/DataSubscriber.h"
class InstrumentViewItem;

class InstrumentView: public TableView<InstrumentView>
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


    public slots:
       /* void addInstrument(const TickerId, const Contract&);*/
        void addInstrument(const TickerId, const Contract&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);

    public slots:
        void onTradeUpdate(const TickerId, const TradeUpdate&);
        void onQuoteUpdate(const TickerId, const QuoteUpdate&);

    private:
        InstrumentViewItem* getInstrumentViewItem(const TickerId);

    signals:
        void closed();


};

#endif // INSTRUMENTVIEW_H
