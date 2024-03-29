-- This script cleans out the entire database data except for the Configuration table
SET FOREIGN_KEY_CHECKS=0;

Truncate table StrategyConfiguration;
Truncate table StrategyBuyList;
Truncate table StrategyLinkedPositionDetail;
Truncate table StrategyLinkedPosition;
Truncate table StratTrader.Orders;

Delete from Strategy where ParentStrategyId is not null; -- first delete children
Delete from Strategy; -- then delete all remaining parent data

Truncate table Strategy; -- truncate so seeds can be reset
Truncate table DailyHistoryBar;
Truncate table IntradayHistoryBar;
Truncate table InstrumentConfiguration;
Truncate table Instrument;
Truncate table Exchange;
Truncate table Country;
Truncate table Sector;
Truncate table GeneralConfiguration;

SET FOREIGN_KEY_CHECKS=1;