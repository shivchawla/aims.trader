#include <QString>

class GeneralConfigurationData
{
    public:
        //member variables
        QString key;
        QString value;
        QString comments;

        GeneralConfigurationData(void);
        ~GeneralConfigurationData(void);

        void printDebug();
};

