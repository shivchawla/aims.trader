#include "Data/GeneralConfigurationData.h"
#include <QDebug>


GeneralConfigurationData::GeneralConfigurationData(void)
{
}


GeneralConfigurationData::~GeneralConfigurationData(void)
{
}

void GeneralConfigurationData::printDebug() {
    qDebug() << key << " " << value << " " << comments << endl;
}
