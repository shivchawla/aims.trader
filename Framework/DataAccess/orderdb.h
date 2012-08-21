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
    uint insertOrder(const OrderData* data);
    uint insertOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                        QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                        float avgFillPrice, uint filledQuantity, float commission,
                        float positionAmount, uint instrumentId, QDateTime goodTillDate, uint strategyId);

    //update methods - return number of rows updated
    uint updateOrder(const OrderData* data);

//    uint updateOrder(uint OrderId, float limitPrice, uint quantity, quint8 action, quint8 status,
//                        QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
//                        float avgFillPrice, uint filledQuantity, float commission,
//                        float positionAmount, uint instrumentId, QDateTime goodTillDate, uint strategyId);

    uint updateOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                                QDateTime placedDate, QDateTime updatedDate, quint8 orderType, float avgFillPrice,
                                uint filledQuantity, float commission, float positionAmount, uint instrumentId,
                                QDateTime goodTillDate, uint StrategyId);


    //delete methods - return number of rows deleted
    uint deleteOrder(const uint &orderId, const QDate &placedDate);

    uint insertRow(const OrderData* data);
    uint deleteRow(const OrderData* data);
    uint updateRow(const OrderData* data);

};


