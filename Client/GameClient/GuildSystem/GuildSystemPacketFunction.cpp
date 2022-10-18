#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemPacketParser.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildCreate.h"
#include "GuildSystem/uiGuildMarkCombination.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildLog.h"
#include "GuildSystem/uiGuildInfoOther.h"
#include "GuildSystem/uiGuildRanking.h"
#include "GuildInfoParser.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiConvenienceStore/uiConvenienceStore_FriendList.h"
#include "uiChatMan/uiChatMan_def.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "GlobalFunc.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "uiGuildIntro.h"
#include "ResultCode.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

namespace GuildSystem{

namespace PacketFunction
{
    // send packet function
    void SEND_CG_GUILD_CREATE_SYN( const TCHAR* guild_name )
    {
        MSG_CG_GUILD_CREATE_SYN send_packet;
        ZeroMemory(send_packet.m_tszGuildName, sizeof(send_packet.m_tszGuildName));
        StrnCopy(send_packet.m_tszGuildName, guild_name, MAX_GUILDNAME_LENGTH);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_DESTROY_SYN()
    {
        MSG_CG_GUILD_DESTROY_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_RECOVERY_GUILD_SYN()
    {
        MSG_CG_GUILD_RECOVERY_GUILD_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_GUILDMARK_REGISTER_SYN(DWORD background_index, 
                                              DWORD pattern_index, 
                                              POSTYPE slot_position)
    {
        MSG_CG_GUILD_GUILDMARK_REGISTER_SYN send_packet;
        send_packet.m_GuildIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid;
        send_packet.m_BackGroundIndex = background_index;
        send_packet.m_PatternIndex = pattern_index;
        send_packet.m_SlotIndex = SI_INVENTORY;
        send_packet.m_SlotPos = slot_position;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_GUILDMARK_DELETE_SYN()
    {
        MSG_CG_GUILD_GUILDMARK_DELETE_SYN send_packet;
        send_packet.m_GuildIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid;
        send_packet.m_BackGroundIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_BackGroundIdx;
        send_packet.m_PatternIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_PatternIdx;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_TOTALINFO_REQ_SYN()
    {
        if (GuildSystem::GetGuildSystemData().is_recv_total_info() == false)
        {
            MSG_CG_GUILD_TOTALINFO_REQ_SYN send_packet;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        else if (GuildSystem::Function::GetGuildDestroyFlag() == eGUILD_DELCHK_DESTROY_WAIT)
        {
            //6236	길드 해산 대기 중입니다.
            GlobalFunc::VerifyMessage(6236);
            return;
        }
    }

    void SEND_CG_GUILD_RELATION_SYN( eGUILD_RELATION relation_type, const TCHAR* guild_name )
    {
        MSG_CG_GUILD_RELATION_SYN send_packet;
        send_packet.m_eType = relation_type;
        StrnCopy(send_packet.m_szRelationGuildName, guild_name, MAX_CHARNAME_LENGTH);
        send_packet.m_szRelationGuildName[_countof(send_packet.m_szRelationGuildName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_RELATION_DELETE_SYN( eGUILD_RELATION relation_type, const TCHAR* guild_name )
    {
        MSG_CG_GUILD_RELATION_DELETE_SYN send_packet;
        send_packet.m_eType = relation_type;
        send_packet.m_eDeleteGuildGuid = GuildSystem::GetGuildSystemData().GetGuildGUID(guild_name);
        StrnCopy(send_packet.m_szGuildName, guild_name, MAX_CHARNAME_LENGTH);
        send_packet.m_szGuildName[_countof(send_packet.m_szGuildName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_RELATION_BOTH_AGAINST_SYN( const TCHAR* guild_name )
    {
        MSG_CG_GUILD_RELATION_BOTH_AGAINST_SYN send_packet;
        send_packet.m_BothAgainstGuildGuid = GuildSystem::GetGuildSystemData().GetGuildGUID(guild_name);
        StrnCopy(send_packet.m_szRelationGuildName, guild_name, MAX_CHARNAME_LENGTH);
        send_packet.m_szRelationGuildName[_countof(send_packet.m_szRelationGuildName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_RELATION_ANSWER_SYN( eGUILD_ANSWER anser_type, eGUILD_RELATION relation_type, CHARGUID request_player_key)
    {
        MSG_CG_GUILD_RELATION_ANSWER_SYN send_packet;
        send_packet.m_eAnswerType = anser_type;
        send_packet.m_eRelationType = relation_type;
        send_packet.m_RequestUserKey = request_player_key;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_INVITE_SYN( const TCHAR* invite_name )
    {
        if (invite_name == NULL)
        {
            return;
        }

        if (_tcslen(invite_name) < 1)
        {
            return;
        }

        MSG_CG_GUILD_INVITE_SYN send_packet;
        send_packet.m_TargetCharName;
        StrnCopy(send_packet.m_TargetCharName, invite_name, MAX_CHARNAME_LENGTH);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_INVITE_ANSWER_SYN( bool invite_answer, CHARGUID request_player_key )
    {
        MSG_CG_GUILD_INVITE_ANSWER_SYN send_packet;
        send_packet.m_bInveite = (invite_answer == true) ? TRUE : FALSE;
        send_packet.m_OperUserKey = request_player_key;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_KICKPLAYER_SYN()
    {
        uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
        if (ui_guild_member == NULL)
        {
            return;
        }

        GUILD_MEMBER_INFO* kick_member_info = ui_guild_member->GetSelectedMemberInfo();
        if (kick_member_info == NULL)
        {
            return;
        }

        MSG_CG_GUILD_KICKPLAYER_SYN send_packet;
        StrnCopy(send_packet.m_ptszKickCharName, kick_member_info->m_tszCharName, MAX_CHARNAME_LENGTH);
        send_packet.m_ptszKickCharName[_countof(send_packet.m_ptszKickCharName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_WITHDRAW_SYN()
    {
        MSG_CG_GUILD_WITHDRAW_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_CHANGE_MASTER_SYN()
    {
        uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
        if (ui_guild_member == NULL)
        {
            return;
        }

        GUILD_MEMBER_INFO* member_info = ui_guild_member->GetSelectedMemberInfo();
        if (member_info == NULL)
        {
            return;
        }

        MSG_CG_GUILD_CHANGE_MASTER_SYN send_packet;
        StrnCopy(send_packet.m_szTargetCharName, member_info->m_tszCharName, MAX_CHARNAME_LENGTH);
        send_packet.m_szTargetCharName[_countof(send_packet.m_szTargetCharName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }


    void SEND_CG_GUILD_CHANGE_DUTY_SYN( GUILD_MEMBER_INFO* member_info, BYTE change_duty )
    {
        if (member_info == NULL)
        {
            return;
        }

        MSG_CG_GUILD_CHANGE_DUTY_SYN send_packet;
        StrnCopy(send_packet.m_szTargetCharName, member_info->m_tszCharName, MAX_CHARNAME_LENGTH);
        send_packet.m_szTargetCharName[_countof(send_packet.m_szTargetCharName) - 1] = '\0';
        send_packet.m_byDuty = change_duty;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_NAME_CHANGE_SYN( const TCHAR* guild_rename, POSTYPE item_position )
    {
        if (guild_rename == NULL)
        {
            return;
        }

        MSG_CG_GUILD_NAME_CHANGE_SYN send_packet;
        send_packet.m_atIndex = SI_INVENTORY;
        send_packet.m_atPos = item_position;
        StrnCopy(send_packet.m_tszGuildName, guild_rename, MAX_CHARNAME_LENGTH);
        send_packet.m_tszGuildName[_countof(send_packet.m_tszGuildName) - 1] = '\0';
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }


    void SEND_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ()
    {
        GuildSystem::GetGuildSystemData().ClearGuildLog();
        MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_ETC_GUILD_RANKINGINFO_SYN()
    {
        MSG_CG_ETC_GUILD_RANKINGINFO_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_ETC_GUILDINFO_SYN(GUILDGUID guild_guid)
    {
        MSG_CG_ETC_GUILDINFO_SYN send_packet;
        send_packet.guid_ = guild_guid;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CW_FRIEND_STATUS_REQ( const TCHAR* member_name, CHARGUID member_guid )
    {
        MSG_CW_FRIEND_STATUS_REQ send_packet;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
        if (member_guid <= 0)
        {
            return;
        }
        send_packet.m_dwToCharGuid = member_guid;
#else
        StrnCopy(send_packet.m_ptszCharName, member_name, MAX_CHARNAME_LENGTH);
        send_packet.m_ptszCharName[_countof(send_packet.m_ptszCharName) - 1] = '\0';
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &send_packet, sizeof(send_packet));	
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void SEND_CG_GUILD_JOIN_REQUEST_SYN(MSG_CG_GUILD_JOIN_REQUEST_SYN& send_packet)
    {
        // 길드 가입, 취소 요청
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_JOIN_REQUEST_APPROVE_SYN( MSG_CG_GUILD_JOIN_REQUEST_APPROVE_SYN& send_packet )
    {
        // 길드 가입 수락
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_JOIN_REQUEST_REJECT_SYN( MSG_CG_GUILD_JOIN_REQUEST_REJECT_SYN& send_packet )
    {
        // 길드 가입 거절
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
    
    void SEND_CG_GUILD_FACILITY_UPGRADE_SYN( const FCODE& facility_code )
    {
        // 길드 시설 업그레이드 요청
        MSG_CG_GUILD_FACILITY_UPGRADE_SYN send_packet;
        send_packet.facility_code = facility_code;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void SEND_CG_GUILD_FACILITY_ACTIVE_SYN( const FCODE& facility_code )
    {
        // 길드 시설 액티브 요청
        MSG_CG_GUILD_FACILITY_ACTIVE_SYN send_packet;
        send_packet.facility_code = facility_code;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

#endif // SEND_CG_GUILD_JOIN_REQUEST_SYN

    // recv packet function
    // 길드 생성
    void RECV_CG_GUILD_CREATE_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_CREATE_ACK* create_ack = static_cast<MSG_CG_GUILD_CREATE_ACK*>(recv_packet);

        // 70432 길드가 창설 되었습니다.
        GlobalFunc::VerifyMessage(70432);

        // 도움말 갱신
        uiHelpMan* helpMan =
            static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

        if (helpMan)
        {
            helpMan->CheckHelpMessage(eHK_GUILD);
        }

        uiGuideSystemMan* guide_manager =
            static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

        if (guide_manager)
        {
            guide_manager->CheckHelpMessage(TotalGuideNotice::kCreateGuild);
        }

        
        if (g_pHero != NULL)
        {
            // 하임 갱신
            MONEY current_money = g_pHero->GetMoney();
            const GuildInfoData& guild_info = GuildInfoParser::Instance()->GetGuildInfo();
            current_money -= guild_info.guild_create_heim_;
            g_pHero->SetMoney(current_money);
            
            // 길드명 세팅
            uiGuildCreate* guild_create = GET_CAST_DIALOG(uiGuildCreate, uiGuildCreate::kDialog_ID);
            if (guild_create != NULL)
            {
                TCHAR guild_name[INTERFACE_STRING_LENGTH] = {'\0', };
                guild_create->GetGuildName(guild_name, INTERFACE_STRING_LENGTH - 1);
                g_pHero->SetGuildName(guild_name, MAX_GUILDNAME_LENGTH);
                g_pHero->GetGuildRenderInfo().m_GuildDuty = eGUILD_DUTY_MASTER;
                g_pHero->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
            }            
        }

        // 길드 정보 요청
        GuildSystem::PacketFunction::SEND_CG_GUILD_TOTALINFO_REQ_SYN();

        // 길드관계 초기화
        GuildSystem::GetGuildSystemData().ClearGuildRelation();

        uiGuildCreate* ui_guild_create = GET_CAST_DIALOG(uiGuildCreate, uiGuildCreate::kDialog_ID);
        if (ui_guild_create != NULL)
        {
            ui_guild_create->ShowInterface(FALSE);
        }
    }

    void RECV_CG_GUILD_CREATE_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_CREATE_BRD* create_brd = static_cast<MSG_CG_GUILD_CREATE_BRD*>(recv_packet);

        Object* guild_master = g_ObjectManager.GetObject(create_brd->m_MasterPlayerKey);
        if ((guild_master != NULL) && 
            (guild_master->IsKindOfObject(PLAYER_OBJECT) == TRUE))
        {
            static_cast<Player*>(guild_master)->SetGuildName(
                create_brd->m_tszGuildName, 
                MAX_GUILDNAME_LENGTH);
        }
    }

    void RECV_CG_GUILD_CREATE_NAK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_CREATE_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_CREATE_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    // 길드원 로그인 정보
    void RECV_CG_GUILD_LOGIN_CHAR_CMD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_LOGIN_CHAR_CMD* guild_login_cmd = 
            static_cast<MSG_CG_GUILD_LOGIN_CHAR_CMD*>(recv_packet);
        GuildSystem::GetGuildSystemData().SetGuildInfo(guild_login_cmd->m_GuildInfo);

        // 길드멤버 정보
        GuildSystem::GetGuildSystemData().AddGuildMemberInfo(guild_login_cmd->m_MemberInfo);

        // 캐릭터 최초 접속시 변경된 길드공지가 있는지 요청
        MSG_CW_GUILD_SELECT_NOTICE_SYN send_packet;
        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &send_packet, sizeof(send_packet));
    }

    void RECV_CG_GUILD_LOGIN_CHAR_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_LOGIN_CHAR_BRD* guild_login_brd = 
            static_cast<MSG_CG_GUILD_LOGIN_CHAR_BRD*>(recv_packet);
        
        TCHAR member_name[MAX_CHARNAME_LENGTH + 1] = {'\0', };
        StrnCopy(member_name, guild_login_brd->m_szCharName, MAX_CHARNAME_LENGTH);
        member_name[_countof(member_name) - 1] = '\0';

        GUILD_MEMBER_INFO* member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(member_name);
        if (member_info != NULL)
        {
            member_info->m_bLogin = TRUE;
        }

        if ((g_pHero != NULL) && 
            (StrnCmp(g_pHero->GetName(), member_name, MAX_CHARNAME_LENGTH) != 0))
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            // 6263 길드원 %s 님이 로그인하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                6263, 
                member_name);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
        }
    }

    void RECV_CG_GUILD_LOGOUT_CHAR_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_LOGOUT_CHAR_BRD* guild_loginout_brd = 
            static_cast<MSG_CG_GUILD_LOGOUT_CHAR_BRD*>(recv_packet);

        TCHAR member_name[MAX_CHARNAME_LENGTH + 1] = {'\0', };
        StrnCopy(member_name, guild_loginout_brd->m_szCharName, MAX_CHARNAME_LENGTH);
        member_name[_countof(member_name) - 1] = '\0';

        GUILD_MEMBER_INFO* member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(member_name);
        if (member_info != NULL)
        {
            member_info->m_bLogin = FALSE;
        }

        if ((g_pHero != NULL) && 
            (StrnCmp(g_pHero->GetName(), member_name, MAX_CHARNAME_LENGTH) != 0))
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            // 6264 길드원 %s 님이 로그아웃하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                6264, 
                member_name);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
        }
    }

    void RECV_CG_GUILD_CHAR_LEVELUP_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHAR_LEVELUP_BRD* char_levelup_brd = 
            static_cast<MSG_CG_GUILD_CHAR_LEVELUP_BRD*>(recv_packet);

        TCHAR member_name[MAX_CHARNAME_LENGTH + 1] = {'\0', };
        StrnCopy(member_name, char_levelup_brd->m_szCharName, MAX_CHARNAME_LENGTH);
        member_name[_countof(member_name) - 1] = '\0';

        GUILD_MEMBER_INFO* member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(member_name);
        if (member_info != NULL)
        {
            member_info->m_Level = char_levelup_brd->m_Level;
        }
    }

    // 길드원 초대
    void RECV_CG_GUILD_JOIN_FAILED_CMD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_JOIN_FAILED_CMD* nak_packet = 
            static_cast<MSG_CG_GUILD_JOIN_FAILED_CMD*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    void RECV_CG_GUILD_INVITE_CMD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_INVITE_CMD* invite_cmd = static_cast<MSG_CG_GUILD_INVITE_CMD*>(recv_packet);

        uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (ui_system_manager != NULL)
        {
            TCHAR invite_message[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
            // 7364 %s 길드로부터 길드가입 권유 메시지가 도착했습니다.\n승낙하시겠습니까?
            g_InterfaceManager.GetInterfaceStringFormat(invite_message, 
                INTERFACE_STRING_LENGTH, 
                7364, 
                invite_cmd->m_OperGuildName);
            
            ui_system_manager->Sys_SystemMessage(invite_message, 
                NULL, 
                GuildSystem::Function::Callback_GuildInvite, 
                uiSystemMessage::kConfirm_Type, 
                false, 
                15000, 
                100, 
                155536, 
                155537, 
                static_cast<DWORD>(invite_cmd->m_OperUserKey));
        }
    }

    void RECV_CG_GUILD_INVITE_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_INVITE_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_INVITE_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    void RECV_CG_GUILD_INVITE_ANSWER_CMD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_INVITE_ANSWER_CMD* invite_answer_cmd = 
            static_cast<MSG_CG_GUILD_INVITE_ANSWER_CMD*>(recv_packet);

        DWORD message_code = 0;
        if (invite_answer_cmd->m_bInveite == FALSE)
        {
            // 거부
            message_code = 6262;
        }
        else
        {
            // 승락
            message_code = 6260;

            MSG_CG_GUILD_JOIN_SYN send_packet;
            send_packet.m_TargetObjKey = invite_answer_cmd->m_TargetObjKey;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceStringFormat(
            message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            message_code, 
            invite_answer_cmd->m_TargetCharName);
        GlobalFunc::VerifyMessage(message_string, NULL);
    }

    void RECV_CG_GUILD_INVITE_ANSWER_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_INVITE_ANSWER_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_INVITE_ANSWER_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    void RECV_CG_GUILD_JOIN_ACK(MSG_BASE* recv_packet)
    {

    }

    void RECV_CG_GUILD_JOIN_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_JOIN_BRD* join_brd = static_cast<MSG_CG_GUILD_JOIN_BRD*>(recv_packet);

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        // 5208 %s님이 길드에 가입하셨습니다.
        g_InterfaceManager.GetInterfaceStringFormat(message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            5208, 
            join_brd->m_MemberInfo.m_tszCharName);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
        
        GuildSystem::GetGuildSystemData().AddGuildMemberInfo(join_brd->m_MemberInfo);
    }

    void RECV_CG_GUILD_JOIN_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_JOIN_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_JOIN_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    void RECV_CG_GUILD_JOIN_CMD(MSG_BASE* recv_packet)
    {
        // 70484 길드 가입을 성공했습니다.
        GlobalFunc::VerifyMessage(70484);
        SEND_CG_GUILD_TOTALINFO_REQ_SYN();
    }

    // 길드원 탈퇴/강퇴
    void RECV_CG_GUILD_WITHDRAW_ACK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_WITHDRAW_ACK* withdraw_ack = static_cast<MSG_CG_GUILD_WITHDRAW_ACK*>(recv_packet);

        GuildSystem::Function::CloseAllGuildDialog();

        // 70503 길드를 탈퇴 하셨습니다.
        GlobalFunc::VerifyMessage(70503);
        GuildSystem::Function::ClearPlayerRelation();
        GuildSystem::GetGuildSystemData().Clear();
    }

    void RECV_CG_GUILD_WITHDRAW_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_WITHDRAW_BRD* withdraw_brd = static_cast<MSG_CG_GUILD_WITHDRAW_BRD*>(recv_packet);

        TCHAR withdraw_member[MAX_CHARNAME_LENGTH + 1] ={'\0', }; // NULL 처리 확실히 하기 위하여
        StrnCopy(withdraw_member, withdraw_brd->m_szCharName, MAX_CHARNAME_LENGTH);
        withdraw_member[_countof(withdraw_member) - 1] = '\0';

        GuildSystem::GetGuildSystemData().DeleteGuildMemberInfo(withdraw_member);
        
        Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(withdraw_member));
        if (player_ptr)
        {
            player_ptr->ClearGuildRenderInfo();
        }
    }

    void RECV_CG_GUILD_WITHDRAW_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_WITHDRAW_NAK* withdraw_nak = static_cast<MSG_CG_GUILD_WITHDRAW_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(withdraw_nak->m_byErrorCode);
    }

    void RECV_CG_GUILD_KICKPLAYER_CMD(MSG_BASE* recv_packet)
    {
        GuildSystem::Function::CloseAllGuildDialog();

        // 5169 길드에서 제명되었습니다.
        GlobalFunc::VerifyMessage(5169);
        GuildSystem::Function::ClearPlayerRelation();
        GuildSystem::GetGuildSystemData().Clear();
    }

    void RECV_CG_GUILD_KICKPLAYER_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_KICKPLAYER_BRD* kickplayer_brd = static_cast<MSG_CG_GUILD_KICKPLAYER_BRD*>(recv_packet);

        GuildSystem::GetGuildSystemData().DeleteGuildMemberInfo(kickplayer_brd->m_szCharName);

        Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(kickplayer_brd->m_szCharName));
        if (player_ptr)
        {
            player_ptr->ClearGuildRenderInfo();
        }

        // 5171 '%s'길드원이 제명되었습니다.
        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceStringFormat(message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            5171, 
            kickplayer_brd->m_szCharName);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
    }

    void RECV_CG_GUILD_KICKPLAYER_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_KICKPLAYER_NAK* kickplayer_nak = 
            static_cast<MSG_CG_GUILD_KICKPLAYER_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(kickplayer_nak->m_dwErrorCode);
    }

    void RECV_CG_GUILD_LEAVE_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_LEAVE_BRD* leave_brd = static_cast<MSG_CG_GUILD_LEAVE_BRD*>(recv_packet);

        Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(leave_brd->m_LeaverObjectKey));
        if (player_ptr)
        {
            player_ptr->ClearGuildRenderInfo();
        }
    }

    // 길드원 직책변경
    void RECV_CG_GUILD_CHANGE_DUTY_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHANGE_DUTY_NAK* change_duty_nak = 
            static_cast<MSG_CG_GUILD_CHANGE_DUTY_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(change_duty_nak->m_byErrorCode);
    }

    void RECV_CG_GUILD_CHANGE_DUTY_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHANGE_DUTY_BRD* change_duty_brd = 
            static_cast<MSG_CG_GUILD_CHANGE_DUTY_BRD*>(recv_packet);

        Player* player_ptr = 
            static_cast<Player*>(g_ObjectManager.GetObject(change_duty_brd->m_szTargetCharName));
        if (player_ptr != NULL)
        {
            player_ptr->GetGuildRenderInfo().m_GuildDuty = change_duty_brd->m_byDuty;
        }

        GUILD_MEMBER_INFO* guild_member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(change_duty_brd->m_szTargetCharName);
        if (guild_member_info == NULL)
        {
            return;
        }
        
        guild_member_info->m_eGuildDuty = static_cast<eGUILD_DUTY>(change_duty_brd->m_byDuty);
        // 6276 %s님의 직책이 %s로 변경되었습니다.
        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR duty_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(
            GuildSystem::Function::GetGuildDutyStringCode(guild_member_info->m_eGuildDuty), 
            duty_string);
        g_InterfaceManager.GetInterfaceStringFormat(message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            6276, 
            change_duty_brd->m_szTargetCharName, 
            duty_string);
        GlobalFunc::VerifyMessage(message_string, NULL);
    }

    // 길드레벨업
    void RECV_CG_GUILD_RANKUP_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RANKUP_BRD* guild_rankup_brd = 
            static_cast<MSG_CG_GUILD_RANKUP_BRD*>(recv_packet);

        // 레벨갱신
        int string_code = 0;
        BYTE& current_guild_grade = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGrade;
        if (current_guild_grade < guild_rankup_brd->m_byNowLevel)
        {
            // 5181 축하합니다! 길드가 %s로 레벨업 되었습니다.
            string_code = 5181;
        }
        else if (current_guild_grade > guild_rankup_brd->m_byNowLevel)
        {
            // 7359 길드가 %s로 레벨이 다운 되었습니다.
            string_code = 7359;
        }
        current_guild_grade = guild_rankup_brd->m_byNowLevel;

        if (string_code != 0)
        {
            TCHAR rank_message[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR new_rank[INTERFACE_STRING_LENGTH] = {'\0', };
            _itot(current_guild_grade, new_rank, 10);
            g_InterfaceManager.GetInterfaceStringFormat(rank_message, 
                INTERFACE_STRING_LENGTH - 1, 
                string_code, 
                new_rank);

            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, rank_message);
            GlobalFunc::VerifyMessage(rank_message, NULL);
            
            // 길드 레벨업 이펙트
            if (g_pHero != NULL)
            {
                g_pHero->CreateEffectAtHead(StrToWzID("A585"),0.0f);
            }
        }
        g_pHero->GetGuildRenderInfo().m_GuildGrade = current_guild_grade;
    }

    // 길드관계 정보
    void RECV_CG_GUILD_ADD_INFO_ACK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_ADD_INFO_ACK* guild_add_info_ack = 
            static_cast<MSG_CG_GUILD_ADD_INFO_ACK*>(recv_packet);

        // 길드 관계 초기화
        GuildSystem::GetGuildSystemData().ClearGuildRelation();

        // 길드 관계 정보 세팅
        GuildSystem::GetGuildSystemData().SetGuildRelationInfo(guild_add_info_ack->m_AddInfo);
    }

    void RECV_CG_GUILD_ADD_INFO_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_ADD_INFO_NAK* guild_add_info_nak = 
            static_cast<MSG_CG_GUILD_ADD_INFO_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(guild_add_info_nak->m_byErrorCode);
    }

    void RECV_CG_GUILD_RELATION_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_RELATION_NAK* relation_nak = 
            static_cast<MSG_CG_GUILD_RELATION_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(relation_nak->m_byErrorCode);
    }

    void RECV_CG_GUILD_RELATION_REQUEST_CMD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_REQUEST_CMD* relation_request_cmd = 
            static_cast<MSG_CG_GUILD_RELATION_REQUEST_CMD*>(recv_packet);

        uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (ui_system_manager != NULL)
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            if (relation_request_cmd->m_eType == eGUILD_RELATION_ALLIENCE)
            {
                // 동맹 신청 수락 질의
                g_InterfaceManager.GetInterfaceStringFormat(
                    message_string, 
                    INTERFACE_STRING_LENGTH - 1, 
                    1167, 
                    relation_request_cmd->m_RequestGuildName);
                ui_system_manager->Sys_SystemMessage(
                    message_string, 
                    NULL, 
                    GuildSystem::Function::Callback_GuildRelationAnswerAllince, 
                    uiSystemMessage::kConfirm_Type, 
                    false, 
                    15000, 
                    100, 
                    155536, 
                    155537, 
                    static_cast<DWORD>(relation_request_cmd->m_RequestUserKey));
            }
            else if (relation_request_cmd->m_eType == eGUILD_RELATION_CANCEL_BOTH_HOSTILITY)
            {
                // 적대 해제 수락 질의
                g_InterfaceManager.GetInterfaceStringFormat(
                    message_string, 
                    INTERFACE_STRING_LENGTH - 1, 
                    7354, 
                    relation_request_cmd->m_RequestGuildName);
                ui_system_manager->Sys_SystemMessage(
                    message_string, 
                    NULL, 
                    GuildSystem::Function::Callback_GuildRelationAnswerHostility, 
                    uiSystemMessage::kConfirm_Type, 
                    false, 
                    15000, 
                    100, 
                    155536, 
                    155537, 
                    static_cast<DWORD>(relation_request_cmd->m_RequestUserKey));
            }
        }
    }

    void RECV_CG_GUILD_RELATION_REJECT_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_REJECT_ACK* relation_reject_ack = 
            static_cast<MSG_CG_GUILD_RELATION_REJECT_ACK*>(recv_packet);

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        if (relation_reject_ack->m_eRelationType == eGUILD_RELATION_ALLIENCE)
        {
            // 6177 %s 길드의 길드 마스터가 동맹 신청을 거절하였습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                6177, 
                relation_reject_ack->m_szRelGuildName);
        }
        else if (relation_reject_ack->m_eRelationType == eGUILD_RELATION_CANCEL_BOTH_HOSTILITY)
        {
            // 7355 %s 길드가 적대관계 해제를 거부하여 적대관계가 유지되었습니다.
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                7355, 
                relation_reject_ack->m_szRelGuildName);
        }
        else
        {
            return;
        }

        GlobalFunc::VerifyMessage(message_string, NULL);
    }

    void RECV_CG_GUILD_RELATION_DELETE_NAK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_DELETE_NAK* relation_delete_nak = 
            static_cast<MSG_CG_GUILD_RELATION_DELETE_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(relation_delete_nak->m_byErrorCode);
    }

