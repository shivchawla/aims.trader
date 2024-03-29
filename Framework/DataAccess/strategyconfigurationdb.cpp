#include <QtSql/QSqlError>
#include "strategyconfigurationdb.h"

StrategyConfigurationDb :: StrategyConfigurationDb(void):DbBase()
{}

StrategyConfigurationData StrategyConfigurationDb :: getStrategyConfiguration(const QString& strategyName, QString confKey) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
        return StrategyConfigurationData();
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyName, ConfKey, ConfValue from StrategyConfiguration "
                  "where ConfKey = :ConfKey and StrategyName = :StrategyName");
    query.bindValue(":ConfKey", confKey);
    query.bindValue(":StrategyName", strategyName);

	query.exec();
	qDebug() << "Got " << query.size() << " rows" << endl;
	if (!query.next()) {
		query.finish();
		db.close();
        return StrategyConfigurationData();
	}
    StrategyConfigurationData item;// = new StrategyConfigurationData();
    item.strategyName = query.value(StrategyName).toUInt();
    item.confKey = query.value(ConfKey).toString();
    item.confValue = query.value(ConfValue).toString();

	query.finish();
	db.close();
	return item;
}

QHash<QString, QString> StrategyConfigurationDb :: getStrategyConfigurations(const QString& strategyName) {
    QHash<QString, QString> strategyParams;
    if (!openDatabase()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return strategyParams;
    }

    QSqlQuery query = getBlankQuery();
    query.prepare("select StrategyName, ConfKey, ConfValue from StrategyConfiguration "
                  "where StrategyName = :StrategyName ");
    query.bindValue(":StrategyName", strategyName);

    bool result = query.exec();
    qDebug() << "Got " << query.size() << " rows" << endl;
    if (!result) {
        qDebug() << query.lastError().text() << endl;
        query.finish();
        db.close();
        return strategyParams;
    }

    while (query.next()) {
        strategyParams[query.value(ConfKey).toString()] = query.value(ConfValue).toString();
    }

    query.finish();
    db.close();

    return strategyParams;
}

/*uint StrategyConfigurationDb :: insertStrategyConfiguration(const StrategyConfigurationData& data) {
    return insertStrategyConfiguration(data.strategyId, data.confKey, data.confValue);
}

uint StrategyConfigurationDb :: insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

	//prepare statement
    QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyConfiguration(StrategyId, ConfKey, ConfValue) Values(:StrategyId, :ConfKey, :ConfValue )"
	);

	query.bindValue(":StrategyId", strategyId);
    query.bindValue(":ConfKey", confKey);
	query.bindValue(":ConfValue", confValue);
	//execute
	bool result = query.exec();
	if (!result) {
		qDebug() << "Couldn't insert row. " << query.lastError().text() << endl;
        return 0;
	}
	qDebug() << "Inserted a row" << endl;
	return 1;
}

uint StrategyConfigurationDb :: insertStrategyConfigurations(const QList<StrategyConfigurationData>& list) {
	//check database if available to work with
	if (!openDatabase()) {
		return 0; //to signify zero inserted rows
	}

	//prepare statement
	QSqlQuery query = getBlankQuery();
    query.prepare("insert into StrategyConfiguration(StrategyId, ConfKey, ConfValue) Values(:StrategyId, :ConfKey, :ConfValue )"
	);

    uint ctr=0;
    db.transaction(); //start a transaction

    foreach(StrategyConfigurationData item, list) {
        query.bindValue(":StrategyId", item.strategyId);
        query.bindValue(":ConfKey", item.confKey);
        query.bindValue(":ConfValue", item.confValue);

        ctr += (query.exec() ? 1 : 0);
    }
    db.commit();

    if (ctr < list.count()) {
        qDebug() << "Couldn't insert strategy configuration data rows. "
                 << "Error: " << query.lastError().text() << " " << endl;
        qDebug() << query.lastQuery() << endl;
        return ctr;
    }
    db.close();
    return ctr;
}

uint StrategyConfigurationDb :: updateStrategyConfiguration(const StrategyConfigurationData& data) {
    return updateStrategyConfiguration(data.strategyId, data.confKey, data.confValue);
}

uint StrategyConfigurationDb :: updateStrategyConfiguration(const uint &strategyId, const QString &key, const QString &value) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Update StrategyConfiguration Set ConfValue = :ConfValue "
                  "Where ConfKey = :ConfKey and StrategyID = :StrategyId ");

    query.bindValue(":StrategyId", strategyId);
    query.bindValue(":ConfKey", key);
    query.bindValue(":ConfValue", value);

	bool result = query.exec();
	if (!result)
        qDebug() << "Could not update table for strategyId " << strategyId << " and confKey " << key << endl;

	query.finish();
	db.close();
    return (result ? 1 : 0);
}

uint StrategyConfigurationDb :: deleteStrategyConfiguration(uint strategyId, QString confKey) {
    if (!openDatabase()) {
		qDebug() << "Unable to connect to database!!" << endl;
		qDebug() << db.lastError().driverText();
		return 0;
	}

    QSqlQuery query = getBlankQuery();
    query.prepare("Delete from StrategyConfiguration where ConfKey = :ConfKey and StrategyId = :StrategyId");
    query.bindValue(":ConfKey", confKey);
    query.bindValue(":StrategyId", strategyId);
	bool result = query.exec();
	if (!result)
        qDebug() << "Could not delete table for strategyId " << strategyId << " and confKey " << confKey << endl;
	query.finish();
	db.close();
    return (result ? 1 : 0);
}
*/

