#ifndef ORDERDATA_H
#define ORDERDATA_H
#include<QUuid>
#include<QChar>
#include<QDateTime>

class OrderData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
	QUuid orderId;
	float limitPrice;
	unsigned int quantity;
	QChar action;
	QChar status;
	QDateTime placedDate;
	QDateTime updatedDate;
	QChar orderType;
	float avgFillPrice;
	unsigned int filledQuantity;
	float commission;
	float positionAmount;
	QUuid exchangeId;
	QUuid instrumentId;
	QDateTime goodTillDate;

	//ctor
	OrderData(void);
	OrderData(OrderData* copy);
	~OrderData(void);

	//methods
    void printDebug();
};

#endif //ORDERDATA_H
