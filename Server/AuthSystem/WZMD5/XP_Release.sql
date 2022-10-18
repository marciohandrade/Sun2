use master
go

--  DLL Release
sp_dropextendedproc 'XP_MD5_CryptKey'
go

sp_dropextendedproc 'XP_MD5_Compare'
go

dbcc WZ_MD5(free)
go