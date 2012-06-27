#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/orderdata.h"

class OrderDb : public DbBase
{
private:
	enum Order {
		OrderId, 
		LimitPrice, 
		Quantity, 
		Action, 
		Status, 
		PlacedDate, 
		UpdatedDate, 
		OrderType, 
		AvgFillPrice, 
		FilledQuantity, 
		Commission, 
		PositionAmount, 
		ExchangeId, 
		InstrumentId, 
        GoodTillDate
	};

public:
	OrderDb(void);
	~OrderDb(void);

	//Methods

	OrderData* getOrderById(QUuid id);
    QList<OrderData*> getOrders();
    QList<OrderData*> getOrdersForStrategy(QUuid strategyId);
    unsigned int insertOrder(const OrderData* data);
    unsigned int insertOrder(QUuid orderId, float limitPrice, unsigned int quantity, QChar action, QChar status,
                        QDateTime placedDate, QDateTime updatedDate, QChar orderType,
                        float avgFillPrice, unsigned int filledQuantity, float commission,
                        float positionAmount, QUuid exchangeId, QUuid instrumentId,
                        QDateTime goodTillDate);
    unsigned int updateOrder(const OrderData* data, QUuid id);
	unsigned int deleteOrder(QUuid id);

};
