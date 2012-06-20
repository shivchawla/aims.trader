-- Sample data script

-- Exchange table sample data

insert into Exchange values(MYUUID(), 'NASDAQ');
insert into Exchange values(MYUUID(), 'NYSE');

-- Instrument table sample script

Insert into Instrument values(MyUUID(), 'KFRC', 'KForce', 'KForce Inc','0', 'SGROVER', NOW());
Insert into Instrument values(MyUUID(), 'IBM', 'International Business Machine', 'International Business Machine Corp','0', 'SGROVER', NOW());
Insert into Instrument values(MyUUID(), 'MSFT', 'Microsoft', 'Microsoft Corp','0', 'SGROVER', NOW());
Insert into Instrument values(MyUUID(), 'AAPL', 'Apple', 'Apple Inc','0', 'SGROVER', NOW());
Insert into Instrument values(MyUUID(), 'GOOG', 'Google', 'Google Inc','0', 'SGROVER', NOW());

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
    (select bintouuid(StrategyId) from Strategy where Name='MomentumABC'),
    (select bintouuid(InstrumentId) from Instrument where Symbol='IBM' and Type='0'));