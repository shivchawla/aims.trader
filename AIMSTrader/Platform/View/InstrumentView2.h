#ifndef INSTRUMENTVIEW2_H
#define INSTRUMENTVIEW2_H
#include "Platform/View/TableView3.h"
#include "Platform/Model/InstrumentModel.h"
#include "Platform/View/InstrumentViewItem2.h"
#include "Platform/View/InstrumentViewSubscriber.h"

class OrderEntryDialog;
class SearchLineEdit;
class InstrumentViewSubscriber;

class InstrumentView2 : public TableView3<InstrumentView2, InstrumentViewItem2,InstrumentModel>
{    
    Q_OBJECT
    private:
        QHash<TickerId, int> _tickerIdToRow;
        InstrumentViewSubscriber* _instrumentViewSubscriber;

    private:
        QMenu* _instrumentMenuA;
        QAction* _addNewInstrument;
        QAction* _closeAll;
        QAction* _buyAction;
        QAction* _sellAction;
        QMenu* _instrumentMenuB;
        QAction* _symbolLookup;
        OrderEntryDialog* _orderEntryDialog;
//        SearchLineEdit* _searchLineEdit;
//        QListWidget* _listWidget;
//        int _numInstrumentsInListWidget;

    public:
        InstrumentView2(QWidget* parent);

    private:
        //InstrumentViewItem* getInstrumentViewItem(const TickerId);

    private:
        void setupActions();
        int addRow(const TickerId);

    public:
        void resizeEvent(QResizeEvent *event);

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

    public:
        void contextMenuEvent(QContextMenuEvent *event);
        void addInstrument(const TickerId, const InstrumentContract&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);
        void addInstrument(const TickerId);

    signals:
        //void closed();
        //void modifyHeadersClicked(const int);
};

#endif // INSTRUMENTVIEW2_H
