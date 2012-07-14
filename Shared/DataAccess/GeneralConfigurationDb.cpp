#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QUuid>
#include "DataAccess/GeneralConfigurationDb.h"
#include <QStringList>

GeneralConfigurationDb::GeneralConfigurationDb(void):DbBase()
{
}

GeneralConfigurationDb::~GeneralConfigurationDb(void)
{
    qDebug()<<"Configuration Db Deleted";
}

GeneralConfigurationData* GeneralConfigurationDb :: GetConfigurationByKey(QString key) {
    //qDebug() << "Received " << key << endl;

    if (!openDatabase()) {
        return NULL;
    }

    //qDebug()<<db.tables();
    QSqlQuery query = getBlankQuery();
    query.prepare("select ConfKey, ConfValue, Comments from GeneralConfiguration where ConfKey = :ConfKey");
    query.bindValue(":ConfKey", key);
	query.exec();
	qDebug() << "Got " << query.size() << " rows and isValid = " << query.isValid() << endl;
    if (!query.next()) {
        qDebug()<<query.lastError().text();
        query.finish();
        db.close();
		return NULL;
	}
    GeneralConfigurationData *c = new GeneralConfigurationData();

    c->key = query.value(ConfKey).toString();
    c->value = query.value(ConfValue).toString();
    c->comments = query.value(Comments).toString();

    c->printDebug();
    query.finish();
    db.close();

	return c;
}

unsigned int GeneralConfigurationDb :: UpdateConfiguration(GeneralConfigurationData* data, QString key) {
    //qDebug() << "Received " << key << endl;

    if (!openDatabase()) {
        return 0;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("Update GeneralConfiguration Set ConfValue = :ConfValue where ConfKey = :ConfKey");
    query.bindValue(":ConfValue", data->value);
    query.bindValue(":ConfKey", key);
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update GeneralConfiguration for key " << key << endl;
    query.finish();
    db.close();
    return query.size();
}
