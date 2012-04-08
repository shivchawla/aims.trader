#include "Platform/Utils/Converter.h"
#include <QChar>
namespace AimsTrader{


/*static*/ String Converter::ConvertString(const wchar16_t* src, uint32_t count)
{
    String dest;
    dest.reserve(count + 1);

    for(uint32_t i = 0; i < count; i++)
        dest.insert(i, (QChar)(src[i]));

    dest.push_back(QChar(0));

    return dest;
}

}
