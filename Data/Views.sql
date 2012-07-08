-- ----- Order View
Create or replace View devOrderView as 
select BINTOUUID(o.OrderId) as OrderId, o.LimitPrice, o.Quantity, 
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
o.PlacedDate, o.UpdatedDate,
Case o.OrderType when '0' then 'MKT'
            when '1' then 'LMT'
else 'Unknown'
End as OrderType,
o.AvgFillPrice, o.FilledQuantity, o.Commission, o.PositionAmount, e.Name, i.Symbol,
Case i.Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As InstrumentType, 
o.GoodTillDate
from `Order` o
inner join Instrument i on o.InstrumentId = i.InstrumentId
inner join Exchange e on o.ExchangeId = e.ExchangeId;

-- ---- Instrument View
Create or Replace View devInstrumentView as 
Select BINTOUUID(i.InstrumentId) as InstrumentId , i.Symbol, i.ShortName, i.Fullname,
Case i.Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As Type,
i.UpdatedBy, i.UpdatedDate, BINTOUUID(e.ExchangeId) as ExchangeId, e.Name as ExchangeName, BINTOUUID(c.CountryId) as CountryId, c.Code as CountryCode, BINTOUUID(s.SectorId) as SectorId, s.Code as SectorCode
from Instrument i
inner join Exchange e on i.ExchangeId = e.ExchangeId
left join Country c on i.CountryId = c.CountryId
left join Sector s on i.SectorId = s.SectorId;

-- ----- Strategy Linked Position View
Create or replace View devStrategyLinkedPositionView as
Select BINTOUUID(p.StrategyLinkedPositionId) as StrategyLinkedPositionId, p.NumberBought, p.NumberSold, p.AvgAmountBought, p.AvgAmountSold,
p.TotalAmountBought, p.TotalAmountSold, p.TotalAmountCommission, p.RealizedPnl, p.UpdatedDate, s.Name StrategyName, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType
from StrategyLinkedPosition p
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- StrategyBuy List View
Create or Replace view devStrategyBuyListView as 
Select l.StrategyBuyListId, l.StrategyId, s.Name StrategyName, l.InstrumentId, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType
from StrategyBuyList l
inner join Strategy s on l.StrategyId = s.StrategyId
inner join Instrument i on l.InstrumentId = i.InstrumentId;

-- --------- DailyHistoryBar View
Create or replace view devDailyHistoryBarView as
select BINTOUUID(h.DailyHistoryBarId) as DailHistoryBarId, BINTOUUID(i.InstrumentId) as InstrumentId, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.HistoryDate, h.Open, h.Close, h.High, h.Low, h.Volume,
e.Name ExchangeName, c.Code CountryCode
from DailyHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId
inner join Exchange e on i.ExchangeId = e.ExchangeId
left join Country c on i.CountryId = c.CountryId;

-- ----------------------------------------------- production Views

-- ---- Instrument View
Create or Replace View InstrumentView as 
Select i.Symbol, i.ShortName, i.Fullname,
Case i.Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As Type,
e.Name as ExchangeName, c.Code as CountryCode, s.Code as SectorCode, i.UpdatedBy, i.UpdatedDate
from Instrument i
inner join Exchange e on i.ExchangeId = e.ExchangeId
left join Country c on i.CountryId = c.CountryId
left join Sector s on i.SectorId = s.SectorId;

-- ----- Strategy Linked Position View
Create or replace View StrategyLinkedPositionView as
Select s.Name StrategyName, i.Symbol, p.NumberBought, p.NumberSold, p.AvgAmountBought, p.AvgAmountSold,
p.TotalAmountBought, p.TotalAmountSold, p.TotalAmountCommission, p.RealizedPnl, p.UpdatedDate,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType
from StrategyLinkedPosition p
inner join Strategy s on p.StrategyId = s.StrategyId
inner join Instrument i on p.InstrumentId = i.InstrumentId;

-- ----- StrategyBuy List View
Create or Replace view StrategyBuyListView as 
Select s.Name StrategyName, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType
from StrategyBuyList l
inner join Strategy s on l.StrategyId = s.StrategyId
inner join Instrument i on l.InstrumentId = i.InstrumentId;

-- --------- DailyHistoryBar View
Create or replace view DailyHistoryBarView as
select i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType,
h.Open, h.Close, h.High, h.Low, h.Volume,
e.Name ExchangeName, c.Code CountryCode, h.HistoryDate
from DailyHistoryBar h
inner join Instrument i on h.InstrumentId = i.InstrumentId
inner join Exchange e on i.ExchangeId = e.ExchangeId
left join Country c on i.CountryId = c.CountryId;