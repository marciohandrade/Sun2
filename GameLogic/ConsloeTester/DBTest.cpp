#include "stdafx.h"

#include "DBTest.h"

BOOL CDBTest::DBConnect(const char* szIP, const int nPort, const char* szDBName, const char* ID, const char* pwd)
{
	char szConn[1024] = {0};
	sprintf(szConn,"%s,%d@%s",szIP,nPort,szDBName);

	TRY_DB_EXEC

	m_sqlconn.Connect(szConn, ID, pwd, SA_SQLServer_Client);
	m_sqlcmd.setConnection(&m_sqlconn);

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_Skill(ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];		
	
	byte szSkill[MAX_SKILL_SLOT_NUM*2] = {0};	
	
	sprintf(sql, "SELECT TOP 1 Skill FROM T_Character WHERE (UserGuid = 2511)");

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		SAString sz = m_sqlcmd.Field("Skill").asBytes();
		int nSize = sz.GetBinaryLength();
		memcpy(szSkill, sz.GetBinaryBuffer(nSize), nSize);

		ptr->decodeSkill(szSkill, eCHAR_BERSERKER);
		//ptr->decodeSkill(szSkill, eCHAREX_BERSERKER_100LEVEL);

		break;

	}

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;		
}

BOOL CDBTest::Test_EquipItem(ICSunOnlinePtr ptr, int nBinVer)
{

	char sql[1024];
	_bstr_t bstTest;

	byte szEquipItem[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};	
	
	sprintf(sql, "SELECT EquipItem, Version FROM T_Character WHERE CharGuid = 66");

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		SAString sz = m_sqlcmd.Field("EquipItem").asBytes();
		int nSize = sz.GetBinaryLength();
		memcpy(szEquipItem, sz.GetBinaryBuffer(nSize), nSize);

        long Version = m_sqlcmd.Field("Version").asLong();

		bstTest = ptr->decodeEquipItem(szEquipItem,Version);

		break;
	}

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_Warehouse(ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];
	_bstr_t bstTest;

    const DWORD warehose_size = MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX);
	byte szWareHouse[warehose_size] = {0};	
    
	//sprintf(sql, "select CharStat from T_CharLog20080428");
	sprintf(sql, "  SELECT WarehouseItem, Version FROM T_Warehouse WHERE UserGuid = 10");
    //sprintf(sql, "  SELECT WarehouseItem FROM T_Warehouse WHERE UserGuid = 226");

	TRY_DB_EXEC

    m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		SAString sz = m_sqlcmd.Field("WarehouseItem").asBytes();
		int nSize = sz.GetBinaryLength();
		memcpy(szWareHouse, sz.GetBinaryBuffer(nSize), nSize);

        long Version = m_sqlcmd.Field("Version").asLong();

		bstTest = ptr->decodeWarehouseItem(szWareHouse,Version,_bstr_t(""));


        int nsize, iNewVer11;
        byte buffwarehouse1[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};
        ptr->encodeWarehouseItem(bstTest, buffwarehouse1,&iNewVer11);
        bstTest = ptr->decodeWarehouseItem(buffwarehouse1,Version,_bstr_t(""));

		//break;
	}

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_GuildWarehouse(ICSunOnlinePtr ptr, int nBinVer)
{
#ifdef __NA000584_20080201_GUILD_WAREHOUSE
	char sql[1024];
	_bstr_t bstTest;

	byte szGuildWareHouse[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)] = {0};	

	//sprintf(sql, "select CharStat from T_CharLog20080428");
	sprintf(sql, "  SELECT WarehouseItem FROM T_GuildWarehouse WHERE GuildGuid = '330'");

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		SAString sz = m_sqlcmd.Field("WarehouseItem").asBytes();
		int nSize = sz.GetBinaryLength();
		memcpy(szGuildWareHouse, sz.GetBinaryBuffer(nSize), nSize);

		bstTest = ptr->decodeGuildWarehouseItem(szGuildWareHouse,nBinVer,_bstr_t(""));

		//break;
	}

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CDBTest::Test_CharStat(ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];

	int aaa = sizeof(BASE_PLAYERINFO);
	byte szCharStat[sizeof(BASE_PLAYERINFO)] = {0};	

	//sprintf(sql, "select CharStat from T_CharLog20080428");
	sprintf(sql, "select CharStat from T_CharLog20080508");

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		SAString sz = m_sqlcmd.Field("CharStat").asBytes();
		int nSize = sz.GetBinaryLength();
		memcpy(szCharStat, sz.GetBinaryBuffer(nSize), nSize);

		ptr->decodeCharStat(szCharStat);

		//break;
	}

	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_ACStream(const char* szID, const char* szCharName, ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];

	byte szACEStream[MAX_ACSTREAM_SIZE] = {0};
	byte szMission[MAX_MISSIONSTREAM_SIZE] = {0};
	byte szCompIQStream[MAX_COMPL_QUESTSTREAM_SIZE] = {0};

	sprintf(sql, "exec USP_SelectUser 3, '%s', 0", szCharName);

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		if(!m_sqlcmd.Field("CharName").isNull())
		{
			SAString str = m_sqlcmd.Field("CharName").asString();

			if(0 == strcmp(szCharName, str))
			{
				SAString sz = m_sqlcmd.Field("ACStream").asBytes();
				int nSize = sz.GetBinaryLength();
				memcpy(szACEStream, sz.GetBinaryBuffer(nSize), nSize);

				sz = m_sqlcmd.Field("Mission").asBytes();
				nSize = sz.GetBinaryLength();
				memcpy(szMission, sz.GetBinaryBuffer(nSize), nSize);

				sz = m_sqlcmd.Field("ComplQStream").asBytes();
				nSize = sz.GetBinaryLength();
				memcpy(szCompIQStream, sz.GetBinaryBuffer(nSize), nSize);

				ptr->decodeACStream(szACEStream,AC_STREAM_SIZE_LAST_VERSION,szMission,szCompIQStream);				

				break;
			}				
		}		

	}
	
	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_Quest(const char* szID, const char* szCharName, ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];

	BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE] = {0};
	BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE] = {0};

	//sprintf(sql, "exec USP_SelectUser 16762, '%s', 0", szCharName);
	//sprintf(sql, "select ProgrQstream, ComplQstream from T_Quest where CharGuid = 16762");
	sprintf(sql, "SELECT * FROM T_Quest WHERE CharGuid = 17515");

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		//if(!m_sqlcmd.Field("CharName").isNull())
		{
			//SAString str = m_sqlcmd.Field("CharName").asString();

			//if(0 == strcmp(szCharName, str))
			{
				
				SAString sz = m_sqlcmd.Field("ProgrQStream").asBytes();
				int nSize = sz.GetBinaryLength();
				memcpy(ProgrQStream, sz.GetBinaryBuffer(nSize), nSize);

				sz = m_sqlcmd.Field("ComplQStream").asBytes();
				nSize = sz.GetBinaryLength();
				memcpy(ComplQStream, sz.GetBinaryBuffer(nSize), nSize);

				ptr->decodeQuest(ProgrQStream,ComplQStream);			

				break;
			}				
		}		

	}
	
	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}

