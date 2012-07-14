#include "Data/InstrumentData.h"
#include <QDebug>

InstrumentData::InstrumentData()
{
}

InstrumentData::InstrumentData(InstrumentData* copy) {
    instrumentId = copy->instrumentId;
    symbol = copy->symbol;
    shortName = copy->shortName;
    fullName = copy->fullName;
    type = copy->type;
    exchangeCode = copy->exchangeCode;
    countryCode = copy->countryCode;
    sectorCode = copy->sectorCode;
}

InstrumentData::~InstrumentData()
{
}

void InstrumentData::printDebug() {
    qDebug() << instrumentId << " " << symbol << " " << shortName << " " << fullName << " " << type << " " << exchangeCode << " " << countryCode << " " << sectorCode << endl;
}
