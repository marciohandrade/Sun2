#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemPacketParser.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/uiGuildIntro.h"
#include "GuildSystem/uiGuildWareHouse.h"
#include "InterfaceManager.h"
#include "GameFramework.h"
#include "SceneBase.h"

namespace GuildSystem{

namespace PacketParser
{
    void ParsePacketGuildSystem(MSG_BASE* recv_packet)
    {
        if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT) ||
            (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE))
        {
            return;
        }

        switch (recv_packet->m_byCategory)
        {
        case CG_GUILD:
            {
                switch (recv_packet->m_byProtocol)
                {
                case CG_GUILD_CREATE_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CREATE_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_CREATE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CREATE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_CREATE_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CREATE_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_LOGIN_CHAR_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_LOGIN_CHAR_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_LOGIN_CHAR_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_LOGIN_CHAR_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_LOGOUT_CHAR_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_LOGOUT_CHAR_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_CHAR_LEVELUP_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHAR_LEVELUP_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_FAILED_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_FAILED_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_INVITE_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_INVITE_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_INVITE_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_INVITE_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_INVITE_ANSWER_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_INVITE_ANSWER_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_INVITE_ANSWER_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_INVITE_ANSWER_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_WITHDRAW_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_WITHDRAW_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_WITHDRAW_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_WITHDRAW_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_WITHDRAW_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_WITHDRAW_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_KICKPLAYER_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_KICKPLAYER_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_KICKPLAYER_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_KICKPLAYER_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_KICKPLAYER_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_KICKPLAYER_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_LEAVE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_LEAVE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_CHANGE_DUTY_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHANGE_DUTY_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_CHANGE_DUTY_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHANGE_DUTY_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_RANKUP_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RANKUP_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_ADD_INFO_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_ADD_INFO_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_ADD_INFO_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_ADD_INFO_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_REQUEST_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_REQUEST_CMD(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_REJECT_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_REJECT_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_DELETE_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_DELETE_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_DELETE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_DELETE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_BOTH_AGAINST_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_BOTH_AGAINST_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_RELATION_MOVE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RELATION_MOVE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_TOTALINFO_REQ_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_TOTALINFO_REQ_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_TOTALINFO_REQ_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_TOTALINFO_REQ_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_CHANGE_CHAOSTATE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHANGE_CHAOSTATE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_REGISTER_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_REGISTER_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_REGISTER_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_REGISTER_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_REGISTER_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_REGISTER_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_DELETE_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_DELETE_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_DELETE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_DELETE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_GUILDMARK_DELETE_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_GUILDMARK_DELETE_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_CHANGE_MASTER_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHANGE_MASTER_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_CHANGE_MASTER_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_CHANGE_MASTER_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_NAME_CHANGE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_NAME_CHANGE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_NAME_CHANGE_SECTOR_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_NAME_CHANGE_SECTOR_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_NAME_CHANGE_RELATION_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_NAME_CHANGE_RELATION_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_NAME_CHANGE_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_NAME_CHANGE_NAK(recv_packet);
                    }
                    break;

                case CG_GUILD_WAREHOUSE_OPEN_ACK:
                case CG_GUILD_WAREHOUSE_OPEN_NAK:
                case CG_GUILD_WAREHOUSE_CLOSE_ACK:
                case CG_GUILD_WAREHOUSE_CLOSE_NAK:
                case CG_GUILD_WAREHOUSE_MONEY_ACK:
                case CG_GUILD_WAREHOUSE_MONEY_NAK:
                    {
                        uiGuildWareHouse* guild_warehouse = 
                            GET_CAST_DIALOG(uiGuildWareHouse, uiGuildWareHouse::kDialog_ID);
                        if (guild_warehouse != NULL)
                        {
                            guild_warehouse->NetworkProc(recv_packet);
                        }
                    }
                    break;
                case CG_GUILD_DESTROY_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_DESTROY_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_DESTROY_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_DESTROY_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_DESTROY_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_DESTROY_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_DESTROY_WAIT_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_DESTROY_WAIT_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_DESTROY_WAIT_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_DESTROY_WAIT_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_RECOVERY_GUILD_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RECOVERY_GUILD_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_RECOVERY_GUILD_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_RECOVERY_GUILD_NAK(recv_packet);
                    }
                    break;
                case CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS(recv_packet);
                    }
                    break;
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
                case CG_GUILD_INTRODUCTION_SAVE_ACK:
                case CG_GUILD_INTRODUCTION_CHANGE_BRD:
                    {
                        uiBase* ui_guild_intro = GET_DIALOG(uiGuildIntro::kDialog_ID);
                        if (ui_guild_intro != NULL)
                        {
                            ui_guild_intro->NetworkProc(recv_packet);
                        }
                    }
                    break;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                case CG_GUILD_JOIN_REQUEST_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_REQUEST_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_REQUEST_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_REQUEST_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_REQUEST_REJECT_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_REQUEST_REJECT_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_JOIN_REQUEST_REJECT_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_JOIN_REQUEST_REJECT_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_FACILITY_TOTALINFO_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_FACILITY_TOTALINFO_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_FACILITY_UPGRADE_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_FACILITY_UPGRADE_ACK(recv_packet);
                    }
                    break;
                case CG_GUILD_FACILITY_UPGRADE_BRD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_FACILITY_UPGRADE_BRD(recv_packet);
                    }
                    break;
                case CG_GUILD_FACILITY_ACTIVE_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_FACILITY_ACTIVE_ACK(recv_packet);
                    }
                    break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_008334_20150608_SONNENSCHEIN
                case CG_GUILD_SONNENSCHEIN_UPDATE_CMD:
                    {
                        GuildSystem::PacketFunction::RECV_CG_GUILD_SONNENSCHEIN_UPDATE_CMD(recv_packet);
                    }
                    break;
