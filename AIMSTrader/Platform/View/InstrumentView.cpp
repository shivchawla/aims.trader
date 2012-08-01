#include "Platform/View/InstrumentView.h"
#include "Platform/View/InstrumentViewItem.h"
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"
#include <QMenu>
#include <QAction>
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Strategy/StrategyManager.h"
#include <QInputDialog>

InstrumentView::InstrumentView(QWidget* parent = 0 ):TableView<InstrumentView, InstrumentViewItem, InstrumentModel, InstrumentModelColumn>(parent)
{
    setupActions();
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
}

//InstrumentView::~InstrumentView()
//{}

InstrumentViewItem* InstrumentView::getInstrumentViewItem(const InstrumentId instrumentId)
{
    if(_instrumentIdToItemMap.count(instrumentId)!=0)
    {
        return _instrumentIdToItemMap[instrumentId];
    }
    return NULL;
}


//void InstrumentView::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
//{
//    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
//    if(instrumentItem)
//    {
//        instrumentItem->updateLastPrice(tradeUpdate.lastPrice, getViewColumn(InstrumentModelLast));
//        instrumentItem->update(QString::number(tradeUpdate.lastSize), getViewColumn(InstrumentModelLastSize));
//    }
//}

//void InstrumentView::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
//{
//    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
//    if(instrumentItem)
//    {
//        instrumentItem->updateBidPrice(quoteUpdate.bidPrice, getViewColumn(InstrumentModelBid));
//        instrumentItem->update(QString::number(quoteUpdate.bidSize), getViewColumn(InstrumentModelBidSize));
//        instrumentItem->updateAskPrice(quoteUpdate.askPrice, getViewColumn(InstrumentModelAsk));
//        instrumentItem->update(QString::number(quoteUpdate.askSize), getViewColumn(InstrumentModelAskSize));
//    }
//}

void InstrumentView::updateTickPrice(const InstrumentId instrumentId, const TickType tickType, const double price, const int canAutoExecute)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->update(QString::number(price), getViewColumn(InstrumentModelOpen)); break;
            case CLOSE: instrumentItem->update(QString::number(price), getViewColumn(InstrumentModelClose)); break;
            case HIGH: instrumentItem->update(QString::number(price), getViewColumn(InstrumentModelHigh)); break;
            case LOW: instrumentItem->update(QString::number(price), getViewColumn(InstrumentModelLow)); break;
            case BID: instrumentItem->updateBidPrice(price, getViewColumn(InstrumentModelBid)); break;
            case ASK: instrumentItem->updateAskPrice(price, getViewColumn(InstrumentModelAsk)); break;
            case LAST: instrumentItem->updateLastPrice(price, getViewColumn(InstrumentModelLast)); break;
            default: break;
        }
    }

}

void InstrumentView::updateTickSize(const InstrumentId instrumentId , const TickType tickType, const int size)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        QString res =  QString::number(size);
         switch(tickType)
        {
            case BID_SIZE:instrumentItem->update(res, getViewColumn(InstrumentModelBidSize)); break;
            case ASK_SIZE:instrumentItem->update(res, getViewColumn(InstrumentModelAskSize)); break;
            case LAST_SIZE:instrumentItem->update(res, getViewColumn(InstrumentModelLastSize));break;
            default: break;
        }
    }
}

void InstrumentView::updateTickGeneric(const InstrumentId instrumentId, const TickType tickType, const double value)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelOpen)); break;
            case CLOSE: instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelClose)); break;
            case HIGH: instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelHigh)); break;
            case LOW: instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelLow)); break;
            case BID: instrumentItem->updateBidPrice(value, getViewColumn(InstrumentModelBid)); break;
            case ASK: instrumentItem->updateAskPrice(value, getViewColumn(InstrumentModelAsk)); break;
            case LAST: instrumentItem->updateLastPrice(value, getViewColumn(InstrumentModelLast)); break;
            case BID_SIZE:instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelBidSize)); break;
            case ASK_SIZE:instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelAskSize)); break;
            case LAST_SIZE:instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelLastSize));break;
            case VOLUME: instrumentItem->update(QString::number(value), getViewColumn(InstrumentModelVolume)); break;
            default: break;
        }
    }
}

