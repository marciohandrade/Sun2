#include "SunClientPrecompiledHeader.h"
#include "LimitedMapPortParser.h"


LimitedMapPortParser::LimitedMapPortParser(void)
{
	m_VillageMapPortHash.Initialize( 50 );
}

LimitedMapPortParser::~LimitedMapPortParser(void)
{
	Unload();
}


VOID LimitedMapPortParser::Init( DWORD dwPoolSize )
{
	m_VillageMapPortHash.Initialize( dwPoolSize );
}

VOID LimitedMapPortParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	m_VillageMapPortHash.Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

VOID LimitedMapPortParser::Unload()
{
	for( MAPPORT_HASH_ITR it = m_VillageMapPortHash.begin() ; it != m_VillageMapPortHash.end() ; ++it )	
	{
		delete (*it);
	}
	m_VillageMapPortHash.RemoveAll();
}
#pragma warning ( push )
#pragma warning ( disable : 4244)


BOOL LimitedMapPortParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		MAPCODE VillageMapCode = GetDataNumber("wMapID", nRow);
		sMapPort * pMapPort = FindVillage(VillageMapCode);
		if (!pMapPort) 
		{
			// 없으면!
			pMapPort = new sMapPort;
			AddVillage(pMapPort, VillageMapCode);
		}
		else
		{
			if (FALSE == bReload)
			{
				assert(!"데이터에 오류가 있습니다.");
			}
		}

		pMapPort->m_VillageMapCode = VillageMapCode;
		memcpy(pMapPort->m_tszDebugVillageName, GetDataString("sMname", nRow), MAX_MAPNAME_LENGTH*sizeof(TCHAR));

		char szTemp[64] = {0,};

		for (int i = 0; i < eVILLAGE_MAX; ++i)
		{
			sprintf(szTemp, "wMission%d", i+1);
			pMapPort->m_VillagePort[i].m_PreMissionCode = GetDataNumber(szTemp, nRow);
			sprintf(szTemp, "wfare%d", i+1);
			pMapPort->m_VillagePort[i].m_Fare           = GetDataNumber(szTemp, nRow);
			sprintf(szTemp, "wMapCode%d", i+1);
			pMapPort->m_VillagePort[i].m_MapCode        = GetDataNumber(szTemp, nRow);
		}

		for (int i = 0; i < _MAX_MISSION_MAP_PORT_NUM; ++i)
		{
			sprintf(szTemp, "bMmap%d", i+1);
			pMapPort->m_MissionMapPort[i] = GetDataNumber(szTemp, nRow);
		}

		for (int i = 0; i < _MAX_HUNTING_MAP_PORT_NUM; ++i)
		{
			sprintf(szTemp, "wHmap%d", i+1);
			pMapPort->m_HuntingMapPort[i] = GetDataNumber(szTemp, nRow);

		}

		for (int i = 0; i < _MAX_PVP_MAP_PORT_NUM; ++i)
		{
			sprintf(szTemp, "wPmap%d", i+1);
			pMapPort->m_PVPMapPort[i] = GetDataNumber(szTemp, nRow);
		}

		for (int i = 0; i < _MAX_QUEST_MAP_PORT_NUM; ++i)
		{
			sprintf(szTemp, "wQmap%d", i+1);
			pMapPort->m_QuestMapPort[i] = GetDataNumber(szTemp, nRow);
		}
		
	}

	return TRUE;
}


#pragma warning ( pop )
