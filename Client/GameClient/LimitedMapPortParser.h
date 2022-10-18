#pragma once

//=======================================================================================================================
/// LimitedMapPortParser class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 1. 3
	@remarks
			- 마을에서 이동가능한 맵을 나타내는 포트 스크립트 파서
	@note
			- 마을은 이용료가 포함되어있다.
	@history
		
*/
//=======================================================================================================================

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>


enum eVILLAGE_NAME
{
	eVILLAGE_TRINGEL,
	eVILLAGE_WALDCHEN,
	eVILLAGE_ETHERAIN,
	eVILLAGE_AAUKRA,
	eVILLAGE_MAX,
};
enum
{
	_MAX_MISSION_MAP_PORT_NUM	= 9,
	_MAX_HUNTING_MAP_PORT_NUM	= 9,
	_MAX_PVP_MAP_PORT_NUM		= 6,
	_MAX_QUEST_MAP_PORT_NUM		= 6,
	_MAX_CHUNTING_MAP_PORT_NUM	= 6,
};
typedef struct _sVILLAGEPORT
{
	MAPCODE		m_PreMissionCode;
	MONEY		m_Fare;
	MAPCODE		m_MapCode;
} sVILLAGEPORT;

struct sMapPort
{	
	MAPCODE			m_VillageMapCode;
	TCHAR			m_tszDebugVillageName[MAX_MAPNAME_LENGTH+1];
	sVILLAGEPORT	m_VillagePort[eVILLAGE_MAX];
	MAPCODE			m_MissionMapPort[_MAX_MISSION_MAP_PORT_NUM];
	MAPCODE			m_HuntingMapPort[_MAX_HUNTING_MAP_PORT_NUM];
	MAPCODE			m_PVPMapPort[_MAX_PVP_MAP_PORT_NUM];
	MAPCODE			m_QuestMapPort[_MAX_QUEST_MAP_PORT_NUM];
	MAPCODE			m_CHuntingMapPort[_MAX_CHUNTING_MAP_PORT_NUM];
};

class LimitedMapPortParser : public util::Singleton<LimitedMapPortParser>, public IParser
{
	typedef util::SolarHashTable<sMapPort *>				MAPPORT_HASH;
	typedef util::SolarHashTable<sMapPort *>::iterator		MAPPORT_HASH_ITR;
public:
	LimitedMapPortParser(void);
	virtual ~LimitedMapPortParser(void);

	VOID					Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID					Init(DWORD dwPoolSize);

	VOID					Release(){ Unload(); };

	virtual BOOL			LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	VOID					Unload();

	inline sMapPort*        FindVillage(MAPCODE code)				{ return m_VillageMapPortHash.GetData(code); }

protected:
	inline VOID             RemoveVillage(MAPCODE code)					{ m_VillageMapPortHash.Remove(code); }
	inline VOID             AddVillage(sMapPort* pVillage, MAPCODE code)	{ m_VillageMapPortHash.Add(pVillage, code); }

private:
	MAPPORT_HASH            m_VillageMapPortHash;
	char                    m_tszFileName[MAX_PATH];
};
