#ifndef AIMSTRADERGLOBAL_H
#define AIMSRADERGLOBAL_H
#include <framework.h>

DatabaseSession* dbSession()
{
    static DatabaseSession* dbSession = new DatabaseSession();
    return dbSession;
}

//Service* service()
//{
//    static Service* _service = new Service();
//    return _service;
//}


#endif // AIMSTRADERGLOBAL_H
