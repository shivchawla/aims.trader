#ifndef CHECKMESSAGETHREAD_H
#define CHECKMESSAGETHREAD_H

#include <QThread>
#include <QMutex>
<<<<<<< HEAD
class TraderAssistant;
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

class CheckMessageThread : public QThread
{
    private:
        QMutex mutex;
<<<<<<< HEAD
        TraderAssistant* _ta;

    public:
        CheckMessageThread(TraderAssistant*);
=======

    public:
        CheckMessageThread();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
        ~CheckMessageThread();

    protected:
        void run();
};

#endif // CHECKMESSAGETHREAD_H
