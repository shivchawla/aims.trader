#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "DbBase.h"
#include "Data/DailyHistoryBarData.h"

class DailyHistoryBarDb : public DbBase
{
private:
    enum DailyHistoryBar {
        DailyHistoryBarId,
        HistoryDate,
        Open,
        Close,
        High,
        Low,
        Volume,
        UpdatedBy,
        UpdatedDate,
        InstrumentId
	};

	//QSqlDatabase db;

public:
    DailyHistoryBarDb(void);
    ~DailyHistoryBarDb(void);
    DailyHistoryBarData* getDailyHistoryBarById(QUuid id);
    unsigned int insertDailyHistoryBar(const DailyHistoryBarData& data);
    unsigned int insertDailyHistoryBar(QDateTime historyDateTime, float open, float close, float high, float low, qint32 volume, QString updatedBy, QDateTime updatedDate, QUuid instrumentId);
};

