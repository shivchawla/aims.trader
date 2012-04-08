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

class Strategy;
class ShortTermWinnersAndLosers : public Indicator
{
    private:
        std::map<TickerId, PriceSnapshot> _priceSnapshots;
        int _numInstruments;

        struct ReturnSnapshot
        {
            double returns;
            TickerId tickerId;
            ReturnSnapshot()
            {
                returns=0;
                tickerId=0;
            }

            bool operator < (const ReturnSnapshot& b) const
            {
                return (returns < b.returns);
            }
        };

        std::vector<ReturnSnapshot> _returnSnapshots;
        int _updateCount;
        int _minuteCount;

    public:

         ShortTermWinnersAndLosers(Strategy*);
        ~ShortTermWinnersAndLosers();

    public:
         void setNumInstruments(const int);

    public slots:
         void startIndicator();

    public:
        void calculate();

    public slots:
         void updateOneMinuteSnapShot(const TickerId , const double);

};

#endif // SHORTTERMWINNERSANDLOSERS_H