BOOL CDBTest::Test_Mission(const char* szID, const char* szCharName, ICSunOnlinePtr ptr, int nBinVer)
{
	char sql[1024];

	BYTE buff[MAX_MISSIONSTREAM_SIZE];

	sprintf(sql, "exec USP_SelectUser 2870, '%s', 0", szCharName);

	TRY_DB_EXEC

	m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
	m_sqlcmd.Execute();

	while( m_sqlcmd.FetchNext() )
	{
		if(!m_sqlcmd.Field("CharName").isNull())
		{
			SAString str = m_sqlcmd.Field("CharName").asString();

			//if(0 == strcmp(szCharName, str))
			{				
				SAString sz = m_sqlcmd.Field("Mission").asBytes();
				int nSize = sz.GetBinaryLength();
				memcpy(buff, sz.GetBinaryBuffer(nSize), nSize);				

				ptr->decodeMission(buff);			

				break;
			}				
		}		

	}
	
	CATCH_DB_EXECPTION(m_sqlconn)

	return TRUE;
}



BOOL CDBTest::Test_Inventory(ICSunOnlinePtr ptr)
{
    char sql[1024];

    BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)];

    //sprintf(sql, "select CharGuid, Version, ExtraInvenTabCount,InventoryItem from dbo.T_Character where CharName = '123123'");
	sprintf(sql, "select * from dbo.T_CharacterInventory where CharGuid = 18026");

    TRY_DB_EXEC

    m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
    m_sqlcmd.Execute();

    while( m_sqlcmd.FetchNext() )
    {
        if(!m_sqlcmd.Field("CharGuid").isNull())
        {

            long CharGuid = m_sqlcmd.Field("CharGuid").asLong();
            SAString sz = m_sqlcmd.Field("InventoryItem").asBytes();
            int nSize = sz.GetBinaryLength();
            memcpy(buff, sz.GetBinaryBuffer(nSize), nSize);

            //long Version = m_sqlcmd.Field("Version").asLong();
            //long ExtraInvenTabCount = m_sqlcmd.Field("ExtraEquipCashSlotCount").asLong();

            _bstr_t bstr = ptr->decodeInventoryItem(buff, ptr->GetItemStreamVersion(), 0, "");
            //int iNewVer11 = 0;
            //ptr->encodeInventoryItem(bstr, buff,&iNewVer11);
            //ptr->decodeInventoryItem(buff, Version, ExtraInvenTabCount, "");
        }		

    }

    CATCH_DB_EXECPTION(m_sqlconn)

    return TRUE;
}

