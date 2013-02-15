#ifndef STRATEGYBUYLISTDATA_H
#define STRATEGYBUYLISTDATA_H
#include <QDateTime>

struct StrategyBuyListData
{
        //member variables
        uint instrumentId;
        QDateTime deactivatedDate;

        //supporting member variables
        QString strategyName;
        quint8 instrumentType;

};

#endif //STRATEGYBUYLISTDATA_H
