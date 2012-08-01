#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <list>
#include <QThread>
#include "Platform/Utils/Singleton.h"

class ThreadManager//: public Singleton<ThreadManager>
{
    private:
        std::list<QThread*> _threads;
    //static ThreadManager* _instance;

    public:
        ThreadManager();
//public:
  //  ~ThreadManager();

public:
    static void initialize();
    void startThreads();
    void waitOnThreads();
    //static ThreadManager* Instance();

public:
    QThread* requestThread();
};

static ThreadManager* threadManager()
{
    static ThreadManager* tm = new ThreadManager();
    return tm;
}

#endif // THREADMANAGER_H
