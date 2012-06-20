#include "exchangedata.h"
#include <QDebug>

ExchangeData :: ExchangeData(void)
{
}

ExchangeData :: ExchangeData(ExchangeData* copy)
{
	exchangeId = copy->exchangeId;
	name = copy->name;
}
ExchangeData :: ~ExchangeData(void)
{
}

void ExchangeData :: printDebug() {
    qDebug() << exchangeId << " " << name << endl;
}
