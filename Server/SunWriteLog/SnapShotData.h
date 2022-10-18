#pragma once

#include "BaseLogData.h"

class DLL_DECLARE SnapShotData : public BaseLogData
{
public:
	SnapShotData(VOID);
	~SnapShotData(VOID);

public:

    __LOG_PROPERTY(WORD, ACStreamVersion );
    __LOG_PROPERTY(WORD, QuestStreamVersion );
	__LOG_PROPERTY(WORD, ReadVersion );
	__LOG_PROPERTY(WORD, ItemBinaryVersion );

	BYTE 	m_byCharStat[MAX_BASE_PLAYER_INFO_SIZE];	
	BYTE 	m_byInventory[MAX_INVENTORYITEMSTREAM_SIZE];
	BYTE 	m_byTempInventory[MAX_TEMPINVENTORYITEMSTREAM_SIZE];
	BYTE 	m_byEquipItem[MAX_EQUIPITEMSTREAM_SIZE];
	BYTE 	m_bySkill[MAX_SKILLSTREAM_SIZE];
	BYTE 	m_byMission[MAX_MISSIONSTREAM_SIZE];
	BYTE	m_byProgrQuest[MAX_PROGR_QUESTSTREAM_SIZE];
	BYTE	m_byComplQuest[MAX_COMPL_QUESTSTREAM_SIZE];
	BYTE	m_byAC[MAX_ACSTREAM_SIZE];

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	VOID	Init();	
	//setter Method
public:
	inline VOID	SetCharStat( BYTE* pbyCharStat, DWORD MaxSize );
	inline VOID	SetInventory( BYTE* pbyInventory, DWORD MaxSize );
	inline VOID	SetTempInventory( BYTE* pbyTempInventory, DWORD MaxSize );
	inline VOID	SetEquipItem( BYTE* pbyEquipItem, DWORD MaxSize );
	inline VOID	SetSkill( BYTE* pbySkill, DWORD MaxSize );	
	inline VOID	SetMission( BYTE* pbyMission, DWORD MaxSize );	
	inline VOID	SetAC( BYTE* pbyAC, DWORD MaxSize );
	inline VOID	SetProgrQuest( BYTE* pbyProgrQuest, DWORD MaxSize );
	inline VOID	SetComplQuest( BYTE* pbyComplQuest, DWORD MaxSize );
public:
	inline BYTE*	GetCharStatPtr(){ return m_byCharStat; }
	inline BYTE*	GetInventoryPtr(){ return m_byInventory; }
	inline BYTE*	GetTempInventoryPtr(){ return m_byTempInventory; }
	inline BYTE*	GetEquipItemPtr(){ return m_byEquipItem; }
	inline BYTE*	GetSkillPtr(){ return m_bySkill; }	
	inline BYTE*	GetMissionPtr(){ return m_byMission; }	
	inline BYTE*	GetACPtr(){	return m_byAC; }
	inline BYTE*	GetProgrQuestPtr(){ return m_byProgrQuest; }
	inline BYTE*	GetComplQuestPtr(){ return m_byComplQuest; }	

public:
	inline int		GetCharStatSize(){ return sizeof(m_byCharStat); }
	inline int		GetInventorySize(){ return sizeof(m_byInventory); }
	inline int		GetTempInventorySize(){ return sizeof(m_byTempInventory); }
	inline int		GetEquipItemSize(){ return sizeof(m_byEquipItem); }
	inline int		GetSkillSize(){ return sizeof(m_bySkill); }	
	inline int		GetMissionSize(){ return sizeof(m_byMission); }	
	inline int		GetACSize(){ return sizeof(m_byAC); }
	inline int		GetProgrQuestSize(){ return sizeof(m_byProgrQuest); }
	inline int		GetComplQuestSize(){ return sizeof(m_byComplQuest); }
};