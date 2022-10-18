use master
go

--  DLL Install
sp_addextendedproc 'XP_MD5_CryptKey', 'WZ_MD5.dll'
go

sp_addextendedproc 'XP_MD5_Compare', 'WZ_MD5.dll'
go