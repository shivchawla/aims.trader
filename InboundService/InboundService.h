#ifndef INBOUNDSERVICE_H
#define INBOUNDSERVICE_H

#include <QObject>
#include <QtCore/QCoreApplication>
#include <QThread>
#include <QHash>
#include <ActiveTickServerAPI.h>
#include <iostream>
#include <stdio.h>

#include "stdafx.h"
#include <QMutex>
#include <QWaitCondition>
#include <QDateTime>

using namespace std;

class APISession;
class Requestor;
class QTimer;

class InboundService : public QObject
{
private:
    Q_OBJECT

    APISession* sessionp;
    Requestor* requestorp;
    QTimer* timer;
	//Streamer streamer;

    //QMutex mutex;
    //QWaitCondition condition;

    //bool isConnected;

	void Logon(string serverAddress, string apiUserId, string userId, string password);
    bool IsIgnoreCase(QDateTime startDate, QDateTime endDate);
    void ScheduleNextRun();
    //void SetNewHistoryStartDate();

public:
    explicit InboundService(QObject *parent = 0);
    ~InboundService();
	void Init();
	void Shutdown();
	void InvokeService();
    void setConnection();

//signals:
  //  void SignalInvocation();

public slots:
	void StartInbound();
};

#endif //INBOUNDSERVICE_H
