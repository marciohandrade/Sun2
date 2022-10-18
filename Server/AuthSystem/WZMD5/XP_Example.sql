use master
go


DECLARE @ID		NVARCHAR(50)
DECLARE @Password	NVARCHAR(50)
DECLARE @Encoded	BINARY(16)
DECLARE @Result	TINYINT


SET @ID = 'ReinLainer'
SET @Password = '12345'


EXEC master..XP_MD5_CryptKey @ID, @Password, @Encoded OUT
SELECT @Encoded

EXEC master..XP_MD5_Compare N'ReinLainer', @Encoded, @Password, @Result OUT
SELECT @Result