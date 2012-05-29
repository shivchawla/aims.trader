#include "Platform/Startup/OutputService.h"
#include "Platform/View/OutputInterface.h"
#include "Platform/Startup/Service.h"
#include "Platform/Reports/EventReport.h"


OutputService* OutputService::_instance = NULL;

OutputService::OutputService()
{
    outputInterface = new OutputInterface();
}

OutputService::~OutputService()
{}

OutputInterface* OutputService::getOutputInterface()
{
    return outputInterface;
}

void OutputService::reportEvent(const String& message)
{
    Service::Instance()->getEventReport()->report("OutputService", message);
}

OutputService* OutputService::Instance()
{
    if(!_instance)
    {
        _instance = new OutputService();
    }

    return _instance;
}

void OutputService::setOutputObjects()
{
    outputInterface = new OutputInterface();
}




