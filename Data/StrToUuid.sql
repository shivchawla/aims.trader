-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`root`@`localhost` FUNCTION `StrToUuid`(id varchar(38)) RETURNS binary(16)
BEGIN 
Declare temp binary(16);
Set temp := UNHEX(trim(replace(replace(replace(id, '{', ''), '}', ''), '-', '')));
if temp = 0x00000000000000000000000000000000 then return null;
else RETURN temp;
end if;
END
