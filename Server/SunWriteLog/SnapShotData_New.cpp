#include "stdafx.h"
#include ".\snapshotdata.h"

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

	ZeroMemory( m_byCharStat, MAX_BASE_PLAYER_INFO_SIZE);
	ZeroMemory( m_byInventory, MAX_INVENTORYITEMSTREAM_SIZE);
	ZeroMemory( m_byTempInventory, MAX_TEMPINVENTORYITEMSTREAM_SIZE);
	ZeroMemory( m_byEquipItem, MAX_EQUIPITEMSTREAM_SIZE);
	ZeroMemory( m_bySkill, MAX_SKILLSTREAM_SIZE);	
	ZeroMemory( m_byMission, MAX_MISSIONSTREAM_SIZE);	
	ZeroMemory( m_byProgrQuest, MAX_PROGR_QUESTSTREAM_SIZE);
	ZeroMemory( m_byComplQuest, MAX_COMPL_QUESTSTREAM_SIZE);	
	ZeroMemory( m_byAC, MAX_ACSTREAM_SIZE);
}

VOID	SnapShotData::SetCharStat( BYTE* pbyCharStat, DWORD MaxSize )
{
	if( !pbyCharStat )
		return;	

	ASSERT( MAX_BASE_PLAYER_INFO_SIZE == MaxSize);

	memcpy( m_byCharStat, pbyCharStat, min( MAX_BASE_PLAYER_INFO_SIZE, MaxSize ) );
}

VOID	SnapShotData::SetInventory( BYTE* pbyInventory, DWORD MaxSize )
{
	if( !pbyInventory )
		return;

	ASSERT( MAX_INVENTORYITEMSTREAM_SIZE == MaxSize);

	memcpy( m_byInventory, pbyInventory, min( MAX_INVENTORYITEMSTREAM_SIZE, MaxSize) );
}

VOID	SnapShotData::SetTempInventory( BYTE* pBYTEmpInventory, DWORD MaxSize )
{
	if( !pBYTEmpInventory )
		return;

	ASSERT( MAX_TEMPINVENTORYITEMSTREAM_SIZE == MaxSize);

	memcpy( m_byTempInventory, pBYTEmpInventory, min( MAX_TEMPINVENTORYITEMSTREAM_SIZE, MaxSize ) );
}

VOID	SnapShotData::SetEquipItem( BYTE* pbyEquipItem, DWORD MaxSize )
{
	if( !pbyEquipItem )
		return;

	ASSERT( MAX_EQUIPITEMSTREAM_SIZE == MaxSize);

	memcpy( m_byEquipItem, pbyEquipItem, min( MAX_EQUIPITEMSTREAM_SIZE, MaxSize ) );
}

VOID	SnapShotData::SetSkill( BYTE* pbySkill, DWORD MaxSize )
{
	if( !pbySkill )
		return;

	ASSERT( MAX_SKILLSTREAM_SIZE == MaxSize);

	memcpy( m_bySkill, pbySkill, min( MAX_SKILLSTREAM_SIZE, MaxSize ) );
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

	ASSERT( MAX_MISSIONSTREAM_SIZE == MaxSize);

	memcpy( m_byMission, pbyMission, min( MAX_MISSIONSTREAM_SIZE, MaxSize ) );
}

VOID	SnapShotData::SetAC( BYTE* pbyAC, DWORD MaxSize )
{
	if( !pbyAC )
		return;

	ASSERT( MAX_ACSTREAM_SIZE == MaxSize);

	memcpy( m_byAC, pbyAC, min( MAX_ACSTREAM_SIZE, MaxSize ) );
}
VOID	SnapShotData::SetProgrQuest( BYTE* pbyProgrQuest, DWORD MaxSize )
{
	if( !pbyProgrQuest )
		return;

	ASSERT( MAX_PROGR_QUESTSTREAM_SIZE == MaxSize);

	memcpy( m_byProgrQuest, pbyProgrQuest, min( MAX_PROGR_QUESTSTREAM_SIZE, MaxSize ) );
}
VOID	SnapShotData::SetComplQuest( BYTE* pbyComplQuest, DWORD MaxSize )
{
	if( !pbyComplQuest )
		return;

	ASSERT( MAX_COMPL_QUESTSTREAM_SIZE == MaxSize);

	memcpy( m_byComplQuest, pbyComplQuest, min( MAX_COMPL_QUESTSTREAM_SIZE, MaxSize ) );
}

