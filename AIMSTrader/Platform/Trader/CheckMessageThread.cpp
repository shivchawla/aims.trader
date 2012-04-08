#include "Platform/Trader/CheckMessageThread.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/Startup/Service.h"

CheckMessageThread::CheckMessageThread(): QThread()
{
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
    while(Service::Instance()->getTrader()->getTraderAssistant()->IsConnected())
    {
        Service::Instance()->getTrader()->getTraderAssistant()->checkMessages();
    }
}
