#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

class ConfigurationData
{
    public:
        //member variables
        QUuid configurationId;
        QString key;
        QString value;

        ConfigurationData(void);
        ~ConfigurationData(void);
};

