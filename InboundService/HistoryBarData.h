#ifndef HISTORYBARDATA_H
#define HISTORYBARDATA_H

#pragma once

//#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

class HistoryBarData// : public QObject
{
private:
    //Q_OBJECT

public:
	QUuid historyBarId;
	QDateTime historyDate;
	float open;
	float close;
	float high;
	float low;
	qint32 volume;
	QString updatedBy;
	QDateTime updatedDate;
	QUuid instrumentId;

	HistoryBarData(void);
	~HistoryBarData(void);
    HistoryBarData(HistoryBarData *copy);
};

#endif //HISTORYBARDATA_H