BOOL CDBTest::Test_Item(ICSunOnlinePtr ptr)
{
    char sql[1024];

    BYTE buff[sizeof(OPTIONPART)];

    sprintf(sql, "select top 1 * from dbo.T_ItemSlotInfo");

    TRY_DB_EXEC

    m_sqlcmd.setCommandText(sql, SA_CmdUnknown);	
    m_sqlcmd.Execute();

    while( m_sqlcmd.FetchNext() )
    {
        if(!m_sqlcmd.Field("CharGuid").isNull())
        {

            long CharGuid = m_sqlcmd.Field("CharGuid").asLong();
            long SlotType = m_sqlcmd.Field("SlotType").asLong();
            long Pos = m_sqlcmd.Field("Pos").asLong();
            __int64 Serial = static_cast<__int64>(m_sqlcmd.Field("Serial").asDouble());
            int Code = static_cast<int>(m_sqlcmd.Field("Code").asLong());
            BYTE DuraOrNum = static_cast<BYTE>(m_sqlcmd.Field("DuraOrNum").asLong());
            BYTE Version = static_cast<BYTE>(m_sqlcmd.Field("Version").asLong());

            SAString sz = m_sqlcmd.Field("OptionStream").asBytes();
            int nSize = sz.GetBinaryLength();
            memcpy(buff, sz.GetBinaryBuffer(nSize), nSize);

            _bstr_t bstr = ptr->DecodeItemInfo(static_cast<BYTE>(Pos),
                Serial,
                Code,
                DuraOrNum,
                Version,
                buff);

            ptr->EncodeItemInfo(bstr, &Serial, &Code, &DuraOrNum, &Version, buff);

            bstr = ptr->DecodeItemInfo(static_cast<BYTE>(Pos),
                Serial,
                Code,
                DuraOrNum,
                Version,
                buff);
            
            
        }		

    }

    CATCH_DB_EXECPTION(m_sqlconn)

    return TRUE;
}