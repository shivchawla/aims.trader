// Redirect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRedirect class
#pragma once
#define BUFFER_SIZE 256

#include <QString>
#include <QProcess>
#include <QObject>

class CRedirect : public QObject
{
    Q_OBJECT
// Construction
public:
	CRedirect();
	~CRedirect();

        bool StartChildProcess(bool bShowChildWindow = FALSE);
        bool IsChildRunning() const;
	void TerminateChildProcess();
        void WriteChildStdIn(QString szInput);
protected:

        QProcess *process;

        int ProcessThread();
private slots:
        void readyReadStandardOutput();
        void readyReadStandardError();
signals:
        void OnChildStarted();
        void OnChildStdOutWrite(QString szOutput);
        void OnChildStdErrWrite(QString szOutput);
        void OnChildTerminate();
};
