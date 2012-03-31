#include "Strategy/TestStrategy.h"
#include "Platform/Utils/TimeZone.h"
#include "Platform/Utils/TradingSchedule.h"

int x = TestStrategy::addStrategy();

TestStrategy::TestStrategy():Strategy("TestStrategy")
{
    QString start = "09:30";
    QString end = "15:30";
   // _tradingSchedule = new TradingSchedule("09:30","15:30", EST );
    _tradingSchedule = new TradingSchedule(start, end , EST );
}

TestStrategy::~TestStrategy()
{}

void TestStrategy::startStrategy()
{
    Contract contract;
    //contract.conId = 36;
    contract.symbol = "MSFT";
    contract.secType = "STK";
    contract.expiry = "";
    contract.strike = 0.0;
    contract.right = "";
    contract.multiplier = "0";
    contract.exchange = "SMART";
    contract.primaryExchange = "ISLAND";
    contract.currency = "USD";

    requestMarketData(contract);

    int i=0;
    /*while(++i<100)
    {
        sleep(2);
        Order o;
        o.totalQuantity=5;
        o.orderType ="MKT";
        //placeOrder(contract,o);
    }*/
}
