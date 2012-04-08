#ifndef INSTRUMENTVIEWITEM_H
#define INSTRUMENTVIEWITEM_H
#include "Platform/View/TableItem.h"
#include "Platform/typedefs.h"

enum InstrumentModelColumn{
       TickerID = 0,
       InstrumentId,
       Exchange,
       Last,
       LastSize,
       Bid,
       BidSize,
       Ask,
       AskSize,
       Open,
       Close,
       High,
       Low,
       Volume
};

class InstrumentViewItem// : public QTableWidgetItem
{
    private:
        const static int _numItems = 14;
        TableItem _items[_numItems];

        double _oldLastPrice;
        double _oldAskPrice;
        double _oldBidPrice;

    public:
        InstrumentViewItem();
        ~InstrumentViewItem();

    public:
       const static int getNumItems();

       void updateHighPrice(const double);
       void updateLowPrice(const double);
       void updateClosePrice(const double);
       void updateOpenPrice(const double);
       void updateLastPrice(const double);
       void updateLastSize(const int);
       void updateAskSize(const int);
       void updateAskPrice(const double);
       void updateBidSize(const int);
       void updateBidPrice(const double);
       void updateVolume(const long);


       void setInstrumentID(const std::string&);
       void setExchange(const std::string&);
       void setTickerId(const TickerId);

       TableItem* getTableItem(const int);
};

#endif // INSTRUMENTVIEWITEM_H
