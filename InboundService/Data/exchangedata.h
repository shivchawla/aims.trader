#ifndef EXCHANGEDATA_H
#define EXCHANGEDATA_H
#include<QUuid>
#include<QString>

class ExchangeData //: public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid exchangeId;
	QString name;

	//ctor
	ExchangeData(void);
	ExchangeData(ExchangeData* copy);
	~ExchangeData(void);

	//methods
    void printDebug();
};

#endif //EXCHANGEDATA_H
