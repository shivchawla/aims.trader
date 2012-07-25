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
        OriginalOrderId
	};

public:
	OrderDb(void);
	~OrderDb(void);

	//Methods

    //Select methods
    OrderData* getOrderById(const uint &id);
    QList<OrderData*> getOrders();
    QList<OrderData*> getOrdersByStrategyId(const uint &strategyId);
    QList<OrderData*> getOrdersByStrategyName(const QString &strategyName);

    //insert methods - return newly inserted primary key and not count of rows inserted
    uint insertOrder(const OrderData* data);
    uint insertOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                        QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                        float avgFillPrice, uint filledQuantity, float commission,
                        float positionAmount, uint instrumentId, QDateTime goodTillDate, uint originalOrderId);

    //update methods - return number of rows updated
    uint updateOrder(const OrderData* data);
    uint updateOrder(uint id, float limitPrice, uint quantity, quint8 action, quint8 status,
                        QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                        float avgFillPrice, uint filledQuantity, float commission,
                        float positionAmount, uint instrumentId, QDateTime goodTillDate, uint originalOrderId);
    uint updateOrderBasedOnOriginalId(float limitPrice, uint quantity, quint8 action, quint8 status,
                                QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                                uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                                QDateTime goodTillDate, uint originalOrderId);
    QList<OrderData*> getOrders(uint originalOrderId, uint instrumentId);

    //delete methods - return number of rows deleted
    uint deleteOrder(const uint &id);

    uint insertRow(const OrderData* data);
    uint deleteRow(const OrderData* data);
    uint updateRow(const OrderData* data);

};


