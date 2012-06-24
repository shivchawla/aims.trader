#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "Data/ConfigurationData.h"
#include "DbBase.h"

class ConfigurationDb : public DbBase
{
private:
	enum Instrument {
	CONFIGURATION_ID,
	CONF_KEY,
    CONF_VALUE
	};

public:
	ConfigurationDb(void);
	~ConfigurationDb(void);
	ConfigurationData* GetConfigurationByKey(QString key);
    unsigned int UpdateConfiguration(ConfigurationData *data, QString id);
};

