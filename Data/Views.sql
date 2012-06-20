-- ----- Order View
Create View OrderView as 
select BINTOUUID(o.OrderId), o.LimitPrice, o.Quantity, 
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
Create View InstrumentView as 
Select BINTOUUID(InstrumentId), Symbol, ShortName, Fullname,
Case Type When '0' then 'Equity'
          When '1' then 'Future'
          When '2' then 'Option'
Else 'Unknown'
End As Type,
UpdatedBy, UpdatedDate
from Instrument;

-- ----- Strategy Linked Position View
Create or replace View StrategyLinkedPositionView as
Select BINTOUUID(p.StrategyLinkedPositionId), p.NumberBought, p.NumberSold, p.AvgAmountBought, p.AvgAmountSold,
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
Create or Replace view StrategyBuyListView as 
Select BINTOUUID(l.StrategyBuyListId), s.Name StrategyName, i.Symbol,
Case i.Type When '0' then 'Equity'
            When '1' then 'Future'
            When '2' then 'Option'
Else 'Unknown'
End As InstrumentType
from StrategyBuyList l
inner join Strategy s on l.StrategyId = s.StrategyId
inner join Instrument i on l.InstrumentId = i.InstrumentId;