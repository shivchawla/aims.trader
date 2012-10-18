#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/orderdata.h"
#include "DataAccess/DbTask.h"

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
		InstrumentId, 
        GoodTillDate,
        //OriginalOrderId
        StrategyId
	};

public:
	OrderDb(void);
	~OrderDb(void);

	//Methods

    //Select methods
    OrderData* getOrderById(const uint &id, const QDateTime &placedDate);
    QList<OrderData*> getOrders();
    QList<OrderData*> getOrders(uint strategyId, uint instrumentId, QDate placedDate);
    QList<OrderData*> getOrdersByStrategyId(const uint &strategyId);
    QList<OrderData*> getOrdersByStrategyName(const QString &strategyName);

    //insert methods return true or false based on result of insert
    //uint insertOrder(const OrderData* data);
    uint insertOrder(const uint runId, const uint orderId, const uint strategyId, const uint instrumentId, const QString& orderType, const double limitPrice, const uint quantity,
                     const QString& action, const QString& status, const QDateTime& placedDate, const QDateTime& goodTillDate);

    //update methods - return number of rows updated
    //uint updateOrder(const OrderData* data);
    uint updateOrder(const uint runId, const uint orderId, const uint strategyId, const uint instrumentId,
                                const QString& status, const double avgFillPrice,
                                const uint filledQuantity, const double commission, const QDateTime& updatedDate);


    //delete methods - return number of rows deleted
    uint deleteOrder(const uint runId, const uint orderId, const QDateTime& placedDate);

    //uint insertRow(const OrderData* data);
    //uint deleteRow(const OrderData* data);
    //uint updateRow(const OrderData* data);

};


