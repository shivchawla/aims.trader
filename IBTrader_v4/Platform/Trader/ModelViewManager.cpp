#include "Platform/Trader/ModelViewManager.h"
#include "Platform/Model/InstrumentModel.h"
#include "Platform/Model/OpenOrderModel.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/OpenOrderView.h"

ModelViewManager::ModelViewManager()
{
    //_instrumentModel =new InstrumentModel();
    _instrumentView = new InstrumentView();
    //_openOrderModel = new OpenOrderModel();
    _openOrderView = new OpenOrderView();
    init();
}

void ModelViewManager::init()
{
   // _instrumentView->setModel(_instrumentModel);
    _instrumentView->show();
   // _openOrderView->setModel(_openOrderModel);
    _openOrderView->show();
}

ModelViewManager::~ModelViewManager()
{
    //delete _instrumentModel;
    delete _instrumentView;
    //delete _openOrderModel;
    delete _openOrderView;
}

void ModelViewManager::removeInstrument(const TickerId tickerId)
{
    _instrumentModel->removeInstrument(tickerId);
}

void ModelViewManager::addInstrument(Instrument* instrument)
{
    _instrumentModel->addInstrument(instrument);
}

void ModelViewManager::updateInstrument(const TickerId tickerId)
{
    _instrumentModel->updateInstrument(tickerId);
}

void ModelViewManager::addOpenOrder(OpenOrder* openOrder)
{
    _openOrderModel->addOpenOrder(openOrder);
}

void ModelViewManager::updateOpenOrder(const OrderId orderId)
{
    _openOrderModel->updateOpenOrder(orderId);
}

void ModelViewManager::updateInstrument(const TickerId tickerId, const ContractDetails& contractDetails)
{
   _instrumentView->updateInstrument(tickerId, contractDetails);
}

void ModelViewManager::updateTickPrice(const TickerId tickerId, const TickType tickType, const double price, const int canAutoExecute)
{
    _instrumentView->updateTickPrice(tickerId, tickType, price, canAutoExecute);
}

void ModelViewManager::updateTickSize(const TickerId tickerId , const TickType tickType, const int size)
{
    _instrumentView->updateTickSize(tickerId , tickType, size);
}

void ModelViewManager::updateTickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    _instrumentView->updateTickGeneric(tickerId, tickType, value);
}

void ModelViewManager::addOpenOrder(const OrderId orderId, const Order& order, const String& strategyName)
{
    _openOrderView->addOpenOrder(orderId, order, strategyName);
}

void ModelViewManager::cancelOpenOrder(const OrderId orderId)
{
    _openOrderView->cancelOpenOrder(orderId);
}

void ModelViewManager::removeOpenOrder(const OrderId orderId)
{
    _openOrderView->removeOpenOrder(orderId);
}

void ModelViewManager::updateOpenOrder(const OrderId orderId, const Contract& contract, const Execution& execution)
{
    _openOrderView->updateOpenOrder(orderId, contract, execution);
}

void ModelViewManager::updateOpenOrder(const OrderId orderId, const String& status, int filled, int remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const String& whyHeld)
{
   _openOrderView->updateOpenOrder(orderId, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld);
}

void ModelViewManager::addInstrument(const TickerId tickerId, const Contract& contract)
{
    _instrumentView->addInstrument(tickerId, contract);
}
