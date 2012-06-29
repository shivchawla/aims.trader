#pragma once
#ifndef HELPER_H
#define HELPER_H

#include <../API/ActiveTickFeed/include/Shared/ATServerAPIDefines.h>
#include "AimsTraderDefs/typedefs.h"
namespace AimsTrader{

class Converter
{
    public:
        static String ConvertString(const wchar16_t* src, uint32_t count);

};

}

#endif // HELPER_H