#endif //_NA_008334_20150608_SONNENSCHEIN
                }                
            }
            break;
        case CG_ETC:
            {
                switch (recv_packet->m_byProtocol)
                {
                case CG_ETC_GUILD_RANKINGINFO_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_ETC_GUILD_RANKINGINFO_ACK(recv_packet);
                    }
                    break;
                case CG_ETC_GUILDINFO_ACK:
                    {
                        GuildSystem::PacketFunction::RECV_CG_CG_ETC_GUILDINFO_ACK(recv_packet);
                    }
                    break;
                }
            }
        }
    }

    void ParsePacketGuildSystemForWorld(MSG_BASE* recv_packet)
    {
        if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT) ||
            (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE))
        {
            return;
        }

        switch (recv_packet->m_byCategory)
        {
        case CW_GUILD:
            {
                switch (recv_packet->m_byProtocol)
                {
                case CW_GUILD_CHAT_BRD:
                case CW_GUILD_CHAT_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CW_GUILD_CHAT_PACKET(recv_packet);
                    }
                    break;
                case CW_GUILD_SELECT_NOTICE_ACK:
                case CW_GUILD_SELECT_NOTICE_NAK:
                case CW_GUILD_NOTICE_CMD:
                case CW_GUILD_CHANGE_NOTICE_BRD:
                case CW_GUILD_CHANGE_NOTICE_NAK:
                    {
                        uiBase* ui_guild_intro = GET_DIALOG(uiGuildIntro::kDialog_ID);
                        if (ui_guild_intro != NULL)
                        {
                            ui_guild_intro->NetworkProc(recv_packet);
                        }
                    }
                    break;
                case CW_GUILD_WAREHOUSE_LOGLIST_ANS:
                    {
                        GuildSystem::PacketFunction::RECV_CW_GUILD_WAREHOUSE_LOGLIST_ANS(recv_packet);
                    }
                    break;
                case CW_GUILD_WAREHOUSE_LOGLIST_NAK:
                    {
                        GuildSystem::PacketFunction::RECV_CW_GUILD_WAREHOUSE_LOGLIST_NAK(recv_packet);
                    }
                    break;
                }
            }
        }
    }

}; //namespace Function
}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL