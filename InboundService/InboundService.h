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
class DataManager;
class InboundService
{
    private:
        DataManager* _dataManager;

        bool IsIgnoreCase(QDateTime startDate, QDateTime endDate);
        void scheduleNextRun();
        //void SetNewHistoryStartDate();

    public:
        InboundService();
        ~InboundService();
        void Init();
        void shutdown();
        void InvokeService();
        void setConnection();
        void startDataProcessing();
        void updatePriceHistory();
        void loadNewSymbols();


    //signals:
      //  void SignalInvocation();

    public slots:
        void StartInbound();
};

#endif //INBOUNDSERVICE_H
