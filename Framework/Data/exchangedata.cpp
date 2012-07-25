#include "exchangedata.h"
#include <QDebug>

ExchangeData :: ExchangeData(void)
{
}

ExchangeData :: ExchangeData(const ExchangeData* copy)
{
    code = copy->code;
	name = copy->name;
}
ExchangeData :: ~ExchangeData(void)
{
}

void ExchangeData :: printDebug() {
    qDebug() << code << " " << name << endl;
}
