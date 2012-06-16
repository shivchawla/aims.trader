#include "Platform/Trader/CheckMessageThread.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/View/OutputInterface.h"

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
    OutputInterface::Instance()->reportEvent("TraderAssistant", "Starting thread to check messages from IB");
    while(_ta->IsConnected())
    {
        _ta->checkMessages();
    }
}
