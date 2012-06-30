-- Sample data script

-- Configuration table sample data
insert into Configuration values(MYUUID(), 'HistoryStartDate', '13-APR-2012', 'This date should be in dd-MON-YYYY format');
insert into Configuration values(MYUUID(), 'ScheduleRunTime', '07:00:00', 'Time should be in HH:MI:SS , 24 hour format');

-- Country table sample data
insert into Country values(MYUUID(), 'USA', 'United States of America');

-- Exchange table sample data

insert into Exchange values(MYUUID(), 'NASDAQ');
insert into Exchange values(MYUUID(), 'NYSE');

-- Instrument table sample script

Insert into Instrument values(MyUUID(), 'KFRC', 'KForce', 'KForce Inc','0', 'SGROVER', NOW(),
 (select ExchangeId from Exchange where Name='NASDAQ'), (select COuntryId from Country where Code='USA'));
Insert into Instrument values(MyUUID(), 'IBM', 'International Business Machine', 'International Business Machine Corp','0', 'SGROVER', NOW(),
 (select ExchangeId from Exchange where Name='NASDAQ'), (select COuntryId from Country where Code='USA'));
Insert into Instrument values(MyUUID(), 'MSFT', 'Microsoft', 'Microsoft Corp','0', 'SGROVER', NOW(),
 (select ExchangeId from Exchange where Name='NASDAQ'), (select COuntryId from Country where Code='USA'));
Insert into Instrument values(MyUUID(), 'AAPL', 'Apple', 'Apple Inc','0', 'SGROVER', NOW(),
 (select ExchangeId from Exchange where Name='NASDAQ'), (select COuntryId from Country where Code='USA'));
Insert into Instrument values(MyUUID(), 'GOOG', 'Google', 'Google Inc','0', 'SGROVER', NOW(),
 (select ExchangeId from Exchange where Name='NASDAQ'), (select COuntryId from Country where Code='USA'));

-- Strategy table

insert into Strategy values(UnHex('C078B8A8B1D511E1A490080027BBBE77'), 'Momentum', sysdate(), false, null);
insert into Strategy values(MYUUID(), 'MomentumABC', sysdate(), true, UnHex('C078B8A8B1D511E1A490080027BBBE77'));
insert into Strategy values(MYUUID(), 'MomentumXYZ', sysdate(), true, UnHex('C078B8A8B1D511E1A490080027BBBE77'));

-- Orders table
-- Order Type 0 - MKT, 1 - LKT
Insert into StratTrader.Order values(MYUUID(), 23.21, 10, '0','4', sysdate(), now(),  '0', 0, 0, 0, 0,
 (select ExchangeId from Exchange where Name='NASDAQ'),
 (select InstrumentId from Instrument where Symbol='IBM' and Type='0'),sysdate());

-- Strategy Linked Position table
insert into StrategyLinkedPosition values(MYUUID(), 0, 0, 0, 0, 0, 0, 0, 0, now(),
    (select StrategyId from Strategy where Name='MomentumABC'), 
    (select InstrumentId from Instrument where Symbol='IBM' and Type='0'));

-- Strategy Buy List table
insert into StrategyBuyList values(MYUUID(),
    (select StrategyId from Strategy where Name='MomentumABC'),
    (select InstrumentId from Instrument where Symbol='IBM' and Type='0'));

