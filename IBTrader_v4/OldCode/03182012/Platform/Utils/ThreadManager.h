#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <list>
#include <QThread>

class ThreadManager
{
private:
    std::list<QThread*> threads;
    static ThreadManager* _instance;

private:
    ThreadManager();
public:
    ~ThreadManager();

public:
    static void initialize();
    void startThreads();
    void waitOnThreads();
    static ThreadManager* Instance();

public:
    QThread* requestThread();
};

#endif // THREADMANAGER_H
