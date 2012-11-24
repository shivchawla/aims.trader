#include "Platform/View/InstrumentView2.h"
#include "Platform/View/InstrumentViewItem2.h"
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"
#include <QMenu>
#include <QAction>
#include "Platform/View/OrderEntryDialog.h"
#include "Platform/Strategy/StrategyManager.h"
#include <QInputDialog>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/View/SearchLineEdit.h"
#include "Platform/View/IODatabase.h"
#include "Platform/View/InstrumentViewSubscriber.h"

InstrumentView2::InstrumentView2(QWidget* parent = 0 ):TableView3<InstrumentView2, InstrumentViewItem2, InstrumentModel>(parent)
{
    _instrumentViewSubscriber = new InstrumentViewSubscriber(this);
    /*_numInstrumentsInListWidget = 0;
    _searchLineEdit = new SearchLineEdit(this);
    _listWidget = new QListWidget(this);
    int headerHeight = horizontalHeader()->size().height();
    int viewWidth = size().width();
    _searchLineEdit->setGeometry(QRect(pos().x(),pos().y(), viewWidth, headerHeight));
    _listWidget->setGeometry(QRect(_searchLineEdit->pos().x(), _searchLineEdit->pos().y() + headerHeight, viewWidth, _numInstrumentsInListWidget*headerHeight));

    _listWidget->hide();
    _listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listWidget->setWindowFlags(Qt::SubWindow);

    connect(_listWidget,SIGNAL(itemClicked(QListWidgetItem* )), this, SLOT(onInstrumentSelection(QListWidgetItem*)));

    for(int i=0;i<5;++i)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setForeground(Qt::black);
        item->setBackground(Qt::white);
        item->setSelected(false);
        item->setFont(QFont("Cambria"));
        _listWidget->insertItem(i,item);
    }*/

    connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(modifyHeaders(const int)));
    //connect()
    setupActions();
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));
    setSortingEnabled(false);
}

//InstrumentViewItem* InstrumentView::getInstrumentViewItem(const TickerId intrumentId)
//{
//    if(_instrumentIdToItemMap.count(intrumentId)!=0)
//    {
//        return _instrumentIdToItemMap[intrumentId];
//    }
//    return NULL;
//}


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

void InstrumentView2::updateTickPrice(const TickerId tickerId, const TickType tickType, const double price, const int canAutoExecute)
{
    int row = _tickerIdToRow.value(tickerId, -1);
    if(row!=-1)
    {
        switch(tickType)
        {
            case OPEN:  updateItem(row, InstrumentModelOpen, price); break;
            case CLOSE: updateItem(row, InstrumentModelClose, price); break;
            case HIGH:  updateItem(row, InstrumentModelHigh, price); break;
            case LOW:   updateItem(row, InstrumentModelLow, price); break;
            case BID:   updateItemSpecial(row, InstrumentModelBid, price); break;
            case ASK:   updateItemSpecial(row, InstrumentModelAsk,price); break;
            case LAST:  updateItemSpecial(row, InstrumentModelLast, price); break;
            default: break;
        }
    }
}

void InstrumentView2::updateTickSize(const TickerId tickerId , const TickType tickType, const int size)
{
    int row = _tickerIdToRow.value(tickerId, -1);
    if(row!=-1)
    {
        QString res =  QString::number(size);
         switch(tickType)
        {
            case BID_SIZE:updateItem(row, InstrumentModelBidSize, res); break;
            case ASK_SIZE:updateItem(row, InstrumentModelAskSize, res); break;
            case LAST_SIZE:updateItem(row, InstrumentModelLastSize, res);break;
            default: break;
        }
    }
}

void InstrumentView2::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    int row = _tickerIdToRow.value(tickerId, -1);
    if(row != -1)
    {
        switch(tickType)
        {
            case OPEN:  updateItem(row, InstrumentModelOpen, value); break;
            case CLOSE: updateItem(row, InstrumentModelClose, value); break;
            case HIGH:  updateItem(row, InstrumentModelHigh, value); break;
            case LOW:   updateItem(row, InstrumentModelLow, value); break;
            case BID:   updateItemSpecial(row, InstrumentModelBid, value); break;
            case ASK:   updateItemSpecial(row, InstrumentModelAsk, value); break;
            case LAST:  updateItemSpecial(row, InstrumentModelLast, value); break;
            case BID_SIZE:updateItem(row, InstrumentModelBidSize, value); break;
            case ASK_SIZE:updateItem(row, InstrumentModelAskSize, value); break;
            case LAST_SIZE:updateItem(row, InstrumentModelLastSize, value);break;
            case VOLUME: updateItem(row, InstrumentModelVolume, value);break;
            default: break;
        }
    }
}

