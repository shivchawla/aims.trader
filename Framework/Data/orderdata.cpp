#include "orderdata.h"
#include <QDebug>

OrderData :: OrderData(void)
{
}

OrderData :: OrderData(const OrderData* copy)
{
	orderId = copy->orderId;
	limitPrice = copy->limitPrice;
	quantity = copy->quantity;
	action = copy->action;
	status = copy->status;
	placedDate = copy->placedDate;
	updatedDate = copy->updatedDate;
	orderType = copy->orderType;
	avgFillPrice = copy->avgFillPrice;
	filledQuantity = copy->filledQuantity;
	commission = copy->commission;
	positionAmount = copy->positionAmount;
	instrumentId = copy->instrumentId;
	goodTillDate = copy->goodTillDate;
    //originalOrderId = copy->originalOrderId;
    strategyId = copy->strategyId;
}
OrderData :: ~OrderData(void)
{
}

// prints all properties for easy console debugging
void OrderData :: printDebug() {
    qDebug() << orderId << " " << limitPrice << " " << action <<" "<< status << " "
             << placedDate << " " << orderType << " " << updatedDate << " " << avgFillPrice << " "
             << filledQuantity << " " << positionAmount << " " << commission << " "
             << instrumentId << " " << goodTillDate << " " << strategyId << endl;
}

