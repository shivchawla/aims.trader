#pragma once
#ifndef INSTRUMENTVIEWITEM_H
#define INSTRUMENTVIEWITEM_H
#include "Platform/View/TableCellItem.h"
#include "AimsTraderDefs/typedefs.h"
#include "Platform/Model/InstrumentModel.h"
#include "Platform/View/TableViewItem.h"

class InstrumentViewItem : public TableViewItem<InstrumentViewItem>
{
         double _oldLastPrice;
         double _oldAskPrice;
         double _oldBidPrice;
         TickerId _tickerId;

    public:
        InstrumentViewItem(const int numItems);
        ~InstrumentViewItem();

       void updateLastPrice(const double, const int);
       void updateAskPrice(const double, const int);
       void updateBidPrice(const double, const int);
       void setTickerId(const TickerId tickerId)
       {
           _tickerId = tickerId;
       }

       const TickerId getTickerId() const
       {
           return _tickerId;
       }

};

#endif // INSTRUMENTVIEWITEM_H
