#include "StdAfx.h"
#include "Chaostate.h"
#include "Player.h"
#include "PlayerManager.h"
#include "ItemManager.h"
#include "PKScriptParser.h"
#include "GameServerEx.h"
#include <PacketStruct_AG.h>
#include "SCItemSlotContainer.h"
#include "DropManager.h"
#include "Summoned.h"

#include <PacketStruct_GZ.h>

#include "GameGuildExtension.h"


ChaoState::ChaoState( Player *pPlayer ) 
: m_pPlayer(pPlayer)
{
}

ChaoState::~ChaoState()
{
}

VOID ChaoState::Init()
{
	m_bBoradori = FALSE;

	m_BoradoriTimer.Clear();
	m_mapBadFriend.clear();
	memset( m_ptszRevengeName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );

	m_NotifyTimer.SetTimer( NOTIFY_CHAOTIME_UPDATE_CYCLE );
}

VOID ChaoState::Update()
{
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	// 보라돌이 유지시간이 다되면 해제된다.
	if( m_BoradoriTimer.IsExpired() )
	{
		EndBoradoriState();
	}

	// 일정시간이 지나면 카오상태가 해제된다.
	if( GetChaoState() != eCHAO_STATE_NORMAL ) 
	{
		time_t cur_time;	util::TimeSync::time( &cur_time );

		if( m_pPlayer->GetField() )
		{
			int iLeavedTime = GetChaoEndTime() - cur_time;

			if( iLeavedTime <= 0 )
				ClearChaoState();
			else if( iLeavedTime <= (int)stPKOptionInfo.m_PreChaoTime && IsChaoState() )
				DownChaoState();
		}

		// 1시간 간격으로 남은 카오시간을 알려준다.
		if( m_NotifyTimer.IsExpired() )
		{
			SendLeavedChaoTime();
		}
	}

	// 월드서버와의 연결이 끊어진 경우 리벤지 리스트 초기화!
	if( !g_pGameServer->IsConnectToWorldServer() )
	{
		m_mapBadFriend.clear();
	}
}

VOID ChaoState::OnDead()
{
	// 보라돌이 상태를 제거한다.
	EndBoradoriState();
}

VOID ChaoState::UpChaoState()
{
	// GM 맵에서는 카오성향을 올리지 않는다.
	GameField *pField = m_pPlayer->GetField();
	if( pField )
	{
		if( HardCode::MAPCODE_for_GM == pField->GetFieldCode() )
			return;
	}

	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	BOOL bChangeState = FALSE;
	time_t cur_time;	util::TimeSync::time( &cur_time );

	// 카오 상태, 끝나는 시간 갱신
	if( GetChaoState() == eCHAO_STATE_NORMAL )
	{
		bChangeState = TRUE;
		SetChaoState( eCHAO_STATE_PREV_CHAO );
		SetChaoEndTime( cur_time + stPKOptionInfo.m_PreChaoTime );
	}
	else if( GetChaoState() == eCHAO_STATE_PREV_CHAO )
	{
		bChangeState = TRUE;
		SetChaoState( eCHAO_STATE_CHAO );

		SetChaoEndTime( GetChaoEndTime() + stPKOptionInfo.m_ChaoTime );
	}
	else if( GetChaoState() == eCHAO_STATE_CHAO )
	{
		// 카오상태에서 또다시 상대를 죽이면 시간이 누적된다.
		SetChaoEndTime( GetChaoEndTime() + stPKOptionInfo.m_AddChaoTime );
	}

	// 카오상태 변경을 클라에 알려준다.
	if( bChangeState )
	{
		// 본인
		MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD cmdMsg;
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		// 주변
		MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD brdMsg;
		brdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		brdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );

		// 길드
		if( m_pPlayer->GetCharInfo()->m_GuildGuid )
		{
			MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN guildMsg;
			guildMsg.m_CharGuid = m_pPlayer->GetCharGuid();
			guildMsg.m_GuildGuid = m_pPlayer->GetCharInfo()->m_GuildGuid;
			guildMsg.m_byChaoState = GetChaoState();
			m_pPlayer->SendToGuildServer( &guildMsg, sizeof(guildMsg) );
		}
	}

	// 카오상태 변경 로그를 남긴다.
	GAMELOG->LogAction( ACT_PK_CHANGE_CHAOSTATE, m_pPlayer, NULL, GetChaoState(), GetChaoEndTime() );
}

