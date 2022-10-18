#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include <PacketStruct_GZ.h>
//
#include "GameGuild.h"
#include "GameGuildManager.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

static bool ProcessGuildPoint(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    int nArgSize = (int)vecCmd.size();
    if (nArgSize != 3)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    if (pPlayer->GetCharInfo()->m_GuildGuid == 0)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    MSG_GZ_GUILD_GM_GUILD_POINT_SYN synMsg;
    synMsg.m_GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
    synMsg.m_CharGuid = pPlayer->GetCharGuid();
    synMsg.m_UP = atoi(vecCmd[1].c_str());
    synMsg.m_GP = atoi(vecCmd[2].c_str());
    if (!pPlayer->SendToGuildServer(&synMsg, sizeof(synMsg)))
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    return true;

}

// ��� ���� [��]
static bool ProcessGuildDestroy(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer) { return false; }

    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 != nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    RC::eGUILD_RESULT rt = g_GameGuildManager.CanDestroyGuild(pPlayer);
    CHKSendErr(rt != RC::RC_GUILD_SUCCESS, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_USER_STATE_INVALID, pPlayer);


    MSG_GZ_GUILD_DESTROY_SYN synMsg;
    synMsg.m_GuildGuid       = pPlayer->GetCharInfo()->m_GuildGuid;
    synMsg.m_MasterCharGuid  = pPlayer->GetCharGuid();
    synMsg.m_DestroyWaitTime = atoi(vecCmd[1].c_str());

    pPlayer->SendToGuildServer(&synMsg, sizeof(synMsg));

    return true;
}

//_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
static bool ProcessGuildWarehouse(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }
    size_t num_arguments = gm_cmd_arguments.size();
    CHKSendErr(num_arguments < 2, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    // �� ��° ���ڴ� �� ��ɾ�
    const TCHAR* sub_command = gm_cmd_arguments[1].c_str();

    // �ݱ�/close ���: �ļ� ���� - ����
    // GM ��ɿ� ���� ��� â�� ���� �ݱ�
    if (_strnicmp(sub_command, "close", 5) == 0 || strncmp(sub_command, "�ݱ�", 4) == 0)
    {
        CHKSendErr(num_arguments < 3, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

        MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD cmd;
        cmd.gm_player_key = player_key;
        // �� ��° ���ڴ� ����
        strncpy(cmd.guild_name, gm_cmd_arguments[2].c_str(), MAX_GUILDNAME_LENGTH);
        cmd.guild_name[MAX_GUILDNAME_LENGTH] = '\0';

        player->SendToGuildServer(&cmd, sizeof(cmd));
    }
    else
    {
        _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
        return false;
    }

    return true;
}

static bool ProcessGuildPenalty(DWORD player_key, const GmCmdTokens& gm_cmd_arguments)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* const player = gm_manager->GetGMPlayer(player_key);
    if (player == NULL) {
        return false;
    }

    size_t num_arguments = gm_cmd_arguments.size();
    CHKSendErr(num_arguments < 2, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

    // �� ��° ���ڴ� �� ��ɾ�
    const TCHAR* sub_command = gm_cmd_arguments[1].c_str();

    // ����/clear ���: �ļ� ���� - ����
    // GM ��ɿ� ���� ��� �г�Ƽ ����
    if (_strnicmp(sub_command, "clear", 5) == 0 || strncmp(sub_command, "����", 4) == 0)
    {
        CHKSendErr(num_arguments < 3, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);

        GameGuild* pGuild = NULL;
        pGuild = g_GameGuildManager.FindGuildName(gm_cmd_arguments[2].c_str());
        if (pGuild == NULL) {
            _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
            return false;
        }

        CTime current_time = util::TimeSync::_time64(NULL);
        CTime penalty_time = current_time + CTimeSpan(0/*penalty_days*/, 0, 0, 0); 

        MSG_GZ_DOMINATION_PENALTY_CMD msg;
        msg.guild_guid = pGuild->GetGuildGuid();
        msg.map_code = 0; // GM mode
        msg.member_count = 0; // GM mode
        msg.penalty_time = penalty_time;
        g_pGameServer->SendToServer(GUILD_SERVER, &msg, sizeof(msg));
    }
    else
    {
        _SendErr(MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, player);
        return false;
    }

    return true;
}

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterGuild(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "�������Ʈ", "guildpoint", &ProcessGuildPoint),
        GMHANDLER_MAPPING(true, "������", "guilddel", &ProcessGuildDestroy), // ��� ���� ��û
        GMHANDLER_MAPPING(true, "���â��", "guildwarehouse", &ProcessGuildWarehouse),
        GMHANDLER_MAPPING(true, "����г�Ƽ", "guildpenalty", &ProcessGuildPenalty),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
