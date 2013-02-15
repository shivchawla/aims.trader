#ifndef STRATEGYDATA_H
#define STRATEGYDATA_H
#include<QString>
#include<QDateTime>

class StrategyData
{
     public:
        //member variables
        QString strategyName;
        QString parentStrategyName;
        QDateTime since;
        bool usedInTrading;
        bool newPositionFlag;
        QDateTime lastUpdated;

        //ctor
        StrategyData();

        //methods
        void printDebug();
};

#endif //STRATEGYDATA_H
