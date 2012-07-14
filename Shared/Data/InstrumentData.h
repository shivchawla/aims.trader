#ifndef INSTRUMENTDATA_H
#define INSTRUMENTDATA_H

#include <QString>

class InstrumentData
{
    public:
        //member variables
        uint instrumentId;
        QString symbol;
        QString shortName;
        QString fullName;
        quint8 type;
        QString exchangeCode;
        QString countryCode;
        QString sectorCode;

        //ctor
        InstrumentData(void);
        InstrumentData(InstrumentData* copy);
        ~InstrumentData(void);

        void printDebug();
	//methods
	
};

#endif //INSTRUMENTDATA_H

