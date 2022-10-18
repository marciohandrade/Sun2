#include "stdafx.h"
#include ".\snapshotdata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

SnapShotData::SnapShotData(VOID)
{
	SetCategory( LOG_SNAPSHOT );
}

SnapShotData::~SnapShotData(VOID)
{
}

VOID SnapShotData::Init()
{
	BaseLogData::Init();
	
	ZeroMemory( m_byCharStat, MAX_CHAR_STAT_SIZE_MAX);
	ZeroMemory( m_byInventory, MAX_INVENTORYITEM_SIZE_MAX);
	ZeroMemory( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_MAX);
	ZeroMemory( m_byEquipItem, MAX_EQUIPITEM_SIZE_MAX);
	ZeroMemory( m_bySkill, MAX_SKILL_SIZE);	
	ZeroMemory( m_byMission, MAX_MISSION_SIZE);	
	ZeroMemory( m_byProgrQuest, MAX_PROGR_QUEST_SIZE);
	ZeroMemory( m_byComplQuest, MAX_COMPL_QUEST_SIZE);	
	ZeroMemory( m_byAC, MAX_AC_SIZE);
}

VOID	SnapShotData::SetCharStat( BYTE* pbyCharStat, DWORD MaxSize )
{
	if( !pbyCharStat )
		return;

	ASSERT( MAX_CHAR_STAT_SIZE_NOW == MaxSize);
											
	memcpy( m_byCharStat, pbyCharStat, min( MAX_CHAR_STAT_SIZE_NOW, MaxSize ) );
}

VOID	SnapShotData::SetInventory( BYTE* pbyInventory, DWORD MaxSize )
{
	if( !pbyInventory )
		return;

	ASSERT( MAX_INVENTORYITEM_SIZE_NOW == MaxSize);

	memcpy( m_byInventory, pbyInventory, min( MAX_INVENTORYITEM_SIZE_NOW, MaxSize) );
}

VOID	SnapShotData::SetTempInventory( BYTE* pBYTEmpInventory, DWORD MaxSize )
{
	if( !pBYTEmpInventory )
		return;

	ASSERT( MAX_TEMPINVENTORYITEM_SIZE_NOW == MaxSize);

	memcpy( m_byTempInventory, pBYTEmpInventory, min( MAX_TEMPINVENTORYITEM_SIZE_NOW, MaxSize ) );
}

VOID	SnapShotData::SetEquipItem( BYTE* pbyEquipItem, DWORD MaxSize )
{
	if( !pbyEquipItem )
		return;

	ASSERT( MAX_EQUIPITEM_SIZE_NOW == MaxSize);

	memcpy( m_byEquipItem, pbyEquipItem, min( MAX_EQUIPITEM_SIZE_NOW, MaxSize ) );
}

VOID	SnapShotData::SetSkill( BYTE* pbySkill, DWORD MaxSize )
{
	if( !pbySkill )
		return;

	ASSERT( MAX_SKILL_SIZE == MaxSize);

	memcpy( m_bySkill, pbySkill, min( MAX_SKILL_SIZE, MaxSize ) );
}

//VOID	SnapShotData::SetQuest( BYTE* pbyQuest )
//{
//	if( !pbyQuest )
//		return;
//
//	memcpy( m_byQuest, pbyQuest, MAX_QUEST_SIZE );
//}

VOID	SnapShotData::SetMission( BYTE* pbyMission, DWORD MaxSize )
{
	if( !pbyMission )
		return;

	ASSERT( MAX_MISSION_SIZE == MaxSize);

	memcpy( m_byMission, pbyMission, min( MAX_MISSION_SIZE, MaxSize ) );
}

