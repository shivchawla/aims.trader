#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class DbBase //: public QSqlDatabase//: public QObject
{
    protected:
        QSqlDatabase db;
        QString _username;
        QString _password;
    public:
        DbBase();
        ~DbBase();

    private:
        void init(const QString connectionName);

    protected:
        bool openDatabase();
        QSqlQuery getBlankQuery();

};


