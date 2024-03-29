#include "Platform/Utils/ThreadManager.h"
#include <iostream>
ThreadManager* ThreadManager::_instance = NULL;

ThreadManager::ThreadManager()
{}

ThreadManager* ThreadManager::Instance()
{
    if(_instance == NULL)
    {
        _instance = new ThreadManager();
    }
    return _instance;
}

QThread* ThreadManager::requestThread()
{
    QThread* thread = new QThread();
    threads.push_back(thread);
    return thread;
}

void ThreadManager::initialize()
{}

void ThreadManager::startThreads()
{
    std::list<QThread*>::iterator it;
    printf("\nNumber of threads %d",threads.size());
    std::list<QThread*>::iterator end = threads.end();
    for ( it=threads.begin() ; it != end; ++it)
    {
         (*it)->start();
    }
}

void ThreadManager::waitOnThreads()
{
    std::list<QThread*>::iterator it;
    std::list<QThread*>::iterator end = threads.end();
    for ( it=threads.begin() ; it != end; ++it)
    {
        (*it)->start();
    }
}
