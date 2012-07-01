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

    public:
        DbBase();
        ~DbBase();

    private:
        void init(const QString connectionName);

    protected:
        bool openDatabase();
        QSqlQuery getBlankQuery();

};


