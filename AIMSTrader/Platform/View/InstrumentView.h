#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
#include "Platform/Utils/DataSubscriber.h"
#include "Platform/Model/InstrumentModel.h"
class InstrumentViewItem;
class OrderEntryDialog;

class InstrumentView: public TableView<InstrumentView, InstrumentViewItem, InstrumentModel, InstrumentModelColumn>
{
    Q_OBJECT
    private:
        std::map<TickerId, InstrumentViewItem*> _tickerIdToItemMap;

    private:
        QMenu* _instrumentMenuA;
        QAction* _addNewInstrument;
        QAction* _closeAll;
        QAction* _buyAction;
        QAction* _sellAction;
        QMenu* _instrumentMenuB;
        QAction* _symbolLookup;
        OrderEntryDialog* _orderEntryDialog;

    public:
        InstrumentView(QWidget* parent);
        ~InstrumentView();

    private:
        InstrumentViewItem* getInstrumentViewItem(const TickerId);

    private:
        void setupActions();

    private slots:
        void addNewInstrumentToView();
        void lookUpSymbol();
        void buyInstrument();
        void sellInstrument();
        void closeAllPositions();
        void onRemoveHeader();
        void onCustomizeHeader();
        void modifyHeaders(const int);
        void placeOrderfromDialog();

    public slots:
        void contextMenuEvent(QContextMenuEvent *event); 
        void addInstrument(const TickerId, const Contract&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);
        void onTradeUpdate(const TickerId, const TradeUpdate&);
        void onQuoteUpdate(const TickerId, const QuoteUpdate&);

    signals:
        void closed();
        void modifyHeadersClicked(const int);
};

#endif // INSTRUMENTVIEW_H
