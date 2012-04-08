#ifndef CHECKMESSAGETHREAD_H
#define CHECKMESSAGETHREAD_H

#include <QThread>
#include <QMutex>
class TraderAssistant;

class CheckMessageThread : public QThread
{
    private:
        QMutex mutex;
        TraderAssistant* _ta;

    public:
        CheckMessageThread(TraderAssistant*);
        ~CheckMessageThread();

    protected:
        void run();
};

#endif // CHECKMESSAGETHREAD_H
