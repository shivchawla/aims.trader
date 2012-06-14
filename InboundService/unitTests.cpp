#include "orderdb.h"
#include "strategydb.h"
#include "strategyviewdb.h"
#include "strategybuylistdb.h"
#include "strategylinkedpositiondb.h"
#include "exchangedb.h"

void testOrder() {


    //Load Orders
    OrderDb oDb;
    QList<OrderData*> orders = oDb.getOrders();

    foreach(OrderData* od, orders) {
        od->printDebug();
        delete od;
    }
    OrderData* anOrder = oDb.getOrderById(QUuid("E5DC2BE0-B28C-11E1-AC1C-080027BBBE77"));
    anOrder->printDebug();
    OrderData* orderCopy = new OrderData(anOrder);
    orderCopy->orderId = QUuid::createUuid();
    orderCopy->quantity = 21;
    oDb.insertOrder(orderCopy);
    orderCopy->filledQuantity = 21;
    oDb.updateOrder(orderCopy, orderCopy->orderId);

    oDb.deleteOrder(orderCopy->orderId);

}

void testStrategy() {
    StrategyDb sdb;
    QList<StrategyData*> list = sdb.getStrategies();
    qDebug() << "********** Printing list" << endl;
    foreach(StrategyData* sd, list) {
        sd->printDebug();
        delete sd;
    }
    StrategyData *aStrategy = sdb.getStrategyById(QUuid("C078B8A8-B1D5-11E1-A490-080027BBBE77"));
    qDebug() << "********** Printing single object" << endl;
    aStrategy->printDebug();
    StrategyData *sCopy = new StrategyData(aStrategy);
    sCopy->strategyId = QUuid::createUuid();
    sCopy->name = "Some Strategy";
    qDebug() << "********** Inserting this object" << endl;
    sCopy->printDebug();
    sdb.insertStrategy(sCopy);
    sCopy->since = QDateTime::currentDateTime();
    qDebug() << "********** Updating this object" << endl;
    sdb.updateStrategy(sCopy, sCopy->strategyId);
    qDebug() << "********** Deleting this object" << endl;
    sdb.deleteStrategy(sCopy->strategyId);

    delete aStrategy;
    delete sCopy;
}

void testStrategyView() {
    StrategyViewDb svdb;
    QList<StrategyViewData*> list = svdb.getStrategyViews();
    foreach(StrategyViewData* v, list) {
        v->printDebug();
        delete v;
    }
    StrategyViewData* aView = svdb.getStrategyViewById(QUuid("EB1F8B72-B1D5-11E1-A490-080027BBBE77"));
    qDebug() << "******************* Printing single strategy view object" << endl;
    aView->printDebug();
    delete aView;
}

void testStrategyBuyList() {
    StrategyBuyListDb db;
    StrategyBuyListData* aCopy = new StrategyBuyListData();
    aCopy->strategyBuyListId = QUuid::createUuid();
    aCopy->strategyId = QUuid("EB1F8B72-B1D5-11E1-A490-080027BBBE77");
    aCopy->instrumentId = QUuid("2F3F76FA-B1DB-11E1-A490-080027BBBE77");
    qDebug() << "****************** Inserting a single strategy buy list object" << endl;
    int rows = db.insertStrategyBuyList(aCopy);
    if (rows <= 0) {
        qDebug() << "insert failed" << endl;
        return;
    }

    QList<StrategyBuyListData*> list = db.getStrategyBuyLists();
    foreach(StrategyBuyListData* o, list) {
        o->printDebug();
        delete o;
    }
    qDebug() << "****************** Printing a single strategy buy list object" << endl;
    StrategyBuyListData* aListObj = db.getStrategyBuyListById(aCopy->strategyBuyListId);
    aListObj->printDebug();
    qDebug() << "****************** updating a single strategy buy list object" << endl;
    aListObj->instrumentId = QUuid("8B22230A-B1DB-11E1-A490-080027BBBE77"); //AAPL
    db.updateStrategyBuyList(aListObj, aListObj->strategyBuyListId);
    qDebug() << "****************** deleting a single strategy buy list object" << endl;
    db.deleteStrategyBuyList(aListObj->strategyBuyListId);
    delete aListObj;
    delete aCopy;
}

void testStrategylinkedPosition() {
    StrategyLinkedPositionDb db;
    QList<StrategyLinkedPositionData*> list=db.getStrategyLinkedPositions();
    qDebug() << "********************** Printing all objects" << endl;
    foreach(StrategyLinkedPositionData* o, list) {
        o->printDebug();
        delete o;
    }
    qDebug() << "********************** Printing single object" << endl;
    StrategyLinkedPositionData* aObj = db.getStrategyLinkedPositionById(QUuid("AE4957F2-B28C-11E1-AC1C-080027BBBE77"));
    aObj->printDebug();

    qDebug() << "********************** inserting single object" << endl;
    StrategyLinkedPositionData* aCopy = new StrategyLinkedPositionData(aObj);
    aCopy->numberBought=10;

    aCopy->strategyLinkedPositionId = QUuid::createUuid();

    db.insertStrategyLinkedPosition(aCopy);
    qDebug() << "********************** updating single object" << endl;
    aCopy->numberSold=10;
    db.updateStrategyLinkedPosition(aCopy, aCopy->strategyLinkedPositionId);
    qDebug() << "********************** deleting single object" << endl;
    db.deleteStrategyLinkedPosition(aCopy->strategyLinkedPositionId);
    delete aObj, aCopy;
}

void testExchange() {
    ExchangeDb db;
    QList<ExchangeData*> list = db.getExchanges();
    foreach(ExchangeData* o, list) {
        o->printDebug();
        delete o;
    }
    ExchangeData* aObj = db.getExchangeById(QUuid("4403AB26-B283-11E1-AC1C-080027BBBE77"));
    aObj->printDebug();
    delete aObj;
}

void unitTests() {
    //This program tests the various database classes to be used for the StratTrader database
    //Load Strategy, Open Positions and Open Orders

    //so business class should encapsulate all these

    //testOrder();
    //testStrategy();
    //testStrategyView();
    //testStrategyBuyList();
    //testStrategylinkedPosition();
    testExchange();
}
