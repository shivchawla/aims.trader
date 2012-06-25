//#include "stdafx.h"
#include "BootStrapper.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <iostream>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QStringList>
using namespace std;

BootStrapper::BootStrapper(void)
{
}


BootStrapper::~BootStrapper(void)
{
}
// initializes a shared database connection for all to use
void BootStrapper :: InitDatabase() {
	QSqlDatabase db = QSqlDatabase :: database();
	if (!db.isValid()) {
		//connect to one
        QString path = QCoreApplication::applicationDirPath() + "/InboundService.ini";
        QSettings settings(path, QSettings::IniFormat);
        qDebug() << settings.allKeys() << endl;
        QString driver = settings.value("database/driver", "QMYSQL").toString();
        QString hostname = settings.value("database/hostname", "localhost").toString();
        int port = settings.value("database/port", 3306).toInt();
        QString database = settings.value("database/database", "StratTrader").toString();
        QString user = settings.value("database/user", "root").toString();
        QString pwd = settings.value("database/password", "").toString();

        db = QSqlDatabase:: addDatabase(driver);
        db.setHostName(hostname);
        db.setPort(port);
        db.setDatabaseName(database);
        db.setUserName(user);
        db.setPassword(pwd);
		qDebug() << "Bootstrapper: database initialized" << endl;
	}
}
// Closes the default database connection and terminates it
void BootStrapper :: ShutdownDatabase() {
	QSqlDatabase db = QSqlDatabase :: database();
	if (db.isValid()) {
		QString dbName = db.connectionName();
		//close if open
		if (db.isOpen()) db.close();
		QSqlDatabase::removeDatabase(dbName);
		qDebug() << "Bootstrapper: database shutdown" << endl;
	}
}

