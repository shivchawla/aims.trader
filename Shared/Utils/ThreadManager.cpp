#include "Utils/ThreadManager.h"
#include <iostream>

ThreadManager::ThreadManager()
{}

ThreadManager::~ThreadManager()
{
//    std::list<QThread*>::iterator it;
//    std::list<QThread*>::iterator end = _threads.end();
//    for ( it=_threads.begin() ; it != end; ++it)
//    {
//        (*it)->stop
//        delete *it;
//    }

//    foreach (QThread* thread, _threads)
//    {
//        thread->exit();
//        delete thread;
//    }

    stopThreads();

}

QThread* ThreadManager::requestThread()
{
    QThread* thread = new QThread();
    _threads.append(thread);
    return thread;
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}

void ThreadManager::initialize()
{}

void ThreadManager::startThreads()
{
//    std::list<QThread*>::iterator it;
//    printf("\nNumber of threads %d",_threads.size());
//    std::list<QThread*>::iterator end = _threads.end();
//    for ( it=_threads.begin() ; it != end; ++it)
//    {
//         (*it)->start();
//    }
    foreach (QThread* thread, _threads)
    {
        thread->start();
    }
}

void ThreadManager::waitOnThreads()
{
//    std::list<QThread*>::iterator it;
//    std::list<QThread*>::iterator end = _threads.end();
//    for ( it=_threads.begin() ; it != end; ++it)
//    {
//        (*it)->wait();
//    }
}

void ThreadManager::stopThreads()
{
    foreach (QThread* thread, _threads)
    {
        thread->exit();
    }

//    foreach (QThread* thread, _threads)
//    {
//        thread->wait();
//    }
}

