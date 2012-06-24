#include "orderdata.h"
#include <QDebug>

OrderData :: OrderData(void)
{
}

OrderData :: OrderData(OrderData* copy)
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
	exchangeId = copy->exchangeId;
	instrumentId = copy->instrumentId;
	goodTillDate = copy->goodTillDate;
}
OrderData :: ~OrderData(void)
{
}

// prints all properties for easy console debugging
void OrderData :: printDebug() {
    qDebug() << orderId << " " << limitPrice << " " << action <<" "<< status << " "
             << placedDate << " " << orderType << " " << updatedDate << " " << avgFillPrice << " "
             << filledQuantity << " " << positionAmount << " " << commission << " "
             << exchangeId << " " << instrumentId << " " << goodTillDate << endl;
}

