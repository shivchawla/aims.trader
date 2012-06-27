#include "Platform/Utils/ThreadManager.h"
#include <iostream>

ThreadManager::ThreadManager()
{}

QThread* ThreadManager::requestThread()
{
    QThread* thread = new QThread();
    _threads.push_back(thread);
    return thread;
}

void ThreadManager::initialize()
{}

void ThreadManager::startThreads()
{
    std::list<QThread*>::iterator it;
    printf("\nNumber of threads %d",_threads.size());
    std::list<QThread*>::iterator end = _threads.end();
    for ( it=_threads.begin() ; it != end; ++it)
    {
         (*it)->start();
    }
}

void ThreadManager::waitOnThreads()
{
    std::list<QThread*>::iterator it;
    std::list<QThread*>::iterator end = _threads.end();
    for ( it=_threads.begin() ; it != end; ++it)
    {
        (*it)->start();
    }
}
