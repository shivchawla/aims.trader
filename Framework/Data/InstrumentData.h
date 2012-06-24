#ifndef INSTRUMENTDATA_H
#define INSTRUMENTDATA_H

#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

class InstrumentData : public QObject
{
    Q_OBJECT
public:
	//member variables
	QUuid instrumentId;
	QString symbol;
	QString shortName;
	QString fullName;
    QChar type;
	QString updatedBy;
	QDateTime updatedDate;

	//ctor
	InstrumentData(void);
    InstrumentData(InstrumentData* copy);
	~InstrumentData(void);

	//methods
	
};

#endif //INSTRUMENTDATA_H

