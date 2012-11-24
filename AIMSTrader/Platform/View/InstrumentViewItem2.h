#ifndef INSTRUMENTVIEWITEM2_H
#define INSTRUMENTVIEWITEM2_H

#include "Platform/View/TableViewItem2.h"
#include "AimsTraderDefs/typedefs.h"

class InstrumentViewItem2 : public TableViewItem2
{
    private:
        TickerId _tickerId;
    public:
        InstrumentViewItem2(const TickerId);

    public:
        void setTickerId(const TickerId tickerId)
        {
            _tickerId = tickerId;
        }

        const TickerId getTickerId()
        {
            return _tickerId;
        }
};

#endif // INSTRUMENTVIEWITEM2_H
