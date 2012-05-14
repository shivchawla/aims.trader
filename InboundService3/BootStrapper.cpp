#include "stdafx.h"
#include "BootStrapper.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <iostream>
#include <QDebug>

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
        db = QSqlDatabase:: addDatabase("QMYSQL");
        //db.connectionName("devAimsTrader");
        db.setHostName("localhost");
		db.setPort(3306);
        db.setDatabaseName("AimsTrader");
		db.setUserName("root");
        db.setPassword("");
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

