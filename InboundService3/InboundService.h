#ifndef INBOUNDSERVICE_H
#define INBOUNDSERVICE_H

#include <QObject>
#include <QtCore/QCoreApplication>
#include <QThread>
#include <QHash>
#include <ActiveTickServerAPI.h>
#include <iostream>
#include <stdio.h>

//#include "Helper.h"
//#include "Requestor.h"
//#include "Session.h"
//#include "Streamer.h"
//#include "ConfigurationDb.h"
//#include "InstrumentDb.h"
#include "stdafx.h"
#include <QMutex>
#include <QWaitCondition>

using namespace std;

class APISession;
class Requestor;

class InboundService
{
    private:

    APISession* sessionp;
    Requestor* requestorp;
	//Streamer streamer;
	void Logon(string serverAddress, string apiUserId, string userId, string password);
    //void SetNewHistoryStartDate();

public:
    explicit InboundService();
    ~InboundService();
	void Init();
	void Shutdown();
	void InvokeService();
    void setConnection();

//signals:
  //  void SignalInvocation();

public:
	void StartInbound();
};

#endif //INBOUNDSERVICE_H
