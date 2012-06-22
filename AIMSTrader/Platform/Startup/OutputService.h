#pragma once
#ifndef OUTPUTSERVICE_H
#define OUTPUTSERVICE_H

#include "Platform/typedefs.h"
#include "Platform/Utils/Singleton.h"

class OutputInterface;

class OutputService: public Singleton<OutputService>
{
    friend class Singleton<OutputService>;
    private:
        OutputInterface* _outputInterface;

    private:
        OutputService();

    public:
        ~OutputService();

    public:
        OutputInterface* getOutputInterface();
        void reportEvent(const String& message);
        void setOutputObjects();
};

#endif // OUTPUTSERVICE_H
