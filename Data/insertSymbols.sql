
SET foreign_key_checks = 0;
load data local infile '/Users/shivkumarchawla/Desktop/Symbols.txt'
into table Instrument
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n' 
IGNORE 1 lines
(Symbol, FullName, @MarketCap, @Industry, ExchangeCode, @Sector, SectorCode)
set CountryCode = 'US',
Type = 0;






