#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategyconfigurationdata.h"

class StrategyConfigurationDb : public DbBase
{
    private:
        enum StrategyConfiguration
        {
            StrategyName,
            ConfKey,
            ConfValue
        };

    public:
        StrategyConfigurationDb();

        //Methods
        StrategyConfigurationData getStrategyConfiguration(const QString& strategyName, QString confKey);
        //QList<StrategyConfigurationData*> getStrategyConfigurations(uint strategyId);
        QHash<QString, QString> getStrategyConfigurations(const QString& strategyName);

        /*uint insertStrategyConfiguration(const StrategyConfigurationData& data);
        uint insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue);
        uint insertStrategyConfigurations(const QList<StrategyConfigurationData>& list);
        uint updateStrategyConfiguration(const StrategyConfigurationData& data);
        uint updateStrategyConfiguration(const uint &strategyId, const QString &key, const QString &value);
        uint deleteStrategyConfiguration(uint strategyId, QString confKey);*/

};
