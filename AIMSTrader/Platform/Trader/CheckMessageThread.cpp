#include "Platform/Trader/CheckMessageThread.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/Startup/Service.h"

<<<<<<< HEAD
CheckMessageThread::CheckMessageThread(TraderAssistant* ta): QThread()
{
    _ta = ta;
=======
CheckMessageThread::CheckMessageThread(): QThread()
{
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

CheckMessageThread::~CheckMessageThread()
{
    //lock the variables

    mutex.lock();
    //stop any processing
    terminate();
    mutex.unlock();

    wait();
}

void CheckMessageThread::run()
{
<<<<<<< HEAD
    Service::Instance()->getEventReport()->report("TraderAssistant", "Starting thread to check messages from IB");
    while(_ta->IsConnected())
    {
        _ta->checkMessages();
=======
    while(Service::Instance()->getTrader()->getTraderAssistant()->IsConnected())
    {
        Service::Instance()->getTrader()->getTraderAssistant()->checkMessages();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    }
}
