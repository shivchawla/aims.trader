#ifndef STRATEGYBUYLISTDATA_H
#define STRATEGYBUYLISTDATA_H
#include <QDateTime>

class StrategyBuyListData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
    uint strategyBuyListId;
    uint strategyId;
    uint instrumentId;
    QDateTime deactivatedDate;

    //supporting member variables
    QString strategyName;
    QString symbol;
    quint8 instrumentType;

	//ctor
	StrategyBuyListData(void);
    StrategyBuyListData(const StrategyBuyListData* copy);
	~StrategyBuyListData(void);

	//methods
    void printDebug();

};

#endif //STRATEGYBUYLISTDATA_H
