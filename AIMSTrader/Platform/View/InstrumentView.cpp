#include "Platform/View/InstrumentView.h"
#include "Platform/View/InstrumentViewItem.h"
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"
#include <QMenu>
#include <QAction>
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Strategy/StrategyManager.h"
#include <QInputDialog>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

InstrumentView::InstrumentView(QWidget* parent = 0 ):TableView<InstrumentView, InstrumentViewItem, InstrumentModel, InstrumentModelColumn>(parent)
{
    setupActions();
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
    setSortingEnabled(false);
}

//InstrumentView::~InstrumentView()
//{}

InstrumentViewItem* InstrumentView::getInstrumentViewItem(const TickerId intrumentId)
{
    if(_instrumentIdToItemMap.count(intrumentId)!=0)
    {
        return _instrumentIdToItemMap[intrumentId];
    }
    return NULL;
}


//void InstrumentView::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
//{
//    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
//    if(instrumentItem)
//    {
//        instrumentItem->updateLastPrice(tradeUpdate.lastPrice, InstrumentModelLast));
//        instrumentItem->update(QString::number(tradeUpdate.lastSize), InstrumentModelLastSize));
//    }
//}

//void InstrumentView::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
//{
//    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
//    if(instrumentItem)
//    {
//        instrumentItem->updateBidPrice(quoteUpdate.bidPrice, InstrumentModelBid));
//        instrumentItem->update(QString::number(quoteUpdate.bidSize), InstrumentModelBidSize));
//        instrumentItem->updateAskPrice(quoteUpdate.askPrice, InstrumentModelAsk));
//        instrumentItem->update(QString::number(quoteUpdate.askSize), InstrumentModelAskSize));
//    }
//}

void InstrumentView::updateTickPrice(const TickerId instrumentId, const TickType tickType, const double price, const int canAutoExecute)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->update(price, InstrumentModelOpen); break;
            case CLOSE: instrumentItem->update(price, InstrumentModelClose); break;
            case HIGH: instrumentItem->update(price, InstrumentModelHigh); break;
            case LOW: instrumentItem->update(price, InstrumentModelLow); break;
            case BID: instrumentItem->updateBidPrice(price, InstrumentModelBid); break;
            case ASK: instrumentItem->updateAskPrice(price, InstrumentModelAsk); break;
            case LAST: instrumentItem->updateLastPrice(price, InstrumentModelLast); break;
            default: break;
        }
    }

}

void InstrumentView::updateTickSize(const TickerId instrumentId , const TickType tickType, const int size)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        QString res =  QString::number(size);
         switch(tickType)
        {
            case BID_SIZE:instrumentItem->update(res, InstrumentModelBidSize); break;
            case ASK_SIZE:instrumentItem->update(res, InstrumentModelAskSize); break;
            case LAST_SIZE:instrumentItem->update(res, InstrumentModelLastSize);break;
            default: break;
        }
    }
}

void InstrumentView::updateTickGeneric(const TickerId instrumentId, const TickType tickType, const double value)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(instrumentId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->update(value, InstrumentModelOpen); break;
            case CLOSE: instrumentItem->update(value, InstrumentModelClose); break;
            case HIGH: instrumentItem->update(value, InstrumentModelHigh); break;
            case LOW: instrumentItem->update(value, InstrumentModelLow); break;
            case BID: instrumentItem->updateBidPrice(value, InstrumentModelBid); break;
            case ASK: instrumentItem->updateAskPrice(value, InstrumentModelAsk); break;
            case LAST: instrumentItem->updateLastPrice(value, InstrumentModelLast); break;
            case BID_SIZE:instrumentItem->update(value, InstrumentModelBidSize); break;
            case ASK_SIZE:instrumentItem->update(value, InstrumentModelAskSize); break;
            case LAST_SIZE:instrumentItem->update(value, InstrumentModelLastSize);break;
            case VOLUME: instrumentItem->update(value, InstrumentModelVolume); break;
            default: break;
        }
    }
}

void InstrumentView::addInstrument(const TickerId instrumentId, const InstrumentContract& instrumentContract)
{
    if(_instrumentIdToItemMap[instrumentId]==0)
    {
        InstrumentViewItem* instrumentItem = addItemInView();
        instrumentItem->setTickerId(instrumentId);
        _instrumentIdToItemMap[instrumentId] = instrumentItem;

        instrumentItem->update(instrumentContract.symbol, InstrumentModelSymbol);
        instrumentItem->update(instrumentContract.exchangeCode, InstrumentModelExchange);
    }
}

void InstrumentView::addInstrument(const TickerId tickerId)
{
    if(_instrumentIdToItemMap[tickerId]==0)
    {
        InstrumentViewItem* instrumentItem = addItemInView();
        instrumentItem->setTickerId(tickerId);
        _instrumentIdToItemMap[tickerId] = instrumentItem;
        Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);

        instrumentItem->update(contract.symbol, InstrumentModelSymbol);
        instrumentItem->update(contract.exchange, InstrumentModelExchange);
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
    TickerId tickerId = _clickedItem->parent()->getTickerId();
    Order order;
    order.action = "BUY";

    _orderEntryDialog->setupDialog(tickerId, order);
}

void InstrumentView::sellInstrument()
{
    TickerId tickerId = _clickedItem->parent()->getTickerId();
    Order order;
    order.action ="SELL";

    _orderEntryDialog->setupDialog(tickerId, order);
}

void InstrumentView::closeAllPositions()
{
    TickerId tickerId = _clickedItem->parent()->getTickerId();
    //ask strategy manager to close aal positions in all strategies for this tickerId
     StrategyManager::strategyManager().closeAllPositionsForInstrument(tickerId);
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
    TickerId tickerId = _clickedItem->parent()->getTickerId();

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
    StrategyManager::strategyManager().addPosition(tickerId, o);
}
