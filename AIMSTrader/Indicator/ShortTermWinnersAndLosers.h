#ifndef SHORTTERMWINNERSANDLOSERS_H
#define SHORTTERMWINNERSANDLOSERS_H

#include "Platform/Indicator/Indicator.h"
#include <QReadWriteLock>
#include <map>
#include <vector>

struct PriceSnapshot
{
    double current;
    double past;
};
//struct ReturnSnapshot
//{
//    double returns;
//    TickerId tickerId;
//    ReturnSnapshot()
//    {
//        returns=0;
//        tickerId=0;
//    }

//    bool operator < (const ReturnSnapshot& b) const
//    {
//        return (returns < b.returns);
//    }
//};

class ShortTermWinnersAndLosers : public Indicator
{
    private:
        int _numInstruments;
        int _timeScale;
        std::map<TickerId, PriceSnapshot> _priceSnapshots;

        std::vector<ReturnSnapshot> _returnSnapshots;
        int _updateCount;
        int _minuteCount;

    public:
         ShortTermWinnersAndLosers();
        ~ShortTermWinnersAndLosers();

         void setNumInstruments(const int numInstruments)
         {
             _numInstruments = numInstruments;
         }

    public slots:
         void OnSnapshotUpdate(const TickerId tickerId, const double snapshot, const int);

    public slots:
         void startIndicator();

    public:
        void calculate();
};

#endif // SHORTTERMWINNERSANDLOSERS_H