int InstrumentView2::addRow(const TickerId tickerId)
{
    int row = _numRows++;
    _tickerIdToRow[tickerId] = row;

    insertRow(row);
    for(int i=0;i<_numCols;++i)
    {
        insertItem(row, i, new InstrumentViewItem2(tickerId));
    }

    return row;
}

void InstrumentView2::addInstrument(const TickerId tickerId, const InstrumentContract& instrumentContract)
{
    int row = _tickerIdToRow.value(tickerId, -1);
    if(row == -1)
    {
        row = addRow(tickerId);
        updateItem(row, InstrumentModelSymbol, instrumentContract.symbol);
        updateItem(row, InstrumentModelExchange, instrumentContract.exchangeCode);
        _instrumentViewSubscriber->subscribeMarketData(tickerId);
    }
}

void InstrumentView2::addInstrument(const TickerId tickerId)
{
    int row = _tickerIdToRow.value(tickerId, -1);
    if(row == -1)
    {
        row = addRow(tickerId);
        Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);
        updateItem(row, InstrumentModelSymbol, QString::fromStdString(contract.symbol));
        updateItem(row, InstrumentModelExchange, QString::fromStdString(contract.exchange));
        _instrumentViewSubscriber->subscribeMarketData(tickerId);
    }
}


void InstrumentView2::addNewInstrumentToView()
{
//    // show a dialog
//    bool ok;
//         QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
//                                              tr("User name:"), QLineEdit::Normal);

}

void InstrumentView2::lookUpSymbol()
{

}

void InstrumentView2::buyInstrument()
{
   //get Information about the clicked item
    TickerId tickerId = static_cast<InstrumentViewItem2*>(_clickedItem)->getTickerId();
    Order order;
    order.action = "BUY";

    _orderEntryDialog->setupDialog(tickerId, order);
}

void InstrumentView2::sellInstrument()
{
    TickerId tickerId = _clickedItem->getTickerId();
    Order order;
    order.action ="SELL";

    _orderEntryDialog->setupDialog(tickerId, order);
}

void InstrumentView2::closeAllPositions()
{
    TickerId tickerId = _clickedItem->getTickerId();
    //ask strategy manager to close aal positions in all strategies for this tickerId
     StrategyManager::strategyManager().closeAllPositionsForInstrument(tickerId);
}

void InstrumentView2::contextMenuEvent(QContextMenuEvent *event)
{
    _clickedItem = static_cast<InstrumentViewItem2*>(itemAt(event->x(), event->y()));
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


void InstrumentView2::onRemoveHeader()
{
    removeHeader();
}

void InstrumentView2::onCustomizeHeader()
{
     //_dialog->show();
}

void InstrumentView2::modifyHeaders(const int column)
{
    //modifyHeader(column);
}

void InstrumentView2::placeOrderfromDialog()
{
    TickerId tickerId = _clickedItem->getTickerId();

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

void InstrumentView2::resizeEvent(QResizeEvent *event)
{
    TableView3<InstrumentView2, InstrumentViewItem2, InstrumentModel>::resizeEvent(event);
//    int headerHeight = horizontalHeader()->size().height();
//    int viewWidth = size().width();
//    _searchLineEdit->setGeometry(QRect(pos().x(),pos().y(), viewWidth, headerHeight));
//    _listWidget->setGeometry(QRect(_searchLineEdit->pos().x(), _searchLineEdit->pos().y() + headerHeight, viewWidth, _numInstrumentsInListWidget*headerHeight));
}

void InstrumentView2::setupActions()
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

    //connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    //connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));

    _orderEntryDialog = new OrderEntryDialog(this);
    connect(_buyAction, SIGNAL(triggered()), this , SLOT(buyInstrument()));
    connect(_sellAction, SIGNAL(triggered()), this, SLOT(sellInstrument()));
    connect(_closeAll,SIGNAL(triggered()), this, SLOT(closeAllPositions()));
    connect(_orderEntryDialog, SIGNAL(accepted()), this, SLOT(placeOrderfromDialog()));
}


