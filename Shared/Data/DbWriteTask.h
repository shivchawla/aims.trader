#ifndef DBWRITETASK_H
#define DBWRITETASK_H

#include <QRunnable>
#include <QList>
#include <QScopedPointer>
#include "Utils/Log.h"

template<class Database, class DataType>
class DbWriteTask : public QRunnable
{
    protected:
        QList<DataType*> _data;
        //Database _db;

    public:
        DbWriteTask(QList<DataType*>&);
        ~DbWriteTask();

    public:
        void run();
};

template<class Database, class DataType>
DbWriteTask<Database, DataType>::~DbWriteTask()
{
    int count = _data.length();
    for(int i=0 ; i<count ; ++i)
    {
        delete _data[i];
    }

    _data.clear();

    //delete _db;
}

template<class Database, class DataType>
DbWriteTask<Database, DataType>::DbWriteTask(QList<DataType*>& data)
{
    //_db = new Database();
    _data = data;
    //_id = id;
    //_frequency = frequency;

    setAutoDelete(true);
}

template<class Database, class DataType>
void DbWriteTask<Database, DataType>::run()
{
//    Database db;
//    int n = db.write(_id, _data);
//    log() << QDateTime::currentDateTime() << n << " history records written to "<< db.getName()<<" for instrument id" << _id << endl;
}


#endif // DBWRITETASK_H
