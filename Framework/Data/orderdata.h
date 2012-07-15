#ifndef ORDERDATA_H
#define ORDERDATA_H
#include<QDateTime>

class OrderData // : public QObject
{
    //Q_OBJECT
public:
	//member variables
    uint orderId;
	float limitPrice;
    uint quantity;
    quint8 action;
    quint8 status;
	QDateTime placedDate;
	QDateTime updatedDate;
    quint8 orderType;
	float avgFillPrice;
    uint filledQuantity;
	float commission;
	float positionAmount;
    uint instrumentId;
	QDateTime goodTillDate;

	//ctor
	OrderData(void);
	OrderData(OrderData* copy);
	~OrderData(void);

	//methods
    void printDebug();
};

#endif //ORDERDATA_H
