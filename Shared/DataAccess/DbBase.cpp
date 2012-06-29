#include "DataAccess/DbBase.h"
#include "Utils/BootStrapper.h"
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

DbBase::DbBase(void)
{
	db = QSqlDatabase :: database();
    if (!db.isValid())
        BootStrapper :: InitDatabase();
}


DbBase::~DbBase(void)
{
	if (db.isOpen()) db.close();
}
