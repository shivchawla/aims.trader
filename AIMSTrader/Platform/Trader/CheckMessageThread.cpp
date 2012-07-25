#include "Platform/Trader/CheckMessageThread.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/View/IOInterface.h"

CheckMessageThread::CheckMessageThread(TraderAssistant* ta): QThread()
{
    _ta = ta;
}

CheckMessageThread::CheckMessageThread()
{}

CheckMessageThread::~CheckMessageThread()
{
    //lock the variables

    _mutex.lock();
    //stop any processing
    terminate();
    _mutex.unlock();

    wait();
}

void CheckMessageThread::run()
{
    ioInterface()->reportEvent("TraderAssistant", "Starting thread to check messages from IB", INFO);
    while(_ta->IsConnected())
    {
        _ta->checkMessages();
    }
}
