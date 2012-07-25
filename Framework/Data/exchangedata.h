#ifndef EXCHANGEDATA_H
#define EXCHANGEDATA_H
#include<QString>

class ExchangeData //: public QObject
{
    //Q_OBJECT
public:
	//member variables
    QString code;
	QString name;

	//ctor
	ExchangeData(void);
    ExchangeData(const ExchangeData* copy);
	~ExchangeData(void);

	//methods
    void printDebug();
};

#endif //EXCHANGEDATA_H
