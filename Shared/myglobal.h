#ifndef MYGLOBAL_H
#define MYGLOBAL_H
#include <QFile>

inline QFile* logFile()
{
    static QFile*  fPtr = new QFile("log.txt");
    return fPtr;
}

#endif // MYGLOBAL_H
