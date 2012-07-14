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

DbBase::DbBase()
{
    QString connectionName;
    connectionName.sprintf("%08p", QThread::currentThread());
    init(connectionName);
}

void DbBase::init(const QString connectionName)
{
    QString path = QCoreApplication::applicationDirPath() + "/Config.ini";
    QSettings settings(path, QSettings::IniFormat);
    //qDebug() << settings.allKeys() << endl;
    QString driver = settings.value("database/driver", "QMYSQL").toString();
    QString hostname = settings.value("database/hostname", "localhost").toString();
    int port = settings.value("database/port", 3306).toInt();
    QString database = settings.value("database/database", "StratTrader").toString();
    _username = settings.value("database/user", "").toString();
    _password = settings.value("database/password", "").toString();

    if (_username == "" || _password == "")
        qDebug() << "Warning: Either the username or password is blank!" << endl;

    if(QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName, false);
        qDebug()<<db.connectionName()<<db.databaseName();
        return;
    }

    db  = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(database);
    db.setUserName(_username);
    db.setPassword(_password);

    qDebug()<<db.connectionName()<<db.databaseName();
    //db.open("root","");
    //qDebug() << db.lastError().driverText()<<db.databaseName()<<db.hostName();
    qDebug() << "DbBase: new database connection initialized" << endl;

}

bool DbBase::openDatabase()
{
    bool isOpen;
    //qDebug()<<db.connectionName()<<db.databaseName();

    if (!(isOpen = db.open(_username,_password))) {
    //if (!(isOpen = db.open())) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
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
