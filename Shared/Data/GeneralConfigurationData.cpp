#include "Data/GeneralConfigurationData.h"
#include <QDebug>


GeneralConfigurationData::GeneralConfigurationData()
{
}


GeneralConfigurationData::~GeneralConfigurationData()
{
}

void GeneralConfigurationData::printDebug() {
    qDebug() << key << " " << value << " " << comments << endl;
}
