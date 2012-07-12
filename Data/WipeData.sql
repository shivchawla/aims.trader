-- This script cleans out the entire database data except for the Configuration table

Delete from StrategyBuyList;
Delete from StrategyLinkedPosition;
Delete from StratTrader.Order;
Delete from Strategy where ParentStrategyId is not null; -- first delete children
Delete from Strategy; -- then delete all remaining parent data
Delete from DailyHistoryBar;
Delete from Instrument;
Delete from Exchange;
Delete from Country;
Delete from Sector;