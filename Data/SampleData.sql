-- Sample data script

-- Configuration table sample data
insert into GeneralConfiguration values('HistoryStartDate', '13-APR-2012', 'This date should be in dd-MON-YYYY format');
insert into GeneralConfiguration values('ScheduleRunTime', '07:00:00', 'Time should be in HH:MI:SS , 24 hour format');

-- Country table sample data
insert into Country values('USA', 'United States of America');

-- Sector table sample data
insert into Sector values('ITECH', 'Information technology');

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
Insert into StratTrader.Order(LimitPrice, Quantity, Action, Status, PlacedDate, UpdatedDate, OrderType, AvgFillPrice, FilledQuantity, Commission, PositionAmount, InstrumentId, GoodTillDate)
values(23.21, 10, 0,4, sysdate(), now(),  0, 0, 0, 0, 0,
    (select InstrumentId from Instrument where Symbol='IBM' and Type=0),sysdate());

-- Strategy Linked Position table
insert into StrategyLinkedPosition(NumberBought, NumberSold, AvgAmountBought, AvgAmountSold, TotalAmountBought, TotalAmountSold, TotalAmountCommission, RealizedPnl, UpdatedDate, StrategyId, InstrumentId)
values(0, 0, 0, 0, 0, 0, 0, 0, now(),
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
