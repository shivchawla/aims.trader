#include "Platform/Utils/SnapshotGenerator.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Utils/Timer.h"
#include <QDebug>

SnapshotGenerator::SnapshotGenerator():TimedObject()
{
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), this, SLOT(start()));
    QObject::connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}

SnapshotGenerator::~SnapshotGenerator()
{
    qDebug()<<"SnapShotGenerator deleted";
}

void SnapshotGenerator::onSnooze()
{
    generateSnapshot(_minuteCounter);
}

void SnapshotGenerator::generateSnapshot(const int minute)
{
    Service::service().getInstrumentManager()->generateSnapshot(minute);
}

void SnapshotGenerator::start()
{
    setSnooze(0.5);
}

