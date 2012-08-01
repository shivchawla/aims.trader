#include "Platform/Utils/SnapshotGenerator.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Utils/Timer.h"

SnapshotGenerator::SnapshotGenerator():TimedObject()
{
    QThread* thread = threadManager()->requestThread();
    moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), this, SLOT(start()));
}

SnapshotGenerator::~SnapshotGenerator()
{}

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
    setSnooze(0.1);
}

