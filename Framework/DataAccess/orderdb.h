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
		InstrumentId, 
        GoodTillDate
	};

public:
	OrderDb(void);
	~OrderDb(void);

	//Methods

    OrderData* getOrderById(const uint &id);
    QList<OrderData*> getOrders();
    QList<OrderData*> getOrdersForStrategy(const uint &strategyId);
    uint insertOrder(const OrderData* &data);
    uint insertOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                        QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                        float avgFillPrice, uint filledQuantity, float commission,
                        float positionAmount, uint instrumentId, QDateTime goodTillDate);

    uint updateOrder(const OrderData* &data, const uint &id);
    uint deleteOrder(const uint &id);

};
