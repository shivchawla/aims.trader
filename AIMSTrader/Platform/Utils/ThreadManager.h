#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <list>
#include <QThread>
#include "Platform/Utils/Singleton.h"

class ThreadManager//: public Singleton<ThreadManager>
{
    private:
        QList<QThread*> _threads;

    public:
        ThreadManager();
        static ThreadManager& threadManager()
        {
            static ThreadManager tm;
            return tm;
        }
  public:
     ~ThreadManager();

public:
    static void initialize();
    void startThreads();
    void waitOnThreads();
    void stopThreads();

    //static ThreadManager* Instance();

public:
    QThread* requestThread();
};

#endif // THREADMANAGER_H
