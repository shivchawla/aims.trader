#pragma once
#ifndef ONEMINUTEMOMENTUM_H
#define ONEMINUTEMOMENTUM_H
#include "Platform/Strategy/Strategy.h"

class OneMinuteMomentum: public Strategy
{
    private:
         String _index;
         std::vector<String> _instruments;
         int _timeScale;
         int _numInstruments;

    public:
        OneMinuteMomentum();
        OneMinuteMomentum(const String&);
        ~OneMinuteMomentum();

    /*public:
        static int addStrategy()
        {
            Strategy::registerStrategy(new OneMinuteMomentum());
            return 1;
        }*/

    public:
        const std::vector<String>& getInstruments();
        void initialize();

    protected:
        void setup(const std::vector<String>&, const int);
        void setup(const String& index, const int);

     public slots:
        void startStrategy();

    private:
        void setupIndicator();

};

#endif // ONEMINUTEMOMENTUM_H
