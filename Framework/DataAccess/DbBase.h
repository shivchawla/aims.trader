#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class DbBase : public QObject
{
protected:
	QSqlDatabase db;
public:
	DbBase(void);
	~DbBase(void);
};

