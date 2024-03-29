#pragma once
#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include "Platform/View/TableView.h"
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Order.h"
#include "InteractiveBroker/Shared/Execution.h"
#include "AimsTraderDefs/typedefs.h"
//#include "Platform/Enumerations/TickType.h"
#include "Platform/Utils/DataSubscriber.h"
#include "Platform/Model/InstrumentModel.h"
class InstrumentViewItem;
class OrderEntryDialog;
class SearchLineEdit;
class InstrumentViewSubscriber;

class InstrumentView: public TableView<InstrumentView, InstrumentViewItem, InstrumentModel, InstrumentModelColumn>
{
    friend class InstrumentViewWidget;
    Q_OBJECT
    private:
        std::map<TickerId, InstrumentViewItem*> _instrumentIdToItemMap;
        //QList<InstrumentData*> _instrumentDataInListWidget;
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

    public:
        InstrumentView(QWidget* parent);
        //~InstrumentView();

    private:
        InstrumentViewItem* getInstrumentViewItem(const TickerId);

    private:
        void setupActions();

//    public slots:
//        void updateSearch(const QString&);

    private slots:
        void onRemoveHeader();
        void modifyHeaders(const int);
        void addNewInstrumentToView();
        void lookUpSymbol();
        void buyInstrument();
        void sellInstrument();
        void closeAllPositions();
        void placeOrderfromDialog();


    public:
        void contextMenuEvent(QContextMenuEvent *event);
        void addInstrument(const TickerId, const InstrumentContract&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);
        void addInstrument(const TickerId);
        const InstrumentViewItem* getClickedInstrumentViewItem();

//    private slots:
//        void onInstrumentSelection(QListWidgetItem*);

    signals:
        void closed();
        void modifyHeadersClicked(const int);
};

#endif // INSTRUMENTVIEW_H
