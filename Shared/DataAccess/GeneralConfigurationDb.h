#pragma once
#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/QSqlQuery>
#include "Data/GeneralConfigurationData.h"
#include "DbBase.h"

class GeneralConfigurationDb : public DbBase
{
private:
    enum GeneralConfiguration {
    ConfKey,
    ConfValue,
    Comments
	};

public:
    GeneralConfigurationDb();
    ~GeneralConfigurationDb();
    GeneralConfigurationData getConfigurationByKey(const QString& key);
    unsigned int updateConfiguration(const GeneralConfigurationData& data, const QString& key);
};

