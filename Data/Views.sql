-- ----- Order View
Create or Replace View `StratTrader`.`vw_Orders` as 
select o.OrderId, o.PlacedDate, o.LimitPrice, o.Quantity, 
Case o.Action when '0' then 'Buy'
            when '1' then 'Sell'
            When '2' then 'Short Sell'
else 'Unknown' 
End as Action,
Case o.Status when '0' then 'None'
            when '1' then 'PendingSubmit'
            When '2' then 'PendingCancel'
            when '3' then 'PreSubmitted'
            when '4' then 'Submitted'
            when '5' then 'Cancelled'
            when '6' then 'FullyFilled'
            when '7' then 'Inactive'
            when '8' then 'PartiallyFilled'
            when '9' then 'ApiPending'
            when '10' then 'ApiCancelled'
else 'Unknown'
End as Status,
o.UpdatedDate,
Case o.OrderType when '0' then 'MKT'
            when '1' then 'LMT'
else 'Unknown'
End as OrderType,
o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, i.Symbol,
Case i.Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As InstrumentType, 
i.ExchangeCode, 
s.Name as StrategyName, 
o.GoodTillDate
from Orders o
inner join Instrument i on o.InstrumentId = i.InstrumentId
inner join Strategy s on o.StrategyId = s.StrategyId;

-- ---- Instrument View
-- Instrument View not needed anymore

-- ----- Strategy Linked Position View
Create or Replace  View `StratTrader`.`vw_StrategyLinkedPosition` as
Select s.Name as Strategy, i.Symbol, p.NumberBought, p.NumberSold, p.AvgAmountBought, p.AvgAmountSold,
p.TotalAmountCommission, p.UpdatedDate, 
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode
from StrategyLinkedPosition p
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- Strategy Linked Position Detail View
Create or Replace  View `StratTrader`.`vw_StrategyLinkedPositionDetail` as
Select s.Name as Strategy, i.Symbol, d.SharesBought, d.SharesSold, d.AvgBought, d.AvgSold, d.Commission, d.CreatedDateTime,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode
from StrategyLinkedPositionDetail d
inner join StrategyLinkedPosition p on d.StrategyLinkedPositionId = p.StrategyLinkedPositionId
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- StrategyBuy List View
Create or Replace view `StratTrader`.`vw_StrategyBuyList` as 
Select s.Name as Strategy, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode
from StrategyBuyList l
inner join Strategy s on l.StrategyId = s.StrategyId
inner join Instrument i on l.InstrumentId = i.InstrumentId;

-- --------- DailyHistoryBar View
Create or replace view `StratTrader`.`vw_DailyHistoryBar` as
select i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.HistoryDate, h.Open, h.Close, h.High, h.Low, h.Volume,
i.ExchangeCode, i.CountryCode
from DailyHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId;

-- --------------Strategy View
Create or replace view `StratTrader`.`vw_Strategy` as
select s.Name as Strategy, s.Since, s.UsedInTrading, p.Name as ParentStrategy
from Strategy s
left join Strategy p on s.ParentStrategyId = p.StrategyId
where s.ParentStrategyId is NOT NULL;

-- --------- IntradayHistoryBar View
Create or replace view `StratTrader`.`vw_IntradayHistoryBar` as
select i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.HistoryDateTime, h.Open, h.Close, h.High, h.Low, h.Volume,
i.ExchangeCode, i.CountryCode
from IntradayHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId;

-- -------------Instrument Configuration View
Create or replace view `StratTrader`.`vw_InstrumentConfiguration` as
select i.Symbol, 
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode, c.ConfKey, c.ConfValue
from Instrument i
left join InstrumentConfiguration c on i.InstrumentId = c.InstrumentId;

-- ------------- Strategy Configuration view
create or replace view `StratTrader`.`vw_StrategyConfiguration` as
select s.Name, c.ConfKey, c.ConfValue
from StrategyConfiguration c
inner join Strategy s on c.StrategyId = s.StrategyId;

-- ---------------------------------------------------
-- ---------------------------------------------------
-- ---------------------------------------------------
-- ---------------------------------------------------
-- ---------------------------------------------------

