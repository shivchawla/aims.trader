#include <QByteArray>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QUuid>
#include "DataAccess/ConfigurationDb.h"

ConfigurationDb::ConfigurationDb(void)
{
}

ConfigurationDb::~ConfigurationDb(void)
{
}

ConfigurationData* ConfigurationDb :: GetConfigurationByKey(QString key) {
	qDebug() << "Received " << key << endl;

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return NULL;
    }
    else qDebug() << "Database connected!!" << endl;

    QSqlQuery query;
    query.prepare("select configuration_id, conf_key, conf_value from Configuration where CONF_KEY = :CONF_KEY");
	query.bindValue(":CONF_KEY", key); 
	query.exec();
	qDebug() << "Got " << query.size() << " rows and isValid = " << query.isValid() << endl;
    if (!query.next()) {
		query.finish();
		db.close();
		return NULL;
	}
	ConfigurationData *c = new ConfigurationData();

    c->configurationId = QUuid::fromRfc4122(query.value(CONFIGURATION_ID).toByteArray());
	c->key = query.value(CONF_KEY).toString();
	c->value = query.value(CONF_VALUE).toString();

    qDebug() << c->configurationId << " " << c->key << " " << c->value << endl;
    query.finish();
    db.close();

	return c;
}

unsigned int ConfigurationDb :: UpdateConfiguration(ConfigurationData* data, QString key) {
    qDebug() << "Received " << key << endl;

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return 0;
    }

    QSqlQuery query;
    query.prepare("Update Configuration Set CONF_VALUE = :CONF_VALUE where CONF_KEY = :CONF_KEY");
    query.bindValue(":CONF_VALUE", data->value);
    query.bindValue(":CONF_KEY", key);
    bool result = query.exec();
    if (!result)
        qDebug() << "Could not update Configuration for key " << key << endl;
    query.finish();
    db.close();
    return query.size();
}
