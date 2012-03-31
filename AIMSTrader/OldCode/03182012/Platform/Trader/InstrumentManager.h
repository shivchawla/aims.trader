#ifndef INSTRUMENTMANAGER_H
#define INSTRUMENTMANAGER_H
#include <map>
#include "Platform/typedefs.h"
#include "Platform/Shared/Contract.h"
#include <QObject>
class Instrument;

typedef std::map<TickerId, Instrument*> InstrumentMap;
typedef std::map<long, TickerId> ContractIdToTickerId;

class InstrumentManager : public QObject
{
    Q_OBJECT
    private:
        InstrumentMap _instruments;
        TickerId _tickerId;
        ContractIdToTickerId _contractIdToTickerId;
    public:
        InstrumentManager();
        ~InstrumentManager();

    public slots:
        void setBid(const TickerId, const double bid);
        void setAsk(const TickerId, const double ask);
        void setLast(const TickerId, const double last);
        void setHigh(const TickerId, const double high);
        void setClose(const TickerId, const double close);
        void setLow(const TickerId, const double low);
        void setBidSize(const TickerId, const int size);
        void setAskSize(const TickerId, const int size);
        void setLastSize(const TickerId, const int size);
        void setContractDetails(const TickerId, const ContractDetails&);
        void requestMarketData(const Contract&);
        void removeInstrument(const TickerId);
        void printThreadId();

    signals:
        void requestMarketDataToTA(const TickerId, const Contract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
};

#endif // INSTRUMENTMANAGER_H