VOID ChaoState::DownChaoState()
{
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	BOOL bChangeState = FALSE;
	time_t cur_time;	util::TimeSync::time( &cur_time );

	// 카오 상태 갱신
	if( GetChaoState() == eCHAO_STATE_PREV_CHAO )
	{
		bChangeState = TRUE;
		SetChaoState( eCHAO_STATE_NORMAL );
		SetChaoEndTime( 0 );
	}
	else if( GetChaoState() == eCHAO_STATE_CHAO )
	{
		bChangeState = TRUE;
		SetChaoState( eCHAO_STATE_PREV_CHAO );

		// 서버에서는 남은 시간이 변하지 않지만, 클라에서는 프리카오에 대한 남은 시간을 새로 갱신해야한다.
		SendLeavedChaoTime();
	}

	// 카오상태 변경을 클라에 알려준다.
	if( bChangeState )
	{
		// 본인
		MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD cmdMsg;
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		// 주변
		MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD brdMsg;
		brdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		brdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );
		// 길드
		if( m_pPlayer->GetCharInfo()->m_GuildGuid )
		{
			MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN guildMsg;
			guildMsg.m_CharGuid = m_pPlayer->GetCharGuid();
			guildMsg.m_GuildGuid = m_pPlayer->GetCharInfo()->m_GuildGuid;
			guildMsg.m_byChaoState = GetChaoState();
			m_pPlayer->SendToGuildServer( &guildMsg, sizeof(guildMsg) );

			MessageOut( eFULL_LOG, "Change ChaoState To GuildServer[G:%u][C:%u]", guildMsg.m_GuildGuid, guildMsg.m_CharGuid );
		}
	}

	// 카오상태 변경 로그를 남긴다.
	GAMELOG->LogAction( ACT_PK_CHANGE_CHAOSTATE, m_pPlayer, NULL, GetChaoState(), GetChaoEndTime() );
}

