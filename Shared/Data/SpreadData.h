#ifndef SPREADDATA_H
#define SPREADDATA_H
typedef uint SpreadId;
struct SpreadData
{
    SpreadId spreadId;
    InstrumentId instrumentId1;
    InstrumentId instrumentId2;
};

#endif // SPREADDATA_H
