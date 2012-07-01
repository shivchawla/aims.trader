#include "DataAccess/DbBase.h"
#include "Utils/BootStrapper.h"
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QThread>
#include <QSettings>
#include <QCoreApplication>

//QSqlDatabase DbBase::db = QSqlDatabase();
DbBase::DbBase()
{
    QString connectionName;
    connectionName.sprintf("%08p", QThread::currentThread());
    init(connectionName);
}

void DbBase::init(const QString connectionName)
{
    if(QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName, false);
        qDebug()<<db.connectionName()<<db.databaseName();
        return;
    }
    //connect to one
    QString path = QCoreApplication::applicationDirPath() + "/InboundService.ini";
    QSettings settings(path, QSettings::IniFormat);
    //qDebug() << settings.allKeys() << endl;
    QString driver = settings.value("database/driver", "QMYSQL").toString();
    QString hostname = settings.value("database/hostname", "localhost").toString();
    int port = settings.value("database/port", 3306).toInt();
    QString database = settings.value("database/database", "StratTrader").toString();
    QString user = settings.value("database/user", "").toString();
    QString pwd = settings.value("database/password", "").toString();

    db  = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(database);
    db.setUserName(user);
    db.setPassword(pwd);

    qDebug()<<db.connectionName()<<db.databaseName();
    //db.open("root","");
    //qDebug() << db.lastError().driverText()<<db.databaseName()<<db.hostName();
    qDebug() << "DbBase: new database connection initialized" << endl;

}

bool DbBase::openDatabase()
{
    bool isOpen;
    //qDebug()<<db.connectionName()<<db.databaseName();

    if (!(isOpen = db.open("root",""))) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        //return NULL;
    }
    else qDebug() << "Database connected!!" << endl;

    return isOpen;
}

QSqlQuery DbBase::getBlankQuery()
{
    return QSqlQuery("", db);
}

DbBase::~DbBase()
{
     qDebug()<<"Db Base Destructor is called";
     if (db.isOpen()) db.close();
}
