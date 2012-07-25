#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>


enum TaskType
{
    INSERT,
    UPDATE,
    DELETE,
    READ
};

class DbBase
{
    protected:
        QSqlDatabase db;
        QString _username;
        QString _password;

    public:
        DbBase();
        virtual ~DbBase();

    private:
        void init(const QString connectionName);

    protected:
        bool openDatabase();
        QSqlQuery getBlankQuery();

};


