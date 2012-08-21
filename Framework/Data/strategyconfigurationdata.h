#ifndef STRATEGYCONFIGURATIONDATA_H
#define STRATEGYCONFIGURATIONDATA_H
#include<QString>

class StrategyConfigurationData //: public QObject
{
    //Q_OBJECT
public:
	//member variables
    uint strategyId;
	QString confKey;
	QString confValue;

	//ctor
	StrategyConfigurationData(void);
	StrategyConfigurationData(StrategyConfigurationData* copy);
	~StrategyConfigurationData(void);

	//methods
	void printDebug();

};

#endif //STRATEGYCONFIGURATIONDATA_H
