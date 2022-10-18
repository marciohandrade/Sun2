#pragma once

#include "../lib/SQLAPI/SQLAPI.h" // main SQLAPI++ header
#pragma comment(lib, "../lib/SQLAPI/sqlapi.lib")


#define TRY_DB_EXEC	try{
#define CATCH_DB_EXECPTION(ptr) }catch(SAException &x)	{\
	try{ ptr.Rollback();}		\
	catch(SAException &) {}		\
	printf("%s\n", (const char*)x.ErrText());\
	return FALSE;}


class CDBTest
{
public:
	BOOL Test_EquipItem(ICSunOnlinePtr ptr, int nBinVer);
	BOOL DBConnect(const char* szIP, const int nPort, const char* szDBName, const char* ID, const char* pwd);	
	BOOL Test_Skill(ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_Warehouse(ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_GuildWarehouse(ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_ACStream(const char* szID,  const char* szCharName, ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_CharStat(ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_Quest(const char* szID, const char* szCharName, ICSunOnlinePtr ptr, int nBinVer);
	BOOL Test_Mission(const char* szID, const char* szCharName, ICSunOnlinePtr ptr, int nBinVer);
    BOOL Test_Inventory(ICSunOnlinePtr ptr); 
    BOOL Test_Item(ICSunOnlinePtr ptr); 
protected:
	SAConnection	m_sqlconn;
	SACommand		m_sqlcmd;
};