VOID ChaoState::StartBoradoriState( Player* pTarget )
{
    S_TRY
    {
        if (NULL == pTarget)
            S_THROW0();

        const DWORD playerKey = m_pPlayer->GetObjectKey();
        const DWORD TargetKey = pTarget->GetObjectKey();

        if (playerKey == TargetKey)	
            S_THROW0();

        if (eCHAO_STATE_CHAO == GetChaoState())
            S_THROW0();

        ChaoState& rTargetChaoState = pTarget->GetChaoState();

    #if !defined(_CHINA) //CHINA_PK_RULE - 중국 버전은 사용하지 않는다.
        if (eCHAO_STATE_PREV_CHAO == GetChaoState())
            S_THROW0();

        // 상대가 카오, 프리카오 상태라면 나는 보라돌이가 되지 않는다.
        if (eCHAO_STATE_NORMAL != rTargetChaoState.GetChaoState())
            S_THROW0();

        //상대가 보라돌이면 정당방위이므로 보라돌이가 되지 않는다.
        if( rTargetChaoState.IsBoradori() )	
            S_THROW0();
    #endif

        // _NA001385_20090924_DOMINATION_ETC :PTA_DOMINATION_WAR_TILE 추가
        const DWORD dwTile = 
            PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE | PTA_PK_TILE | PTA_DOMINATION_WAR_TILE;

        if (FALSE == m_pPlayer->IsPvPAtt(dwTile))    // 마을은 예외로 타일이 존재하지 않는다. 마을에서는 보라돌이가 되지 않는다.
            S_THROW0();

        if (m_pPlayer->IsPvPAtt(PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE) ||  // 분쟁지역에서는 보라돌이가 되지 않는다.
            HostilityRelationCheck(pTarget) ||                                  // 적대 관계면 보라돌이를 만들지 않는다. 
            m_pPlayer->GetPartyState() == pTarget->GetPartyState())             // 같은 파티원이면 보라돌이가 되지 않는다.
        {
            S_THROW0();
        }

        // (CHANGES) (WAVERIX) ...
        const BASE_PLAYERINFO* player_info = m_pPlayer->GetCharInfo();
        if(player_info->m_GuildGuid) {
            // 같은 길드원이면 보라돌이가 되지 않는다.
            const BASE_PLAYERINFO* target_info = pTarget->GetCharInfo();
            if(player_info->m_GuildGuid == target_info->m_GuildGuid)
                S_THROW0();
        }
    }
    S_CATCH0()
    {
        return;
    }

    ChaoState& rTargetChaoState = pTarget->GetChaoState();
    const PKOptionInfo& stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();
    DWORD BoradoriAttackTime = stPKOptionInfo.m_BoradoriNormalTime;

	if (eCHAO_STATE_NORMAL == rTargetChaoState.GetChaoState() &&    // 보라돌이도 아니고 카오도 아닌 유저를 공격하면 보라돌이 시간이 크다.
        FALSE == rTargetChaoState.IsBoradori() )
    {
		BoradoriAttackTime = stPKOptionInfo.m_BoradoriAttackTime;
    }

    m_BoradoriTimer.SetTimer(BoradoriAttackTime);

	if (FALSE == m_bBoradori)
	{
		MSG_CG_CHAO_CHANGE_BORADORI_STATE_BRD cmdMsg;           	// 최초로 보라돌이가 걸릴 때만 패킷을 보낸다.
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_bBoradori = true;
		m_pPlayer->SendPacketAround( &cmdMsg, sizeof(cmdMsg) );
	}

	m_bBoradori = TRUE;                                             // 보라돌이 셋팅
}

VOID ChaoState::EndBoradoriState()
{
	if( !m_bBoradori )	return;

	// 보라돌이 해제
	m_bBoradori = FALSE;
	m_BoradoriTimer.Clear();

	MSG_CG_CHAO_CHANGE_BORADORI_STATE_BRD cmdMsg;
	cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
	cmdMsg.m_bBoradori = false;
	m_pPlayer->SendPacketAround( &cmdMsg, sizeof(cmdMsg) );
}

BOOL ChaoState::FindBadFriend( DWORD dwCharGuid )
{
	HASHMAP_BADFRIEND::const_iterator it = m_mapBadFriend.find( dwCharGuid );
	if( it != m_mapBadFriend.end() )
		return TRUE;

	return FALSE;
}

BOOL ChaoState::AddBadFriend( STRUCT_REVENGE_INFO & RevengeInfo )
{
	if( FindBadFriend( RevengeInfo.dwRevengeCharGuid ) )
	{
		return FALSE;
	}
	else
	{
        m_mapBadFriend.insert(HASHMAP_BADFRIEND::value_type(RevengeInfo.dwRevengeCharGuid, RevengeInfo));
		return TRUE;
	}
}

VOID ChaoState::RequestRemoveBadFriend( DWORD dwCharGuid )
{
	if( FindBadFriend( dwCharGuid ) )
	{
		// 월드서버에 리벤지 대상삭제를 요청한다.(G->A->W)
		MSG_AG_CHAO_REMOVE_REVENGE_CHAR_SYN synMsg;
		synMsg.m_dwRevengeCharGuid = dwCharGuid;
		m_pPlayer->SendPacket( &synMsg, sizeof(synMsg) );

		// PK 유저가 월드서버와의 연결이 끊어졌을 경우 무한 복수를 할 수 있으므로, 먼저 리스트에서 지우고 월드에 요청한다.
		RemoveBadFriend( dwCharGuid );
	}
}

BOOL ChaoState::RemoveBadFriend( DWORD dwCharGuid )
{
	if( FindBadFriend( dwCharGuid ) )
	{
		m_mapBadFriend.erase( dwCharGuid );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID ChaoState::SetRevengeName( const TCHAR * ptszRevengeName )
{
	_tcsncpy( m_ptszRevengeName, ptszRevengeName, MAX_CHARNAME_LENGTH );
	m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
}

BOOL ChaoState::IsRevengeName( const TCHAR * ptszRevengeName )
{
	if( _tcscmp( m_ptszRevengeName, ptszRevengeName ) == 0 )
		return TRUE;
	else
		return FALSE;
}

RC::eREVENGE_RESULT ChaoState::TrackRevenge( const TCHAR * ptszRevengeName, POSTYPE atPos )
{
	// 대상이 리벤지 목록에 있는지 체크
    HASHMAP_BADFRIEND::const_iterator it = m_mapBadFriend.begin();
	for ( ; it != m_mapBadFriend.end(); ++it )
	{
		const STRUCT_REVENGE_INFO &RevengeInfo = it->second;

		if( _tcscmp( RevengeInfo.sPacketInfo.ptszRevengeName, ptszRevengeName ) == 0 )
			break;
	}
	if( it == m_mapBadFriend.end() )
		return RC::RC_REVENGE_INVALID_CHAR;

	// 복수의 돌이 있는지 체크
	if( !m_pPlayer->GetItemManager()->ValidState() ) 
		return RC::RC_REVENGE_CHAR_INVALID_STATE;

	if( !m_pPlayer->GetItemManager()->ValidPos( SI_INVENTORY, atPos, TRUE ) )
		return RC::RC_REVENGE_REVENGEITEM_NOT_EXSITS;

	SCSlotContainer * pContainer = m_pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
	SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot( atPos );
	if( rSlot.GetItemInfo()->m_byWasteType != eITEMWASTE_REVENGESTONE )
		return RC::RC_REVENGE_REVENGEITEM_NOT_EXSITS;

	// Agent에 추적명령을 보낸다.
	MSG_AG_GM_TRACKING_CMD AGMsg;
	_tcsncpy( AGMsg.m_szCharName, ptszRevengeName, MAX_CHARNAME_LENGTH );
	AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
	m_pPlayer->GetPos( &AGMsg.m_vCurPos );
	AGMsg.m_PortalType = (BYTE)ePORTAL_TYPE_REVENGE;
	AGMsg.m_atItemPos = atPos;
	m_pPlayer->SendPacket( &AGMsg, sizeof(AGMsg) );

	return RC::RC_REVENGE_SUCCESS;
}

BOOL ChaoState::DeleteRevengeStone( POSTYPE atPos )
{
	if( !m_pPlayer->GetItemManager()->ValidPos( SI_INVENTORY, atPos, TRUE ) )
	{
		SUNLOG( eCRITICAL_LOG, "[DeleteRevengeStone] Can't Delete RevengeStone for InValidPos" );
		return FALSE;
	}

	SCSlotContainer * pContainer = m_pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
	SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot( atPos );
	if( rSlot.GetItemInfo()->m_byWasteType != eITEMWASTE_REVENGESTONE )
	{
		SUNLOG( eCRITICAL_LOG, "[DeleteRevengeStone] it isn't revengestone" );
		return FALSE;
	}

	// 복수의 돌 아이템을 삭제한다.
	if( RC::RC_ITEM_SUCCESS != m_pPlayer->GetItemManager()->Delete( SI_INVENTORY, atPos, 1, ITEM_DELETE_REVENGE_PORTAL ) )
	{
		SUNLOG( eCRITICAL_LOG, "[DeleteRevengeStone] Cannot Delete revengestone" );
		return FALSE;
	}

	return TRUE;
}

eCHAO_STATE	ChaoState::GetChaoState()	
{ 
	return (eCHAO_STATE)m_pPlayer->GetCharInfo()->m_ChaoState;
}

VOID ChaoState::SetChaoState( eCHAO_STATE eChaoState )	
{
	m_pPlayer->GetCharInfo()->m_ChaoState = eChaoState;
}

VOID ChaoState::ClearChaoState()
{
	if( GetChaoState() == eCHAO_STATE_CHAO )
	{
		SetChaoState( eCHAO_STATE_PREV_CHAO );
		DownChaoState();
	}
	else if( GetChaoState() == eCHAO_STATE_PREV_CHAO )
	{
		DownChaoState();
	}
}

time_t ChaoState::GetChaoEndTime()
{ 
	return static_cast<time_t>(m_pPlayer->GetCharInfo()->m_ChaoEndTime);
}

VOID ChaoState::SetChaoEndTime( time_t EndTime )
{ 
	m_pPlayer->GetCharInfo()->m_ChaoEndTime = static_cast<INT64>(EndTime);

	// 남은 시간을 클라에 알려준다.
	SendLeavedChaoTime();
}

VOID ChaoState::SendLeavedChaoTime()
{
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	MSG_CG_CHAO_NOTIFIY_CHAO_TIME_CMD cmdMsg;
	cmdMsg.m_dwLeavedTime = 0;
	if( GetChaoEndTime() )
	{
		time_t cur_time;	util::TimeSync::time( &cur_time );
		if( GetChaoEndTime() >= cur_time )
			cmdMsg.m_dwLeavedTime = GetChaoEndTime() - cur_time;

		// 프리카오 시간은 빼고 보내준다.
		if( cmdMsg.m_dwLeavedTime > stPKOptionInfo.m_PreChaoTime )
			cmdMsg.m_dwLeavedTime -= stPKOptionInfo.m_PreChaoTime;
	}

	m_pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );
}

VOID ChaoState::DecreaseChaoTime( WORD wDecSeconds )
{
	if( GetChaoState() == eCHAO_STATE_NORMAL )
		return;

	if( GetChaoEndTime() == 0 || wDecSeconds == 0 )
		return;

	SetChaoEndTime( GetChaoEndTime() - wDecSeconds );
}

VOID ChaoState::KillMonster( NPC *pNPC )
{
	if( !pNPC )	return;

	int iLevelDiff = pNPC->GetDisplayLevel() - m_pPlayer->GetLevel();
	WORD wDecSeconds = 0;

	if( iLevelDiff >= 6 )			wDecSeconds = 14;
	else if( iLevelDiff >= 1 )		wDecSeconds = 8;
	else if( iLevelDiff >= -5 )		wDecSeconds = 5;

	// 카오시간을 감소시킨다.
	DecreaseChaoTime( wDecSeconds );
}

BOOL ChaoState::CanUseFuction( DWORD Flag )
{
	if( m_pPlayer->GetCharInfo()->m_ChaoState == eCHAO_STATE_NORMAL )
		return TRUE;
	else if( m_pPlayer->GetCharInfo()->m_ChaoState == eCHAO_STATE_PREV_CHAO )
		return PRE_CHAO_USE_FLAG & Flag;
	else if( m_pPlayer->GetCharInfo()->m_ChaoState == eCHAO_STATE_CHAO )
		return FUL_CHAO_USE_FLAG & Flag;

	return FALSE;
}

bool ChaoState::HostilityRelationCheck(Player *pTarget)
{
	if( !m_pPlayer || !pTarget ) return false;

	GUILDGUID AttkGld = m_pPlayer->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;
	GUILDGUID DefGld  = pTarget->GetPlayerOwner()->GetCharInfo()->m_GuildGuid;

	if( AttkGld == 0 || DefGld == 0 || AttkGld == DefGld ) return false;

	return GuildSmallContentEntry::RelationSystem::IsHostilityRelation(AttkGld, DefGld);
}