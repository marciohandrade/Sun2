#include "Stdafx.h"

#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#include "Player.h"
#include "GameServerEx.h"
#include "RankingDefine.h"
#include <PacketStruct_DG.h>
#include <PacketStruct_GP.h>
#include <SolarDateTime.h>
#include "GameRankingManager.h"
#include "BattleRecorder.h"

#include "BattleGroundRankingManager.h"

VOID BattleGroundRankingManager::EnterPlayer(Player* player, MAPCODE map_code)
{
    //�߰�
    BattleGroundPoints point;
    point.char_guid     = player->GetCharGuid();
    point.user_guid		= player->GetUserGuid();
    point.ranking_no    = map_code;

    BATTLEGROUND_POINT_HASH::iterator it = hash_player_points_.find( player->GetUserGuid() );
    if( it != hash_player_points_.end() )
    {
        SUNLOG(eCRITICAL_LOG, "[EnterPlayer:BattlegroundRankingManager]Already exist User - shogen [%u]", player->GetUserGuid() );
        return;

    }
    hash_player_points_[point.user_guid] = point;

    // ��Ż ������ �����ִ��� üũ?
    if( !(g_pGameServer->IsConnected(RANKING_SERVER)) )
        return;

    //Portal���� ����ð� �޾ƿ���
    //�¼�, ų, ��ý�Ʈ�� ������ ��Ŷ�� �������ش�.
    RANKINGTYPE packet_count = eBATTLEGROUND_RANKING_TYPE_MAX - 1;

    MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN synmsg[eBATTLEGROUND_RANKING_TYPE_MAX - 1];
    DWORD64 RegisterTime=0;
    util::GetDateTime_YYYYMMDDHHMMSSNNN( RegisterTime );

    for(RANKINGTYPE loop = 0; loop < packet_count; ++loop)
    {
        synmsg[loop].RecordInfo.m_MissionNo		= map_code;
        synmsg[loop].RecordInfo.m_ClearTime		= 0;	
        synmsg[loop].RecordInfo.m_RegisterTime	= RegisterTime;
        // ���巩ŷ ����Ÿ���� eRANKING_TYPE_BATTLEGROUND_WIN�� ���� �ٿ����� �Է½�Ų��.
        // RankingSystemType�� �߰��� ���� ��, 
        // ���巩ŷ�� �ƴ� ��쿣 eRANKING_TYPE_BATTLEGROUND_WIN ���� ���� ������,
        // ���巩ŷ�� ��쿣 eRANKING_TYPE_BATTLEGROUND_WIN ���� ū ������ �����Ѵ�.
        synmsg[loop].RecordInfo.ranking_type = ConvertRankingType(static_cast<eBATTLEGROUND_RANKING_TYPE>(loop + (RANKINGTYPE)1));  

        synmsg[loop].MemberInfo.m_UserGuid	= player->GetUserGuid();
        synmsg[loop].MemberInfo.m_Class		= player->GetCharInfo()->m_byClassCode;
        synmsg[loop].MemberInfo.m_LV			= player->GetLevel();
        _tcsncpy( synmsg[loop].MemberInfo.m_tszCharName, player->GetCharName(), MAX_CHARNAME_LENGTH );
        synmsg[loop].MemberInfo.m_tszCharName[MAX_CHARNAME_LENGTH]= '\0';
        if( 0 != player->GetCharInfo()->m_GuildGuid )
        {
            _tcsncpy( synmsg[loop].MemberInfo.m_tszGuildName, player->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH );
            synmsg[loop].MemberInfo.m_tszGuildName[MAX_GUILDNAME_LENGTH]= '\0';
        }
        else
        {
            synmsg[loop].MemberInfo.m_tszGuildName[0]= '\0';
        }

        g_pGameServer->SendToServer( RANKING_SERVER, &synmsg[loop], sizeof(synmsg[loop]) );	
    }
}