void InstrumentView::addInstrument(const InstrumentId instrumentId, const InstrumentContract& instrumentContract)
{
    if(_instrumentIdToItemMap[instrumentId]==0)
    {
        InstrumentViewItem* instrumentItem = addItemInView();
        instrumentItem->setTickerId(instrumentId);
        _instrumentIdToItemMap[instrumentId] = instrumentItem;

        instrumentItem->update(instrumentContract.symbol, getViewColumn(InstrumentModelSymbol));
        instrumentItem->update(instrumentContract.exchangeCode, getViewColumn(InstrumentModelExchange));
    }
}


void InstrumentView::setupActions()
{
    _instrumentMenuA = new QMenu("InstrumentMenuA", this);
    _addNewInstrument = new QAction(tr("Add Instrument"), this);
    connect(_addNewInstrument, SIGNAL(triggered()), this, SLOT(addNewInstrumentToView()));
    _closeAll = new QAction(tr("Close All Positions"), this);
    _buyAction = new QAction(tr("BUY"), this);
    _sellAction = new QAction(tr("SELL"), this);
    _instrumentMenuB = new QMenu("InstrumentMenuB", this);
    _symbolLookup = new QAction(tr("Lookup Symbol"), this);

    _instrumentMenuA->addAction(_buyAction);
    _instrumentMenuA->addAction(_sellAction);
    _instrumentMenuA->addSeparator();
    _instrumentMenuA->addAction(_closeAll);

    _instrumentMenuB->addAction(_addNewInstrument);
    _instrumentMenuB->addAction(_symbolLookup);

    connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));

    _orderEntryDialog = new OrderEntryDialog(this);
    connect(_buyAction, SIGNAL(triggered()), this, SLOT(buyInstrument()));
    connect(_sellAction, SIGNAL(triggered()), this, SLOT(sellInstrument()));
    connect(_closeAll,SIGNAL(triggered()), this, SLOT(closeAllPositions()));
    connect(_orderEntryDialog, SIGNAL(accepted()), this, SLOT(placeOrderfromDialog()));
}

void InstrumentView::addNewInstrumentToView()
{
//    // show a dialog
//    bool ok;
//         QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
//                                              tr("User name:"), QLineEdit::Normal);

}

void InstrumentView::lookUpSymbol()
{

}

void InstrumentView::buyInstrument()
{
   //get Information about the clicked item
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();
    Order order;
    order.action = "BUY";

    _orderEntryDialog->setupDialog(instrumentId, order);
}

void InstrumentView::sellInstrument()
{
    TickerId instrumentId = _clickedItem->parent()->getInstrumentId();
    Order order;
    order.action ="SELL";

    _orderEntryDialog->setupDialog(instrumentId, order);
}

void InstrumentView::closeAllPositions()
{
    TickerId instrumentId = _clickedItem->parent()->getInstrumentId();
    //ask strategy manager to close aal positions in all strategies for this tickerId
     StrategyManager::strategyManager().closeAllPositionsForInstrument(instrumentId);
}

void InstrumentView::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<TableCellItem<InstrumentViewItem> *>(itemAt(event->x(), event->y()));
    if(_clickedItem)
    {
        event->accept();
        _instrumentMenuA->exec(event->globalPos());
    }
    else
    {
        event->accept();
        _instrumentMenuB->exec(event->globalPos());
    }
}


void InstrumentView::onRemoveHeader()
{
    removeHeader();
}

void InstrumentView::onCustomizeHeader()
{
     //_dialog->show();
}

void InstrumentView::modifyHeaders(const int column)
{
    modifyHeader(column);
}

void InstrumentView::placeOrderfromDialog()
{
    InstrumentId instrumentId = _clickedItem->parent()->getInstrumentId();

    Order o;
    int quantity = _orderEntryDialog->getQuantity();
    o.totalQuantity = std::abs(quantity);

    OrderType orderType = _orderEntryDialog->getOrderType();
    if(orderType == MKT)
    {
        o.orderType = "MKT";
    }
    else if(orderType == LMT)
    {
        o.orderType = "LMT";
    }

    OrderSide orderSide = _orderEntryDialog->getOrderSide();
    if(orderSide == BUY)
    {
        o.action = "BUY";
    }
    else if(orderSide == SELL)
    {
        o.action = "SELL";
    }

    o.lmtPrice = _orderEntryDialog->getLimitPrice();
    o.referencePriceType=0;
    StrategyManager::strategyManager().addPosition(instrumentId, o);
}
