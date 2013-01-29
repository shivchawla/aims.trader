#ifndef INSTRUMENTDATA_H
#define INSTRUMENTDATA_H

#include <QString>
#include <myglobal.h>

typedef uint InstrumentId;
typedef uint DbStrategyId;

class InstrumentData
{
    public:
        //member variables
        InstrumentId instrumentId;
        QString symbol;
        QString shortName;
        QString fullName;
        quint8 type;
        QString exchangeCode;
        QString countryCode;
        QString sectorCode;

        //ctor
        InstrumentData();
        InstrumentData(InstrumentData* copy);
        ~InstrumentData();

        const QString getSecurityType()
        {
            //right now we assume that we have only one vendor i.e ActiveTick for which vendor code = 0
            //later we can add more here or send this to db
            switch(type)
            {
                case 0:
                    return "STK";
                    break;

                case 1:
                    return "FUTURE";
                    break;

                case 2:
                    return "OPTION";
                    break;
            }
        }


        void printDebug();
    //methods
	
};

typedef InstrumentData InstrumentContract;

typedef uint DbSpreadId;
struct SpreadData
{
    DbSpreadId spreadId;
    InstrumentId instrumentId1;
    InstrumentId instrumentId2;
};


#endif //INSTRUMENTDATA_H

