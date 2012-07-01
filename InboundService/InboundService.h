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
#include <QDateTime>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>
#include <DataAccess/ConfigurationDb.h>
#include <DataAccess/InstrumentDb.h>

using namespace std;
class DataManager;

class InboundService: public QObject
{
    Q_OBJECT
    private:
        QTimer timer;

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

    public slots:
        void StartInbound();
};

#endif //INBOUNDSERVICE_H