VOID BattleGroundRankingManager::BattleGround_LeavePlayer(Player* player, MAPCODE map_code, 
                                                          const BattleRecorder::Record* record, bool is_win)
{
    BATTLEGROUND_POINT_HASH::iterator it = hash_player_points_.find( player->GetUserGuid() );
    if( it == hash_player_points_.end() )
    {
        SUNLOG(eCRITICAL_LOG, "[BattleGroundRankingManager::LeavePlayer]Not exist User - shogen [%u]", 
               player->GetUserGuid() );
        return;
    }

    if( (it->second).is_ready == FALSE )
    {
        hash_player_points_.erase( it );
        return;
    }

    // ��Ż ������ �����ִ��� üũ?
    if( !(g_pGameServer->IsConnected(RANKING_SERVER)) )
    {
        hash_player_points_.erase( it );
        return;
    }

    //�������̸� ��� ����
    if( g_GameRankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
    {
        hash_player_points_.erase( it );
        return;
    }

    //�¼�, ų, ��ý�Ʈ�� ������ ��Ŷ�� �������ش�.
    RANKINGTYPE packet_count = eBATTLEGROUND_RANKING_TYPE_MAX - 1;

    MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN portal_syn[eBATTLEGROUND_RANKING_TYPE_MAX - 1];
    DWORD64 RegisterTime=0;
    util::GetDateTime_YYYYMMDDHHMMSSNNN( RegisterTime );
    //�÷��̾��������� ������ ������ �����´�
    BattleGroundPlayerInfo& bg_player_info = player->GetBattleGroundPlayerInfo();
    BattleGroundPoints* player_points = &(it->second);
    for(RANKINGTYPE loop = 0; loop < packet_count; ++loop)
    {
        RANKINGTYPE temp_type = loop + (RANKINGTYPE)1;

        portal_syn[loop].RecordInfo.m_MissionNo		= map_code;
        portal_syn[loop].RecordInfo.ranking_type    = ConvertRankingType(static_cast<eBATTLEGROUND_RANKING_TYPE>(temp_type));
        portal_syn[loop].RecordInfo.m_RegisterTime  = RegisterTime;

        switch(static_cast<eBATTLEGROUND_RANKING_TYPE>(temp_type))
        {
        case eBATTLEGROUND_RANKING_TYPE_WIN:
            if (is_win)
            {
                portal_syn[loop].RecordInfo.m_ClearTime	= player_points->win_point+1;
            }
            else
            {
                 portal_syn[loop].RecordInfo.m_ClearTime = player_points->win_point;
            }
            portal_syn[loop].DBKey= player_points->win_db_key;
            break;
        case eBATTLEGROUND_RANKING_TYPE_KILL:
            portal_syn[loop].RecordInfo.m_ClearTime	= player_points->kill_point + record->player_kill_count;
            portal_syn[loop].DBKey= player_points->kill_db_key;
            break;
        case eBATTLEGROUND_RANKING_TYPE_ASSIST:
            portal_syn[loop].RecordInfo.m_ClearTime	= player_points->assist_point + record->assist_count;
            portal_syn[loop].DBKey= player_points->assist_db_key;
            break;
        }

        portal_syn[loop].MemberInfo.m_UserGuid		= player->GetUserGuid();
        portal_syn[loop].MemberInfo.m_Class		= player->GetCharInfo()->m_byClassCode;
        portal_syn[loop].MemberInfo.m_LV			= player->GetLevel();
        _tcsncpy( portal_syn[loop].MemberInfo.m_tszCharName, player->GetCharName(), MAX_CHARNAME_LENGTH );
        portal_syn[loop].MemberInfo.m_tszCharName[MAX_CHARNAME_LENGTH]= '\0';
        if( 0 != player->GetCharInfo()->m_GuildGuid )
        {
            _tcsncpy( portal_syn[loop].MemberInfo.m_tszGuildName, player->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH );
            portal_syn[loop].MemberInfo.m_tszGuildName[MAX_GUILDNAME_LENGTH]= '\0';
        }
        else
        {
            portal_syn[loop].MemberInfo.m_tszGuildName[0]= '\0';
        }

        g_pGameServer->SendToServer( RANKING_SERVER, &portal_syn[loop], sizeof(portal_syn[loop]) );
    }

}

RANKINGTYPE BattleGroundRankingManager::ConvertRankingType( eBATTLEGROUND_RANKING_TYPE temp_type )
{
    switch(temp_type)
    {
    case eBATTLEGROUND_RANKING_TYPE_WIN: return eRANKING_TYPE_BATTLEGROUND_WIN;
    case eBATTLEGROUND_RANKING_TYPE_KILL: return eRANKING_TYPE_BATTLEGROUND_KILL;
    case eBATTLEGROUND_RANKING_TYPE_ASSIST: return eRANKING_TYPE_BATTLEGROUND_ASSIST;
    default:
        return 0;
    }
    return 0;
}

VOID BattleGroundRankingManager::SetPlayerPoint( USERGUID user_guid, DWORD point, RANKINGKEY db_key, 
                                                 RANKINGTYPE ranking_type )
{
    BATTLEGROUND_POINT_HASH::iterator it = hash_player_points_.find( user_guid );
    if( it == hash_player_points_.end() )
    {
        SUNLOG(eCRITICAL_LOG, "[BattleGroundRankingManager::SetPlayerPoint]Not exist User [%u]", 
               user_guid );
        return;
    }

    switch(ranking_type)
    {
    case eRANKING_TYPE_BATTLEGROUND_WIN:
        hash_player_points_[user_guid].win_db_key = db_key;
        hash_player_points_[user_guid].win_point = point;
        hash_player_points_[user_guid].is_ready = TRUE;
        break;
    case eRANKING_TYPE_BATTLEGROUND_KILL:
        hash_player_points_[user_guid].kill_db_key = db_key;
        hash_player_points_[user_guid].kill_point = point;
        hash_player_points_[user_guid].is_ready = TRUE;
        break;
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        hash_player_points_[user_guid].assist_db_key = db_key;
        hash_player_points_[user_guid].assist_point = point;
        hash_player_points_[user_guid].is_ready = TRUE;
        break;
    }
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif // _NA_008012_20150130_RANKING_SYSTEM