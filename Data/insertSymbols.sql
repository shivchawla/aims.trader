
SET foreign_key_checks = 0;
load data local infile '/Users/shivkumarchawla/Desktop/Symbols.txt'
into table Instrument
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n' 
IGNORE 1 lines
(Symbol, FullName, @MarketCap, @Industry, ExchangeCode, @Sector, SectorCode)
set CountryCode = 'US',
Type = 0;

SET foreign_key_checks = 0;
load data local infile '/Users/shivkumarchawla/Desktop/Sectors.txt'
into table Sector
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n' 
IGNORE 0 lines
(Code, Name);
SET foreign_key_checks = 1;






