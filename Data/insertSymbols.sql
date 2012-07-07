
SET foreign_key_checks = 0;
load data local infile '/Users/shivkumarchawla/Desktop/Symbols.txt'
into table Instrument
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n' 
IGNORE 1 lines
(Symbol, FullName, @MarketCap, @Industry, @Exchange, @Sector,@SectorCode)
Set InstrumentId = MyUUID(),
ExchangeId = (select ExchangeId from Exchange where name = @Exchange), 
SectorId = (select SectorId from Sector where code = @SectorCode),
CountryId = (select CountryId from Country where Code='US'),
Type = '0',
UpdatedBy = "SCHAWLA",
UpdatedDate = NOW();






