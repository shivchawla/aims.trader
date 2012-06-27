#ifndef DAILYHISTORYBARDATA_H
#define DAILYHISTORYBARDATA_H

//#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

class DailyHistoryBarData// : public QObject
{
private:
    //Q_OBJECT

public:
    QUuid dailyHistoryBarId;
	QDateTime historyDate;
	float open;
	float close;
	float high;
	float low;
	qint32 volume;
	QString updatedBy;
	QDateTime updatedDate;
	QUuid instrumentId;

    DailyHistoryBarData(void);
    ~DailyHistoryBarData(void);
    DailyHistoryBarData(DailyHistoryBarData *copy);
};

#endif //DAILYHISTORYBARDATA_H
