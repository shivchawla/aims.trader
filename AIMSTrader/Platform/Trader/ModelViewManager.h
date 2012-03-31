#ifndef MODELVIEWMANAGER_H
#define MODELVIEWMANAGER_H

#include "Platform/typedefs.h"
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Enumerations/TickType.h"
class OpenOrderModel;
class OpenOrderView;
class InstrumentModel;
class InstrumentView;
class Instrument;
class OpenOrder;

class ModelViewManager
{
    private:
        OpenOrderModel* _openOrderModel;
        OpenOrderView* _openOrderView;
        InstrumentModel* _instrumentModel;
        InstrumentView* _instrumentView;

    public:
        ModelViewManager();
        ~ModelViewManager();

    private:
        void init();

    public:
        void removeInstrument(const TickerId);
        void updateInstrument(const TickerId);
        void addInstrument(Instrument*);
        //void removeOpenOrder(const OrderId);
        void addOpenOrder(OpenOrder*);
        void updateOpenOrder(const OrderId);

        void addInstrument(const TickerId, const Contract&);
        void updateInstrument(const TickerId, const ContractDetails&);
        void updateTickPrice(const TickerId, const TickType, const double price, const int canAutoExecute);
        void updateTickSize(const TickerId , const TickType, const int size);
        void updateTickGeneric(const TickerId, const TickType, const double value);

        void addOpenOrder(const OrderId, const Order&, const String&);
        void cancelOpenOrder(const OrderId);
        void removeOpenOrder(const OrderId);
        void updateOpenOrder(const OrderId, const Contract&, const Execution&);
        void updateOpenOrder(const OrderId, const String& status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const String& whyHeld);

};

#endif // MODELVIEWMANAGER_H
