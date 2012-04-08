#ifndef CHECKMESSAGETHREAD_H
#define CHECKMESSAGETHREAD_H

#include <QThread>
#include <QMutex>

class CheckMessageThread : public QThread
{
    private:
        QMutex mutex;

    public:
        CheckMessageThread();
        ~CheckMessageThread();

    protected:
        void run();
};

#endif // CHECKMESSAGETHREAD_H
