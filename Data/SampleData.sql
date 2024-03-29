-- Sample data script

-- Configuration table sample data
insert into GeneralConfiguration values('DailyHistoryStartDate', '2006-01-01', 'This date should be in ISO format');
insert into GeneralConfiguration values('IntradayHistoryStartDate', '2012-06-15', 'This date should be in ISO format');
insert into GeneralConfiguration values('ScheduleRunTime', '07:00:00', 'Time should be in HH:MI:SS , 24 hour format');

-- Country table sample data
insert into Country values('USA', 'United States of America');

-- Sector table sample data
insert into Sector values('CDISC','Consumer Discretionary');
insert into Sector values('CSTAP','Consumer Staples');
insert into Sector values('ENERGY','Energy');
insert into Sector values('FINCE','Financial');
insert into Sector values('HLTHC','Healthcare');
insert into Sector values('INDST','Industrial');
insert into Sector values('ITECH','Information Technology');
insert into Sector values('MATERL','Materials');
insert into Sector values('UTILS','Utilities');
insert into Sector values('MISC','Miscellaneous');


-- Exchange table sample data

insert into Exchange values('NASDAQ', 'NASDAQ');
insert into Exchange values('NYSE', 'New York Stock Exchange');

-- Instrument table sample script

Insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, CountryCode, ExchangeCode)
values('KFRC', 'KForce', 'KForce Inc', 0, 'ITECH', 'USA', 'NASDAQ');
Insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, CountryCode, ExchangeCode)
values('IBM', 'International Business Machine', 'International Business Machine Corp', 0, 'ITECH', 'USA', 'NASDAQ');
Insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, CountryCode, ExchangeCode)
values('MSFT', 'Microsoft', 'Microosft Corp', 0, 'ITECH', 'USA', 'NASDAQ');
Insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, CountryCode, ExchangeCode)
values('GOOG', 'Google', 'Google Inc', 0, 'ITECH', 'USA', 'NASDAQ');
Insert into Instrument(Symbol, ShortName, FullName, Type, SectorCode, CountryCode, ExchangeCode)
values('AAPL', 'Apple', 'Apple Inc', 0, 'ITECH', 'USA', 'NASDAQ');

-- Strategy table

insert into Strategy(Name, Since, UsedInTrading, ParentStrategyId) values('Momentum', sysdate(), false, null);
Select StrategyId into @tempId from Strategy where Name='Momentum';

insert into Strategy(Name, Since, UsedInTrading, ParentStrategyId) values('MomentumABC', sysdate(), false, @tempId);
insert into Strategy(Name, Since, UsedInTrading, ParentStrategyId) values('MomentumXYZ', sysdate(), false, @tempId);

-- Orders table
-- Order Type 0 - MKT, 1 - LKT
Insert into Orders(OrderId, LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate, StrategyId)
values(0, 23.21, 10, 0,4, sysdate(), now(),  0, 0, 0, 0, 0,
    (select InstrumentId from Instrument where Symbol='IBM' and Type=0),sysdate(),
	(select StrategyId from Strategy where Name='MomentumABC'));

-- Strategy Linked Position table
insert into StrategyLinkedPosition(NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, TotalAmountCommission, CreatedDate, UpdatedDate, StrategyId, InstrumentId)
values(0, 0, 0, 0, 0, now(), now(),
    (select StrategyId from Strategy where Name='MomentumABC'), 
    (select InstrumentId from Instrument where Symbol='IBM' and Type=0));

-- Strategy Buy List table
insert into StrategyBuyList(StrategyId, InstrumentId)
values(
    (select StrategyId from Strategy where Name='MomentumABC'),
    (select InstrumentId from Instrument where Symbol='IBM' and Type=0));

-- Daily History bar test data to test insert and truncate statements
insert into DailyHistoryBar values('1978-10-21 18:30:00', 10,10,10,10,1000, 
(select InstrumentId from Instrument where Symbol='IBM' and Type=0));

-- StrategyLinkedPositionDetail table
insert into StrategyLinkedPositionDetail(SharesBought, SharesSold, AvgBought, AvgSold, Commission, CreatedDateTime, StrategyLinkedPositionId)
values(100, 50, 34.5, 100.78, 90, now(), (select StrategyLinkedPositionId from StrategyLinkedPosition LIMIT 1));

-- StrategyConfiguration table
insert into StrategyConfiguration(StrategyId, ConfKey, ConfValue)
values(	(select StrategyId from Strategy where Name='MomentumABC'), 'ShouldSaveToDbWhenIdle', 'True');