VOID	SnapShotData::SetAC( BYTE* pbyAC, DWORD MaxSize )
{
	if( !pbyAC )
		return;
	
	ASSERT( MAX_AC_SIZE == MaxSize);
	
	memcpy( m_byAC, pbyAC, min( MAX_AC_SIZE, MaxSize ) );
}
VOID	SnapShotData::SetProgrQuest( BYTE* pbyProgrQuest, DWORD MaxSize )
{
	if( !pbyProgrQuest )
		return;
	
	ASSERT( MAX_PROGR_QUEST_SIZE == MaxSize);

	memcpy( m_byProgrQuest, pbyProgrQuest, min( MAX_PROGR_QUEST_SIZE, MaxSize ) );
}
VOID	SnapShotData::SetComplQuest( BYTE* pbyComplQuest, DWORD MaxSize )
{
	if( !pbyComplQuest )
		return;
	
	ASSERT( MAX_COMPL_QUEST_SIZE == MaxSize);

	memcpy( m_byComplQuest, pbyComplQuest, min( MAX_COMPL_QUEST_SIZE, MaxSize ) );
}

VOID SnapShotData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)SNAPSHOTDATA_LAST_VERSION;

	rArchive << (DWORD)LOG_BINARY_ITEM_VERSION;
	rArchive << (DWORD)LOG_BINARY_BASE_PLAYERINFO_VERSION;

	switch(LOG_BINARY_BASE_PLAYERINFO_VERSION)
	{
	case 1:
		rArchive.Write( m_byCharStat, MAX_CHAR_STAT_SIZE_V1 );
		break;
	case 2:
		rArchive.Write( m_byCharStat, MAX_CHAR_STAT_SIZE_V2 );
		break;
	case 3:
		rArchive.Write( m_byCharStat, MAX_CHAR_STAT_SIZE_V3 );
		break;
	case 4:
		rArchive.Write( m_byCharStat, MAX_CHAR_STAT_SIZE_V4 );
		break;
	default:
		ASSERT(FALSE);
		break;
	}	

	//바이너리
	switch(SNAPSHOTDATA_LAST_VERSION)
	{
	case 1:		
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V1 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	case 2:
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V1 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	case 3:		
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V2 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	case 4:
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V2 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	case 5:
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V3 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	case 6:
		rArchive.Write( m_byInventory, MAX_INVENTORYITEM_SIZE_V3 );
		rArchive.Write( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V3 );
		rArchive.Write( m_byEquipItem, MAX_EQUIPITEM_SIZE_V4 );
		rArchive.Write( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Write( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Write( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Write( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Write( m_byAC, MAX_AC_SIZE );
		break;
	}

	//스냅샷은 택스트 로그 안 남긴다.
}


VOID SnapShotData::Read(CLogArchive& rArchive)
{	
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;

	rArchive >> m_ItemBinaryVersion;
	rArchive >> m_BasePlayerInfoBinaryVersion;

	switch(m_BasePlayerInfoBinaryVersion)
	{
	case 1:
		rArchive.Read( m_byCharStat, MAX_CHAR_STAT_SIZE_V1 );
		break;
	case 2:
		rArchive.Read( m_byCharStat, MAX_CHAR_STAT_SIZE_V2 );
		break;
	case 3:
		rArchive.Read( m_byCharStat, MAX_CHAR_STAT_SIZE_V3 );
		break;
	case 4:
		rArchive.Read( m_byCharStat, MAX_CHAR_STAT_SIZE_V4 );
		break;
	default:
		ASSERT(FALSE);
		break;
	}	

	//바이너리
	switch(m_ReadVersion)
	{
	case 1:		
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V1 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	case 2:
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V1 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V1 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V1 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	case 3:
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V2 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	case 4:
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V2 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	case 5:
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V2 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V3 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	case 6:
		rArchive.Read( m_byInventory, MAX_INVENTORYITEM_SIZE_V3 );
		rArchive.Read( m_byTempInventory, MAX_TEMPINVENTORYITEM_SIZE_V3 );
		rArchive.Read( m_byEquipItem, MAX_EQUIPITEM_SIZE_V4 );
		rArchive.Read( m_bySkill, MAX_SKILL_SIZE );	
		rArchive.Read( m_byMission, MAX_MISSION_SIZE );	
		rArchive.Read( m_byProgrQuest, MAX_PROGR_QUEST_SIZE );
		rArchive.Read( m_byComplQuest, MAX_COMPL_QUEST_SIZE );	
		rArchive.Read( m_byAC, MAX_AC_SIZE );
		break;
	}
}

#endif