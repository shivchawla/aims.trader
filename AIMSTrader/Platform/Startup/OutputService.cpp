#include "Platform/Startup/OutputService.h"
#include "Platform/View/OutputInterface.h"
#include "Platform/Startup/Service.h"
#include "Platform/Reports/EventReport.h"


//OutputService* OutputService::_instance = NULL;

OutputService::OutputService()
{
    _outputInterface = new OutputInterface();
}

OutputService::~OutputService()
{
    delete _outputInterface;
}

OutputInterface* OutputService::getOutputInterface()
{
    return _outputInterface;
}

void OutputService::reportEvent(const String& message)
{
    //Service::Instance()->getEventReport()->report("OutputService", message);
}

void OutputService::setOutputObjects()
{
    _outputInterface = new OutputInterface();
}




