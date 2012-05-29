#ifndef OUTPUTSERVICE_H
#define OUTPUTSERVICE_H

#include "Platform/typedefs.h"

class OutputInterface;

class OutputService
{
    private:
        static OutputService* _instance;

    private:
        OutputInterface* outputInterface;

    private:
        OutputService();

    public:
        ~OutputService();

    public:
        OutputInterface* getOutputInterface();
        void reportEvent(const String& message);
        static OutputService* Instance();
        void setOutputObjects();

};

#endif // OUTPUTSERVICE_H
