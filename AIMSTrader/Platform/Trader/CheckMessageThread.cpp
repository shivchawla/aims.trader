#include "Platform/Trader/CheckMessageThread.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/Startup/Service.h"

CheckMessageThread::CheckMessageThread(TraderAssistant* ta): QThread()
{
    _ta = ta;
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
    Service::Instance()->getEventReport()->report("TraderAssistant", "Starting thread to check messages from IB");
    while(_ta->IsConnected())
    {
        _ta->checkMessages();
    }
}
