#pragma once
#include <qthread.h>

class UtilBox : public QThread
{
public:
	UtilBox(void);
	~UtilBox(void);

	static void sleep(unsigned long secs) {
		QThread::sleep(secs);
	}
	static void msleep(unsigned long msecs) {
		QThread::msleep(msecs);
	}
	static void usleep(unsigned long usecs) {
		QThread::usleep(usecs);
	}
};

