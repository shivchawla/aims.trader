#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H
#include <string>
#include <map>
typedef std::string String;

class ConfigurationManager
{
    private:
        static ConfigurationManager* _cManager;
        static std::map<String, String > _appSettings;
        static std::map<String, String> _connectionSettings;

    private:
        ConfigurationManager();

    public:
        ~ConfigurationManager();

    public:
        void configure();

    private:
        void readConfigFiles();

};

#endif // CONFIGURATIONMANAGER_H
