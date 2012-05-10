#include "Platform/View/InstrumentView.h"
#include "Platform/View/InstrumentViewItem.h"
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"

InstrumentView::InstrumentView(QWidget* parent = 0 ):TableView<InstrumentView>(parent)
{
    _numCols = InstrumentViewItem::getNumItems();
    setInstrumentView();
}

void InstrumentView::setInstrumentView()
{
    setMinimumSize(InstrumentViewItem::getNumItems()*80,200);
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();
}

void InstrumentView::setHeaders()
{
     _header << "TickerID"
             << "InstrumentId"
             << "Exchange"
             << "Last"
             << "LastSize"
             << "Bid"
             << "BidSize"
             << "Ask"
             << "AskSize"
             << "Open"
             << "Close"
             << "High"
             << "Low"
             << "Volume";
     setHorizontalHeaderLabels(_header);
}

InstrumentView::~InstrumentView()
{}

InstrumentViewItem* InstrumentView::getInstrumentViewItem(const TickerId tickerId)
{
    if(_tickerIdToItemMap.count(tickerId)!=0)
    {
        return _tickerIdToItemMap[tickerId];
    }
    return NULL;
}


void InstrumentView::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
    if(instrumentItem)
    {
        instrumentItem->updateLastPrice(tradeUpdate.lastPrice);
        instrumentItem->updateLastSize(tradeUpdate.lastSize);
    }
}

void InstrumentView::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
    if(instrumentItem)
    {
        instrumentItem->updateBidPrice(quoteUpdate.bidPrice);
        instrumentItem->updateBidSize(quoteUpdate.bidSize);
        instrumentItem->updateAskPrice(quoteUpdate.askPrice);
        instrumentItem->updateAskSize(quoteUpdate.askSize);
    }
}

void InstrumentView::insertInstrumentItem(InstrumentViewItem* item)
{
    int currentRow = _numRows++;
    insertRow(currentRow);
    int numItems = InstrumentViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        setItem(currentRow,i,item->getTableItem(i));
    }
}

/*void InstrumentView::addInstrument(const TickerId tickerId, const Contract& contract)
{
    InstrumentViewItem* instrumentItem = new InstrumentViewItem();
    //QTableWidgetItem* firstItem = instrumentItem->getItemPointer();
    int numItems = instrumentItem->getNumItems();
    #pragma omp critical (InstrumentViewmap)
    {
        _tickerIdToItemMap[tickerId] = firstItem;


    #pragma omp critical(InstrumentView)
    {
        int row = _numRows++;
        insertRow(row);
        for(int i=0;i<numItems;++i)
        {
            setItem(row,i,firstItem+i);
        }
        item(row, InstrumentView::TickerID)->setText(QString("%d").arg(tickerId));
        item(row, InstrumentView::InstrumentId)->setText(QString::fromStdString(contract.symbol));
        item(row, InstrumentView::Exchange)->setText(QString::fromStdString(contract.exchange));;
    }
}
*/

void InstrumentView::updateTickPrice(const TickerId tickerId, const TickType tickType, const double price, const int canAutoExecute)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->updateOpenPrice(price); break;
            case CLOSE: instrumentItem->updateClosePrice(price); break;
            case HIGH: instrumentItem->updateHighPrice(price); break;
            case LOW: instrumentItem->updateLowPrice(price); break;
            case BID: instrumentItem->updateBidPrice(price); break;
            case ASK: instrumentItem->updateAskPrice(price); break;
            case LAST: instrumentItem->updateLastPrice(price); break;
            default: break;
        }
    }

}

void InstrumentView::updateTickSize(const TickerId tickerId , const TickType tickType, const int size)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
    if(instrumentItem)
    {
         switch(tickType)
        {
            case BID_SIZE:instrumentItem->updateBidSize(size); break;
            case ASK_SIZE:instrumentItem->updateAskSize(size); break;
            case LAST_SIZE:instrumentItem->updateLastSize(size);break;
            default: break;
        }
    }
}

void InstrumentView::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    InstrumentViewItem* instrumentItem = getInstrumentViewItem(tickerId);
    if(instrumentItem)
    {
        switch(tickType)
        {
            case OPEN: instrumentItem->updateOpenPrice(value); break;
            case CLOSE: instrumentItem->updateClosePrice(value); break;
            case HIGH: instrumentItem->updateHighPrice(value); break;
            case LOW: instrumentItem->updateLowPrice(value); break;
            case BID: instrumentItem->updateBidPrice(value); break;
            case ASK: instrumentItem->updateAskPrice(value); break;
            case LAST: instrumentItem->updateLastPrice(value); break;
            case BID_SIZE: instrumentItem->updateBidSize(value); break;
            case ASK_SIZE: instrumentItem->updateAskSize(value); break;
            case LAST_SIZE: instrumentItem->updateLastSize(value);break;
            case VOLUME: instrumentItem->updateVolume(value); break;
            default: break;
        }
    }
}

void InstrumentView::addInstrument(const TickerId tickerId, const Contract& contract)
{
<<<<<<< HEAD
    if(_tickerIdToItemMap[tickerId]==0)
    {
        InstrumentViewItem* instrumentItem = new InstrumentViewItem();
        insertInstrumentItem(instrumentItem);
        _tickerIdToItemMap[tickerId] = instrumentItem;

        instrumentItem->setInstrumentID(contract.symbol);
        instrumentItem->setExchange(contract.exchange);
        instrumentItem->setTickerId(tickerId);
    }
=======
    InstrumentViewItem* instrumentItem = new InstrumentViewItem();
    insertInstrumentItem(instrumentItem);
    _tickerIdToItemMap[tickerId] = instrumentItem;

    instrumentItem->setInstrumentID(contract.symbol);
    instrumentItem->setExchange(contract.exchange);
    instrumentItem->setTickerId(tickerId);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}