VOID SnapShotData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	WORD wSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)SNAPSHOTDATA_LAST_VERSION;	
	
	switch(SNAPSHOTDATA_LAST_VERSION)
	{
		case 1:
		{
			//BASE_PLAYERINFO
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byCharStat, rArchive);

			//ITEM
			rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;

			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byInventory, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byTempInventory, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byEquipItem, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_bySkill, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byMission, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byProgrQuest, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byComplQuest, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byAC, rArchive);
		}
		break;
		case  2:
		{
			//BASE_PLAYERINFO
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byCharStat, rArchive);

			//ITEM
			rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;

			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byInventory, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byTempInventory, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byEquipItem, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_bySkill, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byMission, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byProgrQuest, rArchive);
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byComplQuest, rArchive);
			rArchive << (WORD)AC_STREAM_SIZE_LAST_VERSION;		
			_CHECK_LOG_SIZE_AND_WRITE( wSize, m_byAC, rArchive);
		}
		break;
        case  3:
            {
                //BASE_PLAYERINFO
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byCharStat, rArchive);

                //ITEM
                rArchive << (WORD)ITEM_STREAM_SIZE_LAST_VERSION;

                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byInventory, rArchive);
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byTempInventory, rArchive);
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byEquipItem, rArchive);
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_bySkill, rArchive);
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byMission, rArchive);
                rArchive << (WORD)QUEST_STREAM_SIZE_LAST_VERSION;		
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byProgrQuest, rArchive);
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byComplQuest, rArchive);
                rArchive << (WORD)AC_STREAM_SIZE_LAST_VERSION;		
                _CHECK_LOG_SIZE_AND_WRITE( wSize, m_byAC, rArchive);
            }
            break;
		default:
			{

			}
			break;
	}

	//스냅샷은 택스트 로그 안 남긴다.
}

VOID SnapShotData::Read(CLogArchive& rArchive)
{
	WORD wSize = 0;
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;

	switch( m_ReadVersion )
	{
	case 1:
		{
			//BASE_PLAYERINFO
			LOG_READ( wSize, m_byCharStat, rArchive );

			//ITEM
			rArchive >> m_ItemBinaryVersion;		

			LOG_READ( wSize, m_byInventory, rArchive );		
			LOG_READ( wSize, m_byTempInventory, rArchive );
			LOG_READ( wSize, m_byEquipItem, rArchive );
			LOG_READ( wSize, m_bySkill, rArchive );
			LOG_READ( wSize, m_byMission, rArchive );
			LOG_READ( wSize, m_byProgrQuest, rArchive );
			LOG_READ( wSize, m_byComplQuest, rArchive );
			LOG_READ( wSize, m_byAC, rArchive );
		}
		break;
	case 2:
		{
			//BASE_PLAYERINFO
			LOG_READ( wSize, m_byCharStat, rArchive );

			//ITEM
			rArchive >> m_ItemBinaryVersion;

			LOG_READ( wSize, m_byInventory, rArchive );
			LOG_READ( wSize, m_byTempInventory, rArchive );
			LOG_READ( wSize, m_byEquipItem, rArchive );
			LOG_READ( wSize, m_bySkill, rArchive );
			LOG_READ( wSize, m_byMission, rArchive );
			LOG_READ( wSize, m_byProgrQuest, rArchive );
			LOG_READ( wSize, m_byComplQuest, rArchive );
			rArchive >> m_ACStreamVersion;
			LOG_READ( wSize, m_byAC, rArchive );
		}
        break;
    case 3:
        {
            //BASE_PLAYERINFO
            LOG_READ( wSize, m_byCharStat, rArchive );

            //ITEM
            rArchive >> m_ItemBinaryVersion;

            LOG_READ( wSize, m_byInventory, rArchive );
            LOG_READ( wSize, m_byTempInventory, rArchive );
            LOG_READ( wSize, m_byEquipItem, rArchive );
            LOG_READ( wSize, m_bySkill, rArchive );
            LOG_READ( wSize, m_byMission, rArchive );
            rArchive >> m_QuestStreamVersion;
            LOG_READ( wSize, m_byProgrQuest, rArchive );
            LOG_READ( wSize, m_byComplQuest, rArchive );
            rArchive >> m_ACStreamVersion;
            LOG_READ( wSize, m_byAC, rArchive );
        }
        break;
	default:
		{
		}
		break;
	}
}

