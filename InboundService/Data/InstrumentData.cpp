#include "InstrumentData.h"

InstrumentData::InstrumentData(void)
{
}

InstrumentData::InstrumentData(InstrumentData* copy) {
    instrumentId = copy->instrumentId;
    symbol = copy->symbol;
    shortName = copy->shortName;
    fullName = copy->fullName;
    type = copy->type;
    updatedBy = copy->updatedBy;
    updatedDate = copy->updatedDate;
}

InstrumentData::~InstrumentData(void)
{
}