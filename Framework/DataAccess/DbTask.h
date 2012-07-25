#ifndef DBTASK_H
#define DBTASK_H
#include <QRunnable>
#include "DataAccess/DbBase.h"

template<class OutputData, class Database, class Data>
class DbTask : public QRunnable
{
    Data* _data;
    TaskType _type;
    OutputData _output;

    public:
        DbTask()
        {}

        DbTask(const Data* data, const TaskType type):QRunnable()
        {
            _data = new Data(data);
            _type = type;
        }

        DbTask(const TaskType type):QRunnable()
        {
            _type = type;
        }

    public:
        void run()
        {
            Database db;
            switch(_type)
            {
                case INSERT: _output = db.insertRow(_data); break;
                case UPDATE: _output = db.updateRow(_data); break;
                case DELETE: _output = db.deleteRow(_data); break;
            }
        }
};

#endif // DBWRITETASK_H
