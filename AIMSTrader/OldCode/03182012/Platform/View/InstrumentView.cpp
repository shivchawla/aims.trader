#include "Platform/View/InstrumentView.h"
#include "Platform/View/InstrumentViewItem.h"
#include <iostream>
InstrumentView::InstrumentView():TableView<InstrumentView>()
{
    _numRows=0;
    _numCols = InstrumentViewItem::getNumItems();
    init();
}

void InstrumentView::init()
{
     setRowCount(_numRows);
     setColumnCount(_numCols);
     _header << "TickerID"
             << "InstrumentId"
             << "Exchange"
             << "Last"
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

void InstrumentView::addInstrument(const TickerId tickerId, const Contract& contract)
{
    InstrumentViewItem* instrumentItem = new InstrumentViewItem();
    QTableWidgetItem* firstItem = instrumentItem->getItemPointer();
    int numItems = instrumentItem->getNumItems();
    #pragma omp critical (InstrumentViewmap)
    {
        _tickerIdToItemMap[tickerId] = firstItem;
    }

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

void InstrumentView::updateInstrument(const TickerId tickerId, const ContractDetails& contractDetails)
{}

void InstrumentView::updateTickPrice(const TickerId tickerId, const TickType tickType, const double price, const int canAutoExecute)
{
    QTableWidgetItem* tickerIdItem;
    #pragma omp critical (InstrumentViewMap)
    {
        tickerIdItem =_tickerIdToItemMap[tickerId];
    }
    double oldBid,oldAsk,oldLast;
    #pragma omp critical(InstrumentView)
    {
        if(!tickerIdItem)
        {
            int row = tickerIdItem->row();
            switch(tickType)
            {
                case OPEN: item(row, InstrumentView::Open)->setText(QString("%f").arg(price)); break;
                case CLOSE: item(row, InstrumentView::Close)->setText(QString("%f").arg(price)); break;
                case HIGH: item(row, InstrumentView::High)->setText(QString("%f").arg(price)); break;
                case LOW: item(row, InstrumentView::Low)->setText(QString("%f").arg(price)); break;

                case BID:
                oldBid = item(row, InstrumentView::Bid)->text().toDouble();
                item(row, InstrumentView::Bid)->setText(QString("%f").arg(price));
                if(oldBid>price)
                {
                    item(row, InstrumentView::Bid)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Bid)->setForeground(Qt::green);
                }
                break;

                case ASK: oldAsk = item(row, InstrumentView::Ask)->text().toDouble();
                item(row, InstrumentView::Ask)->setText(QString("%f").arg(price));
                if(oldAsk>price)
                {
                    item(row, InstrumentView::Ask)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Ask)->setForeground(Qt::green);
                }

                case Last: oldLast = item(row, InstrumentView::Ask)->text().toDouble();
                item(row, InstrumentView::Last)->setText(QString("%f").arg(price));
                if(oldLast>price)
                {
                    item(row, InstrumentView::Last)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Last)->setForeground(Qt::green);
                }
                break;
                default: break;
            }
        }
    }
}

void InstrumentView::updateTickSize(const TickerId tickerId , const TickType tickType, const int size)
{
    QTableWidgetItem* tickerIdItem;

    #pragma omp critical (InstrumentViewMap)
    {
        tickerIdItem =_tickerIdToItemMap[tickerId];
    }

    #pragma omp critical(InstrumentView)
    {
        if(!tickerIdItem)
        {
            int row = tickerIdItem->row();
            switch(tickType)
            {
                case BID_SIZE: item(row, InstrumentView::BidSize)->setText(QString("%f").arg(size)); break;
                case ASK_SIZE: item(row, InstrumentView::AskSize)->setText(QString("%f").arg(size)); break;
                default: break;
            }
        }
    }
}

void InstrumentView::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    QTableWidgetItem* tickerIdItem;

    #pragma omp critical (InstrumentViewMap)
    {
        tickerIdItem =_tickerIdToItemMap[tickerId];
    }
    double oldAsk,oldBid,oldLast;
    #pragma omp critical(InstrumentView)
    {
        if(!tickerIdItem)
        {
            int row = tickerIdItem->row();
            switch(tickType)
            {
                case OPEN: item(row, InstrumentView::Open)->setText(QString("%f").arg(value)); break;
                case CLOSE: item(row, InstrumentView::Close)->setText(QString("%f").arg(value)); break;
                case HIGH: item(row, InstrumentView::High)->setText(QString("%f").arg(value)); break;
                case LOW: item(row, InstrumentView::Low)->setText(QString("%f").arg(value)); break;
                case BID:
                oldBid = item(row, InstrumentView::Bid)->text().toDouble();
                item(row, InstrumentView::Bid)->setText(QString("%f").arg(value));
                if(oldBid>value)
                {
                    item(row, InstrumentView::Bid)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Bid)->setForeground(Qt::green);
                }
                break;
                case ASK: oldAsk = item(row, InstrumentView::Ask)->text().toDouble();
                item(row, InstrumentView::Ask)->setText(QString("%f").arg(value));
                if(oldAsk>value)
                {
                    item(row, InstrumentView::Ask)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Ask)->setForeground(Qt::green);
                }
                case LAST: oldLast = item(row, InstrumentView::Last)->text().toDouble();
                item(row, InstrumentView::Last)->setText(QString("%f").arg(value));
                if(oldLast>value)
                {
                    item(row, InstrumentView::Last)->setForeground(Qt::red);
                }
                else
                {
                    item(row, InstrumentView::Last)->setForeground(Qt::green);
                }
                break;
                case BID_SIZE: item(row, InstrumentView::BidSize)->setText(QString("%f").arg(value)); break;
                case ASK_SIZE: item(row, InstrumentView::AskSize)->setText(QString("%f").arg(value)); break;
                case VOLUME: item(row, InstrumentView::Volume)->setText(QString("%f").arg(value)); break;
                default: break;
            }
        }
    }
}


