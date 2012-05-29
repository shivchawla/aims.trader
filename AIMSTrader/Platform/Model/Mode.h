#ifndef MODE_H
#define MODE_H
#include <string>
#define NUMBEROFMODES 4

enum Mode {
    Trade = 0,
    BackTest,
    ForwardTest,
    Optimization,
    Test
};

inline const char* getModeName(const Mode mode)
{
    switch(mode)
    {
        case Trade:return "Trade"; break;
        case BackTest:return "BackTest"; break;
        case ForwardTest:return "ForwardTest"; break;
        case Optimization:return "Optimization"; break;
    }
}


#endif // MODE_H
