#ifndef PORTFOLIOMANAGER_H
#define PORTFOLIOMANAGER_H

#include "Platform/typedefs.h"
#include <map>

class Position;

class PortfolioManager
{
    private:
        std::map<TickerId, Position*> _totalPositions;

    public:
        void addPosition(const TickerId);
        void

    public:
        PortfolioManager();
        ~PortfolioManager();
};

#endif // PORTFOLIOMANAGER_H