    void RECV_CG_GUILD_RELATION_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_BRD* relation_brd = 
            static_cast<MSG_CG_GUILD_RELATION_BRD*>(recv_packet);

        // 서버에서 본인의 길드를 보내는 일이있다. 거르자.
        // (거르지않으면 자신의 길드와 동맹,적대관계를 맺는 상황이 발생)
        GUILD_RELATION& guild_relation = relation_brd->m_stRELGuild;
        if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid == guild_relation.m_GuildGuid)
        {
            return;
        }

        int message_code = 0;
        switch (guild_relation.m_eRelationType)
        {
        case eGUILD_RELATION_ALLIENCE:
            {
                GuildSystem::GetGuildSystemData().AddGuildRelation(guild_relation);
                // 6183 %s 길드와 동맹관계가 성립되었습니다.
                message_code = 6183;
            }
            break;
        case eGUILD_RELATION_PROCLAIM_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().AddGuildRelation(guild_relation);
                // 6184 %s 길드에게 적대 선포를 하였습니다.
                message_code = 6184;
            }
            break;
        case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().AddGuildRelation(guild_relation);
                // 6185 %s 길드와 일방적인 적대관계가 선포되었습니다.
                message_code = 6185;
            }
            break;
        case eGUILD_RELATION_CANCEL_BOTH_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().DeleteGuildRelation(guild_relation);
                // 6187 %s 길드와 적대관계가 해제 되었습니다.
                message_code = 6187;
            }
            break;
        }

        if (message_code != 0)
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                message_code, 
                guild_relation.m_GuildName);
            GlobalFunc::VerifyMessage(message_string, NULL);
        }
    }

    void RECV_CG_GUILD_RELATION_DELETE_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_DELETE_BRD* relation_delete_brd = 
            static_cast<MSG_CG_GUILD_RELATION_DELETE_BRD*>(recv_packet);

        GUILDGUID& guild_guid = relation_delete_brd->m_RelGuildGuid;
        if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid == guild_guid)
        {
            return;
        }

        TCHAR guild_name[INTERFACE_STRING_LENGTH] = {'\0', };
        eGUILD_RELATION guild_relation = relation_delete_brd->m_eRelType;
        int message_code = 0;
        switch (guild_relation)
        {
        case eGUILD_RELATION_ALLIENCE:
            {
                GuildSystem::GetGuildSystemData().GetRelationGuildName(guild_guid, guild_name);
                GuildSystem::GetGuildSystemData().DeleteGuildRelation(guild_guid, guild_relation);
                // 5199 %s 길드와 동맹관계가 해제되었습니다.
                message_code = 5199;
            }
            break;
        case eGUILD_RELATION_PROCLAIM_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().GetRelationGuildName(guild_guid, guild_name);
                GuildSystem::GetGuildSystemData().DeleteGuildRelation(guild_guid, guild_relation);
                // 7360 %s 길드에 적대 선포를 취소하였습니다.
                message_code = 7360;
            }
            break;
        case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().GetRelationGuildName(guild_guid, guild_name);
                GuildSystem::GetGuildSystemData().DeleteGuildRelation(guild_guid, guild_relation);
                // 7361 %s 길드로부터 적대 선포가 취소 되었습니다.
                message_code = 7361;
            }
            break;
        case eGUILD_RELATION_CANCEL_BOTH_HOSTILITY:
            {
                GuildSystem::GetGuildSystemData().GetRelationGuildName(guild_guid, guild_name);
                GuildSystem::GetGuildSystemData().DeleteGuildRelation(guild_guid, guild_relation);
                // 6187 %s 길드와 적대관계가 해제 되었습니다.
                message_code = 6187;
            }
            break;
        }

        if (message_code != 0)
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                message_code, 
                guild_name);
            GlobalFunc::VerifyMessage(message_string, NULL);
        }
    }

    void RECV_CG_GUILD_RELATION_BOTH_AGAINST_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD* both_against_brd = 
            static_cast<MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD*>(recv_packet);

        GUILDGUID& guild_guid = both_against_brd->m_BothGuildGuid;
        if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid == guild_guid)
        {
            return;
        }

        GUILD_RELATION* guild_relation = GuildSystem::GetGuildSystemData().GetGuildRelation(guild_guid);
        if (guild_relation == NULL)
        {
            return;
        }

        GUILD_RELATION new_guild_relation = (*guild_relation);

        GuildSystem::GetGuildSystemData().DeleteGuildRelation(
            guild_guid, eGUILD_RELATION_PROCLAIM_HOSTILITY);
        GuildSystem::GetGuildSystemData().DeleteGuildRelation(
            guild_guid, eGUILD_RELATION_ONE_SIDE_HOSTILITY);
        
        new_guild_relation.m_RelationApplyTime = both_against_brd->m_RelationApplyTime;
        new_guild_relation.m_RelationChangeTime = both_against_brd->m_RelationChangeTime;
        new_guild_relation.m_eRelationType = eGUILD_RELATION_BOTH_HOSTILITY;

        GuildSystem::GetGuildSystemData().AddGuildRelation(new_guild_relation);

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        // 6186 %s 길드와 적대관계가 되었습니다.
        g_InterfaceManager.GetInterfaceStringFormat(
            message_string, 
            INTERFACE_STRING_LENGTH - 1,
            6186, 
            new_guild_relation.m_GuildName);
        GlobalFunc::VerifyMessage(message_string, NULL);
    }

    void RECV_CG_GUILD_RELATION_MOVE_BRD( MSG_BASE* recv_packet )
    {

    }

    // 길드 종합정보
    void RECV_CG_GUILD_TOTALINFO_REQ_ACK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_TOTALINFO_REQ_ACK* total_info_request = 
            static_cast<MSG_CG_GUILD_TOTALINFO_REQ_ACK*>(recv_packet);

        GuildSystem::GetGuildSystemData().SetGuildInfo(total_info_request->m_GuildInfo);
        GuildSystem::GetGuildSystemData().SetGuildMemberInfo(total_info_request->m_MemberTotalInfo);
        GuildSystem::GetGuildSystemData().set_guild_rank(total_info_request->guild_info_extra_.ranking_);
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        GuildSystem::GetGuildSystemData().set_guild_intro(total_info_request->guild_info_extra_.introduction_strings_);
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        GuildSystem::GetGuildSystemData().set_guild_join_request_total_info_(&total_info_request->m_GuildJoinRequestTotalInfo);
        uiGuildIntro* ui_guild_intro = GET_CAST_DIALOG(uiGuildIntro, uiGuildIntro::kDialog_ID);
        if (ui_guild_intro)
        {
            ui_guild_intro->GetGuildIntro();
        }
        uiGuildInfoOther* ui_guild_info_other = GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
        if (ui_guild_info_other)
        {
            ui_guild_info_other->RequestJoinButtonReset();
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
        if (ui_guild_info != NULL)
        {
            if (ui_guild_info->show_flag() == true)
            {
                ui_guild_info->ShowInterface(TRUE);
            }
        }

        uiConvenienceStore_FriendList* friend_list_dlg = 
            GET_CAST_DIALOG(uiConvenienceStore_FriendList, uiConvenienceStore_FriendList::kDialog_ID);
        if ((friend_list_dlg != NULL) && 
            (friend_list_dlg->IsVisible() == TRUE))
        {
            friend_list_dlg->RefreshFriendList();
        }

        MSG_CG_GUILD_ADD_INFO_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    void RECV_CG_GUILD_TOTALINFO_REQ_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_TOTALINFO_REQ_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_TOTALINFO_REQ_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

    // 카오 상태 변경
    void RECV_CG_GUILD_CHANGE_CHAOSTATE_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHANGE_CHAOSTATE_BRD* change_chaostate_brd = 
            static_cast<MSG_CG_GUILD_CHANGE_CHAOSTATE_BRD*>(recv_packet);

        TCHAR member_name[MAX_CHARNAME_LENGTH + 1] = {'\0', };
        StrnCopy(member_name, change_chaostate_brd->m_szCharName, MAX_CHARNAME_LENGTH);
        member_name[_countof(member_name) - 1] = '\0';

        GUILD_MEMBER_INFO* member_info = GuildSystem::GetGuildSystemData().GetGuildMemberInfo(member_name);
        if (member_info != NULL)
        {
            member_info->m_byChaoState = change_chaostate_brd->m_byChaoState;
        }
    }

    // 길드 마크
    void RECV_CG_GUILD_GUILDMARK_REGISTER_ACK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_REGISTER_ACK* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_REGISTER_ACK*>(recv_packet);
        
        GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
        guild_info.m_BackGroundIdx = guildmark_packet->m_BackGroundIndex;
        guild_info.m_PatternIdx = guildmark_packet->m_PatternIndex;

        if (g_pHero != NULL)
        {
            g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
            g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;
        }

        // 6164 길드마크가 정상적으로 등록되었습니다.
        GlobalFunc::VerifyMessage(6164);

        uiBase* guild_mark_combination = GET_DIALOG(uiGuildMarkCombination::kDialog_ID);
        if (guild_mark_combination != NULL)
        {
            guild_mark_combination->ShowInterface(FALSE);
        }
    }

    void RECV_CG_GUILD_GUILDMARK_REGISTER_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_REGISTER_BRD* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_REGISTER_BRD*>(recv_packet);

        if ((g_pHero != NULL) && 
            (g_pHero->GetObjectKey() == guildmark_packet->m_dwPlayerKey))
        {
            GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
            guild_info.m_BackGroundIdx = guildmark_packet->m_BackGroundIndex;
            guild_info.m_PatternIdx = guildmark_packet->m_PatternIndex;

            g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
            g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;

            g_pHero->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;

            //6127	길드마크가 등록 되었습니다.
            GlobalFunc::VerifyMessage(6127);
        }
    }

    void RECV_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD(MSG_BASE* recv_packet)
    {
        int a = 0;
        ++a;
        //MSG_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD* guildmark_packet = 
        //    static_cast<MSG_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD*>(recv_packet);

        //GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
        //guild_info.m_BackGroundIdx = guildmark_packet->m_BackGroundIndex;
        //guild_info.m_PatternIdx = guildmark_packet->m_PatternIndex;

        //if ((g_pHero != NULL) && 
        //    (g_pHero->GetObjectKey() == guildmark_packet->m_dwPlayerKey))
        //{
        //    g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
        //    g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;

        //    g_pHero->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
        //}

        //Player* player_object = 
        //    static_cast<Player*>(g_ObjectManager.GetObject(guildmark_packet->m_dwPlayerKey));
        //if (player_object != NULL)
        //{
        //    // 해당 길드와의 길드 관계 세팅
        //}
    }

    void RECV_CG_GUILD_GUILDMARK_REGISTER_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_REGISTER_NAK* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_REGISTER_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(guildmark_packet->m_bErrorCode);
    }

    void RECV_CG_GUILD_GUILDMARK_DELETE_ACK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_DELETE_ACK* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_DELETE_ACK*>(recv_packet);

        GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
        guild_info.m_BackGroundIdx = guildmark_packet->m_BackGroundIndex;
        guild_info.m_PatternIdx = guildmark_packet->m_PatternIndex;

        if (g_pHero != NULL)
        {
            g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
            g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;

            g_pHero->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;

            //7241	길드 마크가 정상적으로 삭제되었습니다.
            GlobalFunc::VerifyMessage(7241);
        }
    }

    void RECV_CG_GUILD_GUILDMARK_DELETE_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_DELETE_BRD* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_DELETE_BRD*>(recv_packet);

        if ((g_pHero != NULL) && 
            (g_pHero->GetObjectKey() == guildmark_packet->m_dwPlayerKey))
        {
            GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
            guild_info.m_BackGroundIdx = guildmark_packet->m_BackGroundIndex;
            guild_info.m_PatternIdx = guildmark_packet->m_PatternIndex;

            g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
            g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;

            g_pHero->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;

            //7241	길드 마크가 정상적으로 삭제되었습니다.
            GlobalFunc::VerifyMessage(7241);
        }
    }

    void RECV_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD(MSG_BASE* recv_packet)
    {
        //MSG_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD* guildmark_packet = 
        //    static_cast<MSG_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD*>(recv_packet);
        int a = 0;
        ++a;
    }

    void RECV_CG_GUILD_GUILDMARK_DELETE_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_GUILDMARK_DELETE_NAK* guildmark_packet = 
            static_cast<MSG_CG_GUILD_GUILDMARK_DELETE_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(guildmark_packet->m_bErrorCode);
    }

    // 길드마스터 변경
    void RECV_CG_GUILD_CHANGE_MASTER_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHANGE_MASTER_BRD* change_master_brd = 
            static_cast<MSG_CG_GUILD_CHANGE_MASTER_BRD*>(recv_packet);

        TCHAR old_master_name[MAX_CHARNAME_LENGTH + 1] = {'\0', }; // NULL 처리 확실히 하기 위하여
        StrnCopy(old_master_name, change_master_brd->m_szOldMasterName, MAX_CHARNAME_LENGTH);
        old_master_name[_countof(old_master_name) - 1] = '\0';


        TCHAR new_master_name[MAX_CHARNAME_LENGTH + 1] = {'\0', }; // NULL 처리 확실히 하기 위하여
        StrnCopy(new_master_name, change_master_brd->m_szNewMasterName, MAX_CHARNAME_LENGTH);
        new_master_name[_countof(new_master_name) - 1] = '\0';

        GUILD_MEMBER_INFO* old_master_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(old_master_name);
        if (old_master_info != NULL)
        {
            old_master_info->m_eGuildDuty = eGUILD_DUTY_REGULAR_SOLDIER;
        }

        GUILD_MEMBER_INFO* new_master_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(new_master_name);
        if (new_master_info != NULL)
        {
            new_master_info->m_eGuildDuty = eGUILD_DUTY_MASTER;
        }

        if ((old_master_info == NULL) || 
            (new_master_info == NULL))
        {
            assert(old_master_info);
            assert(new_master_info);
        }

        Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(old_master_name));
        if (player_ptr != NULL)
        {
            player_ptr->GetGuildRenderInfo().m_GuildDuty = eGUILD_DUTY_REGULAR_SOLDIER;
        }
        player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(new_master_name));
        if (player_ptr != NULL)
        {
            player_ptr->GetGuildRenderInfo().m_GuildDuty = eGUILD_DUTY_MASTER;
        }

        // 6279 길드마스터가 %s님으로 변경되었습니다.
        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceStringFormat(message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            6279, 
            new_master_name);
        GlobalFunc::VerifyMessage(message_string, NULL);
        GuildSystem::GetGuildSystemData().SetMasterName(new_master_name);
    }

    void RECV_CG_GUILD_CHANGE_MASTER_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_CHANGE_MASTER_NAK* change_master_nak = 
            static_cast<MSG_CG_GUILD_CHANGE_MASTER_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(change_master_nak->m_byErrorCode);
    }

    // 길드명 변경
    void RECV_CG_GUILD_NAME_CHANGE_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_NAME_CHANGE_BRD* guild_name_change_brd = 
            static_cast<MSG_CG_GUILD_NAME_CHANGE_BRD*>(recv_packet);

        g_pHero->SetGuildName(guild_name_change_brd->m_tszGuildName, MAX_GUILDNAME_LENGTH);
        GuildSystem::GetGuildSystemData().SetGuildName(guild_name_change_brd->m_tszGuildName);

        // 7252 길드명이 %s로 변경되었습니다.
        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceStringFormat(message_string, 
            INTERFACE_STRING_LENGTH - 1, 
            7252, 
            guild_name_change_brd->m_tszGuildName);
        GlobalFunc::VerifyMessage(message_string, NULL);
    }

    void RECV_CG_GUILD_NAME_CHANGE_SECTOR_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_NAME_CHANGE_SECTOR_BRD* guild_name_change_sector_brd = 
            static_cast<MSG_CG_GUILD_NAME_CHANGE_SECTOR_BRD*>(recv_packet);

        Player* player_ptr = 
            static_cast<Player*>(g_ObjectManager.GetObject(guild_name_change_sector_brd->m_dwPlayerKey));
        if (player_ptr != NULL)
        {
            player_ptr->SetGuildName(guild_name_change_sector_brd->m_tszGuildName, MAX_GUILDNAME_LENGTH);
        }
    }

    void RECV_CG_GUILD_NAME_CHANGE_RELATION_BRD(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_NAME_CHANGE_RELATION_BRD* guild_name_change_relation_brd = 
            static_cast<MSG_CG_GUILD_NAME_CHANGE_RELATION_BRD*>(recv_packet);

        GUILD_RELATION* guild_relation = 
            GuildSystem::GetGuildSystemData().GetGuildRelation(guild_name_change_relation_brd->m_GuildGuid);
        if (guild_relation != NULL)
        {
            StrnCopy(guild_relation->m_GuildName, 
                guild_name_change_relation_brd->m_tszGuildName, 
                MAX_CHARNAME_LENGTH);
            guild_relation->m_GuildName[_countof(guild_relation->m_GuildName) - 1] = '\0';
        }
    }

    void RECV_CG_GUILD_NAME_CHANGE_NAK(MSG_BASE* recv_packet)
    {
        MSG_CG_GUILD_NAME_CHANGE_NAK* guild_name_change_nak = 
            static_cast<MSG_CG_GUILD_NAME_CHANGE_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(guild_name_change_nak->m_bErrorCode);
    }

    //////////////////////////////////////////////////////////////////////////

    void RECV_CG_GUILD_DESTROY_ACK( MSG_BASE* recv_packet )
    {
        // 70483 길드를 해산 하셨습니다.
        GlobalFunc::VerifyMessage(70483, 0);

        GuildSystem::Function::CloseAllGuildDialog();
        GuildSystem::GetGuildSystemData().Clear();
        GuildSystem::Function::ClearGuildRelation();
    }

    void RECV_CG_GUILD_DESTROY_NAK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_DESTROY_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_DESTROY_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
        
    }

    void RECV_CG_GUILD_DESTROY_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_DESTROY_BRD* destroy_brd =
            static_cast<MSG_CG_GUILD_DESTROY_BRD*>(recv_packet);

        Player* player_ptr = 
            static_cast<Player*>(g_ObjectManager.GetObject(destroy_brd->m_MasterPlayerKey));
        if (player_ptr != NULL)
        {
            player_ptr->ClearGuildRenderInfo();
        }
    }

    void RECV_CG_GUILD_DESTROY_WAIT_ACK( MSG_BASE* recv_packet )
    {
        GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildDestroyWaitFlag = eGUILD_DELCHK_DESTROY_WAIT;

        // 6236 길드 해산 대기 중입니다.
        GlobalFunc::VerifyMessage(6236, 0);

        GuildSystem::Function::CloseAllGuildDialog();
        
        GuildSystem::GetGuildSystemData().ClearGuildRelation();

        //if (g_pHero != NULL)
        //{
        //    g_pHero->ClearGuildRenderInfo();
        //}

        GuildSystem::Function::ClearGuildRelation();
    }

    void RECV_CG_GUILD_DESTROY_WAIT_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_DESTROY_WAIT_BRD* destroy_wait_brd = 
            static_cast<MSG_CG_GUILD_DESTROY_WAIT_BRD*>(recv_packet);

        GuildSystem::Function::SetPlayerGuildRelation(
            destroy_wait_brd->m_RelGuildGuid, eGUILD_RELATION_NONE);

        GUILD_RELATION* guild_relation = 
            GuildSystem::GetGuildSystemData().GetGuildRelation(destroy_wait_brd->m_RelGuildGuid);
        if (guild_relation != NULL)
        {
            // 7236 %s길드가 해산 신청을 하여 해당 길드와의 길드 관계는 모두 해제 되었습니다.
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1, 
                7236, 
                guild_relation->m_GuildName);
            GlobalFunc::VerifyMessage(message_string, NULL);
            GuildSystem::GetGuildSystemData().DeleteGuildRelation(*guild_relation);
        }
    }

    void RECV_CG_GUILD_RECOVERY_GUILD_ACK( MSG_BASE* recv_packet )
    {
        GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildDestroyWaitFlag = eGUILD_DELCHK_NORMAL;

        // 6234 길드 해산 해제 신청이 완료 되었습니다.
        GlobalFunc::VerifyMessage(6234, 0);

        GuildSystem::Function::CloseAllGuildDialog();
    }

    void RECV_CG_GUILD_RECOVERY_GUILD_NAK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_RECOVERY_GUILD_NAK* nak_packet = 
            static_cast<MSG_CG_GUILD_RECOVERY_GUILD_NAK*>(recv_packet);
        GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);

    }

    // 길드 로그
    void RECV_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS* guild_log_packet = 
            static_cast<MSG_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS*>(recv_packet);

        GuildSystem::GetGuildSystemData().AddGuildLog(guild_log_packet->log_category_, 
            &(guild_log_packet->log_data_[0]), guild_log_packet->log_count_);

        uiGuildLog* ui_guild_log = GET_CAST_DIALOG(uiGuildLog, uiGuildLog::kDialog_ID);
        if (ui_guild_log != NULL)
        {
            ui_guild_log->SetGuildLogUpList();
        }
    }

    // 길드 랭크
    void RECV_CG_ETC_GUILD_RANKINGINFO_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_ETC_GUILD_RANKINGINFO_ACK* guild_ranking_info_packet = 
            static_cast<MSG_CG_ETC_GUILD_RANKINGINFO_ACK*>(recv_packet);
        GuildSystem::GetGuildSystemData().AddGuildRankingInfo(
            guild_ranking_info_packet->guild_ranking_info_, guild_ranking_info_packet->guild_ranking_info_count_);
        
        uiGuildRanking* ui_guild_ranking = GET_CAST_DIALOG(uiGuildRanking, uiGuildRanking::kDialog_ID);
        if (ui_guild_ranking != NULL)
        {
            ui_guild_ranking->UpdateRankList();
        }
    }

    void RECV_CG_CG_ETC_GUILDINFO_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_ETC_GUILDINFO_ACK* guild_ranking_info_packet = 
            static_cast<MSG_CG_ETC_GUILDINFO_ACK*>(recv_packet);

        if (guild_ranking_info_packet->guid_ == 0)
        {
            return;
        }

        GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
            GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();

        GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
            ranking_guild_info_container.find(guild_ranking_info_packet->guid_);
        if (found_itr == ranking_guild_info_container.end())
        {
            GuildSystem::GuildSystemData::GuildInfoForClient guild_info(
                guild_ranking_info_packet->game_guild_info_, 
                guild_ranking_info_packet->guild_info_extra_);

            ranking_guild_info_container.insert(
                GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::value_type(
                    guild_ranking_info_packet->guid_, 
                    guild_info));
        }
        else
        {
            GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;

            guild_info.game_guild_info_ = guild_ranking_info_packet->game_guild_info_;
            guild_info.guild_info_extra_ = guild_ranking_info_packet->guild_info_extra_;
        }

        uiGuildInfoOther* guild_info_other = 
            GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
        if (guild_info_other != NULL)
        {
            guild_info_other->ShowInterface(TRUE);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            guild_info_other->SetJoinRequestButton(guild_ranking_info_packet->request_state_);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
    }

    void SEND_CW_GUILD_WAREHOUSE_LOGLIST_REQ()
    {
        MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ send_packet;
        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &send_packet, sizeof(send_packet));
    }

    void RECV_CW_GUILD_CHAT_PACKET( MSG_BASE* recv_packet )
    {
        uiBase* chat_dialog = GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG);
        if (chat_dialog != NULL)
        {
            chat_dialog->NetworkProc(recv_packet);
        }
    }

    void RECV_CW_GUILD_WAREHOUSE_LOGLIST_ANS( MSG_BASE* recv_packet )
    {
        MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS* warehouse_log_list = 
            static_cast<MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS*>(recv_packet);

        if (warehouse_log_list->m_byCurrentPage == 1)
        {
            GuildSystem::GetGuildSystemData().ClearWareHouseLog();
        }

        GuildSystem::GetGuildSystemData().AddWareHouseLog(
            &(warehouse_log_list->m_LogList[0]), warehouse_log_list->m_byLogCount);
        
        uiGuildLog* ui_guild_log = GET_CAST_DIALOG(uiGuildLog, uiGuildLog::kDialog_ID);
        if (ui_guild_log != NULL)
        {
            ui_guild_log->UpdateGuildWareHouseLog();
        }
    }

    void RECV_CW_GUILD_WAREHOUSE_LOGLIST_NAK( MSG_BASE* recv_packet )
    {
        MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK* nak_packet = 
            static_cast<MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK*>(recv_packet);
        
        // 기존 로그 에러코드처리가 주석처리되어 있음(주석처리가 되어 있는 이유 확인 필요)
        //GuildSystem::Function::ProcessErrorCode(nak_packet->m_byErrorCode);
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void RECV_CG_GUILD_JOIN_REQUEST_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_JOIN_REQUEST_ACK* recv_message = 
            static_cast<MSG_CG_GUILD_JOIN_REQUEST_ACK*>(recv_packet);

        // 메시지 박스
        if (recv_message->result_ == RC::RC_GUILD_SUCCESS)
        {
            TCHAR guild_name[MAX_GUILDNAME_LENGTH+1] = {0, };
            if (GuildSystem::GetGuildSystemData().get_guildname_for_guild_join_request(recv_message->guild_guid, guild_name))
            {                
                TCHAR format_message[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(7298, format_message);
                Snprintf(result_message, INTERFACE_STRING_LENGTH, format_message, guild_name, guild_name);
                GlobalFunc::VerifyMessage(result_message, "");                

                uiGuildInfoOther* guild_info_other = 
                    GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
                if (guild_info_other != NULL)
                {
                    guild_info_other->SetJoinRequestButton(eGUILD_JOIN_REQUEST_CANCEL);
                }
            }
            else
            {
                GlobalFunc::VerifyMessage("Unkown GuildName", "");
            }
        }
        else
        {
            // 상황에 따른 처리를 해야한다.
            /*
            RC_GUILD_JOIN_REQUEST_FULL_LIST,                //가입 요청자 목록이 초과되었다.
            RC_GUILD_JOIN_REQUEST_ALREADY_EXIST,            //이미 가입 요청자 목록에 존재한다.
            RC_GUILD_JOIN_REQUEST_NOT_EXIST,                //가입 요청자 목록에 존재하지않는다.
            RC_GUILD_JOIN_REQUEST_GUILD_EXIST,              //요청자가 이미 길드를 가지고 있다.
            RC_GUILD_JOIN_REQUEST_INVALID_GUILD,            //길드를 찾을 수 없다.
            */
            int string_index = 0;
            TCHAR guild_name[MAX_GUILDNAME_LENGTH+1] = {0, };
            TCHAR format_message[INTERFACE_STRING_LENGTH] = {0, };
            TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
            if (GuildSystem::GetGuildSystemData().get_guildname_for_guild_join_request(recv_message->guild_guid, guild_name))
            {                
                g_InterfaceManager.GetInterfaceString(7298, format_message);
            }
            switch (recv_message->result_)
            {
            case RC::RC_GUILD_JOIN_REQUEST_GUILD_EXIST:
                {
                    // 7103 : 이미 다른 길드에 가입을 하였습니다.
                    string_index = 7103;
                    g_InterfaceManager.GetInterfaceString(string_index, result_message);
                }
                break;
            case RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD:
                {
                    // 7300 : 길드정보가 없습니다.
                    string_index = 7300;
                    g_InterfaceManager.GetInterfaceString(string_index, result_message);
                }
                break;
            case RC::RC_GUILD_JOIN_REQUEST_ALREADY_EXIST:
                {
                    // 7107 : 이미 가입요청을 하였습니다.
                    string_index = 7107;
                    g_InterfaceManager.GetInterfaceString(string_index, result_message);                    
                }
                break;
            case RC::RC_GUILD_JOIN_REQUEST_FULL_LIST:
                {
                    // 7301 : %s 길드에 가입 가능한 길드원의 수가 초과되어 길드가입 요청을 할 수 없습니다.
                    string_index = 7301;
                    g_InterfaceManager.GetInterfaceString(string_index, format_message);
                    Snprintf(result_message, INTERFACE_STRING_LENGTH, format_message, guild_name);
                }
                break;
            case RC::RC_GUILD_JOIN_REQUEST_PLAYER_MAX:
                {
                    // 요청 5번 넘어가면 온다 hsh2303 스트링 테이블 요청 해야함.
                    string_index = 7303;
                    g_InterfaceManager.GetInterfaceString(string_index, format_message);
                    Snprintf(result_message, INTERFACE_STRING_LENGTH, format_message, "5");
                }
                break;
            }
            if (string_index != 0)
            {
                GlobalFunc::VerifyMessage(result_message, "");
            }
        }
    }

    void RECV_CG_GUILD_JOIN_REQUEST_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_JOIN_REQUEST_BRD* recv_message = 
            static_cast<MSG_CG_GUILD_JOIN_REQUEST_BRD*>(recv_packet);

        /*
        GUILD_JOIN_REQUEST_INFO request_info;
        eGUILD_JOIN_REQUEST     request_type;
        */
        GuildSystem::Function::Process_Guild_Join_Request(eGUILD_JOIN_REQUEST_OK, 
            recv_message->request_info);
    }

    void RECV_CG_GUILD_JOIN_REQUEST_REJECT_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_JOIN_REQUEST_REJECT_ACK* recv_message = 
            static_cast<MSG_CG_GUILD_JOIN_REQUEST_REJECT_ACK*>(recv_packet);

        TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR guild_name[MAX_GUILDNAME_LENGTH+1] = {0, };
        _tcsncpy(guild_name, recv_message->guild_name, _countof(recv_message->guild_name));
        switch (recv_message->result_)
        {
        case RC::RC_GUILD_DONT_HAVE_RIGHTS_STATE:
            {
                // 6271 : 해당 기능을 이용할 권한이 없습니다.
                g_InterfaceManager.GetInterfaceString(6271, result_message);                
            }
            break;
        case RC::RC_GUILD_NOT_EXIST_HOST_STATE:
            {
                // 7106 : 대상이 가입요청 목록에 없습니다.
                g_InterfaceManager.GetInterfaceString(7106, result_message);
            }
            break;
        case RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD:
            {
                // 7300 : 길드정보가 없습니다.
                g_InterfaceManager.GetInterfaceString(7300, result_message);                
            }
            break;
        case RC::RC_GUILD_JOIN_REQUEST_REJECT:
            {
                // 7104 : 길드 가입을 거절하였습니다.
                g_InterfaceManager.GetInterfaceString(7104, result_message);                
            }
            break;
        case RC::RC_GUILD_JOIN_REQUEST_OVER_WAIT_TIME:
            {
                // 7105 : %s 길드가입에 실패하였습니다.
                g_InterfaceManager.GetInterfaceString(7105, format_string);
                Snprintf(result_message, INTERFACE_STRING_LENGTH, format_string, guild_name);
            }
            break;
        }

        if (Strcmp(result_message, _T("")) != 0)
        {
            GlobalFunc::VerifyMessage(result_message, "");
        }

        uiGuildInfoOther* guild_info_other = 
            GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
        if (guild_info_other != NULL)
        {
            // 길드가입 거절을 해서 다시 요청상태 버튼으로 바꿔준다.
            guild_info_other->SetJoinRequestButton(eGUILD_JOIN_REQUEST_OK);
        }
    }

    void RECV_CG_GUILD_JOIN_REQUEST_REJECT_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_JOIN_REQUEST_REJECT_BRD* recv_message = 
            static_cast<MSG_CG_GUILD_JOIN_REQUEST_REJECT_BRD*>(recv_packet);

        GuildSystem::Function::Delete_Element_Guild_Join_Request_List(recv_message->char_guid);
    }

    void RECV_CG_GUILD_FACILITY_UPGRADE_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_FACILITY_UPGRADE_ACK* recv_message = 
            static_cast<MSG_CG_GUILD_FACILITY_UPGRADE_ACK*>(recv_packet);

        GuildSystem::Function::ProcessGuildFacilityResultCode(recv_message->result);
    }

    void RECV_CG_GUILD_FACILITY_UPGRADE_BRD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_FACILITY_UPGRADE_BRD* recv_message = 
            static_cast<MSG_CG_GUILD_FACILITY_UPGRADE_BRD*>(recv_packet);
        GuildSystem::GetGuildSystemData().set_element_guild_facility(recv_message->facility_info);
        GuildSystem::Function::UpdateAllGuildInstitution();
        if (g_pHero)
        {
            g_pHero->UpdateSkillAttr();
        }
    }

    void RECV_CG_GUILD_FACILITY_ACTIVE_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_FACILITY_ACTIVE_ACK* recv_message = 
            static_cast<MSG_CG_GUILD_FACILITY_ACTIVE_ACK*>(recv_packet);
        
        if (recv_message->result == RC::RC_GUILD_SUCCESS)
        {
            GuildSystem::GetGuildSystemData().set_element_guild_facility_active(recv_message->facility_code, true);
        }
        else
        {
            GuildSystem::Function::ProcessGuildFacilityResultCode(recv_message->result);
        }
    }

    void RECV_CG_GUILD_FACILITY_TOTALINFO_ACK( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_FACILITY_TOTALINFO_ACK* recv_message = 
            static_cast<MSG_CG_GUILD_FACILITY_TOTALINFO_ACK*>(recv_packet);
        GuildSystem::GetGuildSystemData().set_guild_facility_container(recv_message->guild_facility_total_info);

    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    
#ifdef _NA_008334_20150608_SONNENSCHEIN
    void RECV_CG_GUILD_SONNENSCHEIN_UPDATE_CMD( MSG_BASE* recv_packet )
    {
        MSG_CG_GUILD_SONNENSCHEIN_UPDATE_CMD* recv_message = 
            static_cast<MSG_CG_GUILD_SONNENSCHEIN_UPDATE_CMD*>(recv_packet);

        Player* player = static_cast<Player*>(g_ObjectManager.GetObject(recv_message->m_dwPlayerKey));
        if (player != NULL)
        {
            if (g_pHero && (g_pHero->GetObjectKey() == recv_message->m_dwPlayerKey))
            {                        
                GuildSystem::GetGuildSystemData().set_sonnenschein_phase(recv_message->sonnenschein_phase);
                uiDominationTowerMan::RefreshZoneMarkInfo();
            }
            else
            {
                player->GetGuildRenderInfo().sonnenschein_phase = recv_message->sonnenschein_phase;
            }
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}; //namespace Function
}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL