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

	// ������ �����ð��� �ٵǸ� �����ȴ�.
	if( m_BoradoriTimer.IsExpired() )
	{
		EndBoradoriState();
	}

	// �����ð��� ������ ī�����°� �����ȴ�.
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

		// 1�ð� �������� ���� ī���ð��� �˷��ش�.
		if( m_NotifyTimer.IsExpired() )
		{
			SendLeavedChaoTime();
		}
	}

	// ���弭������ ������ ������ ��� ������ ����Ʈ �ʱ�ȭ!
	if( !g_pGameServer->IsConnectToWorldServer() )
	{
		m_mapBadFriend.clear();
	}
}

VOID ChaoState::OnDead()
{
	// ������ ���¸� �����Ѵ�.
	EndBoradoriState();
}

VOID ChaoState::UpChaoState()
{
	// GM �ʿ����� ī�������� �ø��� �ʴ´�.
	GameField *pField = m_pPlayer->GetField();
	if( pField )
	{
		if( HardCode::MAPCODE_for_GM == pField->GetFieldCode() )
			return;
	}

	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	BOOL bChangeState = FALSE;
	time_t cur_time;	util::TimeSync::time( &cur_time );

	// ī�� ����, ������ �ð� ����
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
		// ī�����¿��� �Ǵٽ� ��븦 ���̸� �ð��� �����ȴ�.
		SetChaoEndTime( GetChaoEndTime() + stPKOptionInfo.m_AddChaoTime );
	}

	// ī������ ������ Ŭ�� �˷��ش�.
	if( bChangeState )
	{
		// ����
		MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD cmdMsg;
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		// �ֺ�
		MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD brdMsg;
		brdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		brdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );

		// ���
		if( m_pPlayer->GetCharInfo()->m_GuildGuid )
		{
			MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN guildMsg;
			guildMsg.m_CharGuid = m_pPlayer->GetCharGuid();
			guildMsg.m_GuildGuid = m_pPlayer->GetCharInfo()->m_GuildGuid;
			guildMsg.m_byChaoState = GetChaoState();
			m_pPlayer->SendToGuildServer( &guildMsg, sizeof(guildMsg) );
		}
	}

	// ī������ ���� �α׸� �����.
	GAMELOG->LogAction( ACT_PK_CHANGE_CHAOSTATE, m_pPlayer, NULL, GetChaoState(), GetChaoEndTime() );
}

VOID ChaoState::DownChaoState()
{
	PKOptionInfo & stPKOptionInfo = PKScriptParser::Instance()->GetPKOptionInfo();

	BOOL bChangeState = FALSE;
	time_t cur_time;	util::TimeSync::time( &cur_time );

	// ī�� ���� ����
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

		// ���������� ���� �ð��� ������ ������, Ŭ�󿡼��� ����ī���� ���� ���� �ð��� ���� �����ؾ��Ѵ�.
		SendLeavedChaoTime();
	}

	// ī������ ������ Ŭ�� �˷��ش�.
	if( bChangeState )
	{
		// ����
		MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD cmdMsg;
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacket( &cmdMsg, sizeof(cmdMsg) );

		// �ֺ�
		MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD brdMsg;
		brdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		brdMsg.m_byChaoState = GetChaoState();
		m_pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );
		// ���
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

	// ī������ ���� �α׸� �����.
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

    #if !defined(_CHINA) //CHINA_PK_RULE - �߱� ������ ������� �ʴ´�.
        if (eCHAO_STATE_PREV_CHAO == GetChaoState())
            S_THROW0();

        // ��밡 ī��, ����ī�� ���¶�� ���� �����̰� ���� �ʴ´�.
        if (eCHAO_STATE_NORMAL != rTargetChaoState.GetChaoState())
            S_THROW0();

        //��밡 �����̸� ��������̹Ƿ� �����̰� ���� �ʴ´�.
        if( rTargetChaoState.IsBoradori() )	
            S_THROW0();
    #endif

        // _NA001385_20090924_DOMINATION_ETC :PTA_DOMINATION_WAR_TILE �߰�
        const DWORD dwTile = 
            PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE | PTA_PK_TILE | PTA_DOMINATION_WAR_TILE;

        if (FALSE == m_pPlayer->IsPvPAtt(dwTile))    // ������ ���ܷ� Ÿ���� �������� �ʴ´�. ���������� �����̰� ���� �ʴ´�.
            S_THROW0();

        if (m_pPlayer->IsPvPAtt(PTA_CONFLICT_TILE | PTA_FREE_CONFLICT_TILE) ||  // �������������� �����̰� ���� �ʴ´�.
            HostilityRelationCheck(pTarget) ||                                  // ���� ����� �����̸� ������ �ʴ´�. 
            m_pPlayer->GetPartyState() == pTarget->GetPartyState())             // ���� ��Ƽ���̸� �����̰� ���� �ʴ´�.
        {
            S_THROW0();
        }

        // (CHANGES) (WAVERIX) ...
        const BASE_PLAYERINFO* player_info = m_pPlayer->GetCharInfo();
        if(player_info->m_GuildGuid) {
            // ���� �����̸� �����̰� ���� �ʴ´�.
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

	if (eCHAO_STATE_NORMAL == rTargetChaoState.GetChaoState() &&    // �����̵� �ƴϰ� ī���� �ƴ� ������ �����ϸ� ������ �ð��� ũ��.
        FALSE == rTargetChaoState.IsBoradori() )
    {
		BoradoriAttackTime = stPKOptionInfo.m_BoradoriAttackTime;
    }

    m_BoradoriTimer.SetTimer(BoradoriAttackTime);

	if (FALSE == m_bBoradori)
	{
		MSG_CG_CHAO_CHANGE_BORADORI_STATE_BRD cmdMsg;           	// ���ʷ� �����̰� �ɸ� ���� ��Ŷ�� ������.
		cmdMsg.m_dwObjKey = m_pPlayer->GetObjectKey();
		cmdMsg.m_bBoradori = true;
		m_pPlayer->SendPacketAround( &cmdMsg, sizeof(cmdMsg) );
	}

	m_bBoradori = TRUE;                                             // ������ ����
}

VOID ChaoState::EndBoradoriState()
{
	if( !m_bBoradori )	return;

	// ������ ����
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
		// ���弭���� ������ �������� ��û�Ѵ�.(G->A->W)
		MSG_AG_CHAO_REMOVE_REVENGE_CHAR_SYN synMsg;
		synMsg.m_dwRevengeCharGuid = dwCharGuid;
		m_pPlayer->SendPacket( &synMsg, sizeof(synMsg) );

		// PK ������ ���弭������ ������ �������� ��� ���� ������ �� �� �����Ƿ�, ���� ����Ʈ���� ����� ���忡 ��û�Ѵ�.
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
	// ����� ������ ��Ͽ� �ִ��� üũ
    HASHMAP_BADFRIEND::const_iterator it = m_mapBadFriend.begin();
	for ( ; it != m_mapBadFriend.end(); ++it )
	{
		const STRUCT_REVENGE_INFO &RevengeInfo = it->second;

		if( _tcscmp( RevengeInfo.sPacketInfo.ptszRevengeName, ptszRevengeName ) == 0 )
			break;
	}
	if( it == m_mapBadFriend.end() )
		return RC::RC_REVENGE_INVALID_CHAR;

	// ������ ���� �ִ��� üũ
	if( !m_pPlayer->GetItemManager()->ValidState() ) 
		return RC::RC_REVENGE_CHAR_INVALID_STATE;

	if( !m_pPlayer->GetItemManager()->ValidPos( SI_INVENTORY, atPos, TRUE ) )
		return RC::RC_REVENGE_REVENGEITEM_NOT_EXSITS;

	SCSlotContainer * pContainer = m_pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
	SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot( atPos );
	if( rSlot.GetItemInfo()->m_byWasteType != eITEMWASTE_REVENGESTONE )
		return RC::RC_REVENGE_REVENGEITEM_NOT_EXSITS;

	// Agent�� ��������� ������.
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

	// ������ �� �������� �����Ѵ�.
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

	// ���� �ð��� Ŭ�� �˷��ش�.
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

		// ����ī�� �ð��� ���� �����ش�.
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

	// ī���ð��� ���ҽ�Ų��.
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