-- Dev Views
-- ----- Order View
Create or replace View `StratTrader`.`dvw_Orders` as 
select o.OrderId, o.PlacedDate, o.LimitPrice, o.Quantity, 
Case o.Action when '0' then 'Buy'
            when '1' then 'Sell'
            When '2' then 'Short Sell'
else 'Unknown' 
End as Action,
Case o.Status when '0' then 'None'
            when '1' then 'PendingSubmit'
            When '2' then 'PendingCancel'
            when '3' then 'PreSubmitted'
            when '4' then 'Submitted'
            when '5' then 'Cancelled'
            when '6' then 'FullyFilled'
            when '7' then 'Inactive'
            when '8' then 'PartiallyFilled'
            when '9' then 'ApiPending'
            when '10' then 'ApiCancelled'
else 'Unknown'
End as Status,
o.UpdatedDate,
Case o.OrderType when '0' then 'MKT'
            when '1' then 'LMT'
else 'Unknown'
End as OrderType,
o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, i.Symbol,
Case i.Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As InstrumentType, 
i.ExchangeCode,
s.Name as StrategyName,
o.GoodTillDate,
s.StrategyId
from Orders o
inner join Instrument i on o.InstrumentId = i.InstrumentId
inner join Strategy s on o.StrategyId = s.StrategyId;

-- ---- Instrument View
-- Instrument View not needed anymore

-- ----- Strategy Linked Position View
Create or replace View `StratTrader`.`dvw_StrategyLinkedPosition` as
Select p.StrategyLinkedPositionId, p.NumberBought, p.NumberSold, p.AvgAmountBought, p.AvgAmountSold,
p.TotalAmountCommission, p.UpdatedDate, s.Name StrategyName, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode
from StrategyLinkedPosition p
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- Strategy Linked Position Detail Dev View
Create or Replace  View `StratTrader`.`dvw_StrategyLinkedPositionDetail` as
Select s.StrategyId, s.Name as Strategy, i.InstrumentId, i.Symbol, d.SharesBought, d.SharesSold, d.AvgBought, d.AvgSold, d.Commission, d.CreatedDateTime,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode,
p.StrategyLinkedPositionId,
d.StrategyLinkedPositionDetailId
from StrategyLinkedPositionDetail d
inner join StrategyLinkedPosition p on d.StrategyLinkedPositionId = p.StrategyLinkedPositionId
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- StrategyBuy List View
Create or Replace view `StratTrader`.`dvw_StrategyBuyList` as 
Select l.StrategyBuyListId, l.StrategyId, s.Name StrategyName, l.InstrumentId, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode
from StrategyBuyList l
inner join Strategy s on l.StrategyId = s.StrategyId
inner join Instrument i on l.InstrumentId = i.InstrumentId;

-- --------- DailyHistoryBar View
Create or replace view `StratTrader`.`dvw_DailyHistoryBar` as
select i.InstrumentId, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.HistoryDate, h.Open, h.Close, h.High, h.Low, h.Volume,
i.ExchangeCode, i.CountryCode
from DailyHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId;

-- --------------Strategy View
Create or replace view `StratTrader`.`dvw_Strategy` as
select s.StrategyId, s.Name, s.Since, s.UsedInTrading, s.ParentStrategyId, p.Name as ParentName
from Strategy s
left join Strategy p on s.ParentStrategyId = p.StrategyId;

-- --------- IntradayHistoryBar View
Create or replace view `StratTrader`.`dvw_IntradayHistoryBar` as
select i.InstrumentId, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.HistoryDateTime, h.Open, h.Close, h.High, h.Low, h.Volume,
i.ExchangeCode, i.CountryCode
from IntradayHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId;

-- ------------- Dev Instrument Configuration View
Create or replace view `StratTrader`.`dvw_InstrumentConfiguration` as
select i.InstrumentId, i.Symbol, 
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
i.ExchangeCode, c.ConfKey, c.ConfValue
from Instrument i
left join InstrumentConfiguration c on i.InstrumentId = c.InstrumentId;

-- ------------- Dev Strategy Configuration view
create or replace view `StratTrader`.`dvw_StrategyConfiguration` as
select s.StrategyId, s.Name, c.ConfKey, c.ConfValue
from StrategyConfiguration c
inner join Strategy s on c.StrategyId = s.StrategyId;