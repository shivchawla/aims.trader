#include <QString>

class GeneralConfigurationData
{
    public:
        //member variables
        QString key;
        QString value;
        QString comments;

        GeneralConfigurationData();
        ~GeneralConfigurationData();

        void printDebug();
};

