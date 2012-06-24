#include "framework.h"
#include <QDebug>


Framework::Framework()
{
    qDebug() << "Invoking framework from shared library!!" << endl;
}

void Framework :: sayHello() {
    qDebug() << "Hello Micky, Sid and Mona!!" << endl;
}
