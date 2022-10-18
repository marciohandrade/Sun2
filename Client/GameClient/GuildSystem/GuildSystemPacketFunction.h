#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiSystemMan/uiSystemMessage.h"

namespace GuildSystem
{
    namespace PacketFunction
    {
        // send packet function
        void SEND_CG_GUILD_CREATE_SYN(const TCHAR* guild_name);

        void SEND_CG_GUILD_DESTROY_SYN();

        void SEND_CG_GUILD_RECOVERY_GUILD_SYN();

        void SEND_CG_GUILD_GUILDMARK_REGISTER_SYN(DWORD background_index, 
                                                  DWORD pattern_index, 
                                                  POSTYPE slot_position);

        void SEND_CG_GUILD_GUILDMARK_DELETE_SYN();

        void SEND_CG_GUILD_TOTALINFO_REQ_SYN();

        void SEND_CG_GUILD_RELATION_SYN(eGUILD_RELATION relation_type, const TCHAR* guild_name);
        void SEND_CG_GUILD_RELATION_DELETE_SYN(eGUILD_RELATION relation_type, const TCHAR* guild_name);
        void SEND_CG_GUILD_RELATION_BOTH_AGAINST_SYN(const TCHAR* guild_name);
        void SEND_CG_GUILD_RELATION_ANSWER_SYN(eGUILD_ANSWER anser_type, 
            eGUILD_RELATION relation_type, 
            CHARGUID request_player_key);

        void SEND_CG_GUILD_INVITE_SYN(const TCHAR* invite_name);
        void SEND_CG_GUILD_INVITE_ANSWER_SYN(bool invite_answer, CHARGUID request_player_key);
        void SEND_CG_GUILD_KICKPLAYER_SYN();
        void SEND_CG_GUILD_WITHDRAW_SYN();
        void SEND_CG_GUILD_CHANGE_MASTER_SYN();

        void SEND_CG_GUILD_CHANGE_DUTY_SYN(GUILD_MEMBER_INFO* member_info, BYTE change_duty);

        void SEND_CG_GUILD_NAME_CHANGE_SYN(const TCHAR* guild_rename, POSTYPE item_position);

        void SEND_CG_GUILD_IN_GAME_GUILD_LOG_LIST_REQ();

        // 길드 랭킹
        void SEND_CG_ETC_GUILD_RANKINGINFO_SYN();
        void SEND_CG_ETC_GUILDINFO_SYN(GUILDGUID guild_guid);

        void SEND_CW_FRIEND_STATUS_REQ(const TCHAR* member_name, CHARGUID member_guid);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        void SEND_CG_GUILD_JOIN_REQUEST_APPROVE_SYN(MSG_CG_GUILD_JOIN_REQUEST_APPROVE_SYN& send_packet);
        void SEND_CG_GUILD_JOIN_REQUEST_REJECT_SYN(MSG_CG_GUILD_JOIN_REQUEST_REJECT_SYN& send_packet);
        void SEND_CG_GUILD_JOIN_REQUEST_SYN(MSG_CG_GUILD_JOIN_REQUEST_SYN& send_packet); // 길드 가입, 취소 요청

        void SEND_CG_GUILD_FACILITY_UPGRADE_SYN(const FCODE& facility_code); // 길드 시설 업그레이드 요청
        void SEND_CG_GUILD_FACILITY_ACTIVE_SYN(const FCODE& facility_code);  // 길드 시설 액티브 요청
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        // recv packet function
        // 길드 생성
        void RECV_CG_GUILD_CREATE_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_CREATE_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_CREATE_NAK(MSG_BASE* recv_packet);
        // 길드원 로그인 정보
        void RECV_CG_GUILD_LOGIN_CHAR_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_LOGIN_CHAR_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_LOGOUT_CHAR_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_CHAR_LEVELUP_BRD(MSG_BASE* recv_packet);
        // 길드원 초대
        void RECV_CG_GUILD_JOIN_FAILED_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_INVITE_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_INVITE_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_INVITE_ANSWER_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_INVITE_ANSWER_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_JOIN_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_JOIN_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_JOIN_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_JOIN_CMD(MSG_BASE* recv_packet);
        // 길드원 탈퇴/강퇴
        void RECV_CG_GUILD_WITHDRAW_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_WITHDRAW_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_WITHDRAW_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_KICKPLAYER_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_KICKPLAYER_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_KICKPLAYER_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_LEAVE_BRD(MSG_BASE* recv_packet);
        // 길드원 직책변경
        void RECV_CG_GUILD_CHANGE_DUTY_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_CHANGE_DUTY_BRD(MSG_BASE* recv_packet);
        // 길드레벨업
        void RECV_CG_GUILD_RANKUP_BRD(MSG_BASE* recv_packet);
        // 길드관계 정보
        void RECV_CG_GUILD_ADD_INFO_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_ADD_INFO_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_REQUEST_CMD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_REJECT_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_DELETE_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_DELETE_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_BOTH_AGAINST_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RELATION_MOVE_BRD(MSG_BASE* recv_packet);

        // 길드 종합정보
        void RECV_CG_GUILD_TOTALINFO_REQ_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_TOTALINFO_REQ_NAK(MSG_BASE* recv_packet);
        // 카오 상태 변경
        void RECV_CG_GUILD_CHANGE_CHAOSTATE_BRD(MSG_BASE* recv_packet);
        // 길드 마크
        void RECV_CG_GUILD_GUILDMARK_REGISTER_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_REGISTER_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_REGISTER_SECTOR_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_REGISTER_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_DELETE_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_DELETE_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_DELETE_SECTOR_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_GUILDMARK_DELETE_NAK(MSG_BASE* recv_packet);
        // 길드마스터 변경
        void RECV_CG_GUILD_CHANGE_MASTER_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_CHANGE_MASTER_NAK(MSG_BASE* recv_packet);
        // 길드명 변경
        void RECV_CG_GUILD_NAME_CHANGE_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_NAME_CHANGE_SECTOR_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_NAME_CHANGE_RELATION_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_NAME_CHANGE_NAK(MSG_BASE* recv_packet);
        // 길드 해산
        void RECV_CG_GUILD_DESTROY_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_DESTROY_NAK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_DESTROY_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_DESTROY_WAIT_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_DESTROY_WAIT_BRD(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RECOVERY_GUILD_ACK(MSG_BASE* recv_packet);
        void RECV_CG_GUILD_RECOVERY_GUILD_NAK(MSG_BASE* recv_packet);
        // 길드 로그
        void RECV_CG_GUILD_IN_GAME_GUILD_LOG_LIST_ANS(MSG_BASE* recv_packet);
        // 길드 랭크
        void RECV_CG_ETC_GUILD_RANKINGINFO_ACK(MSG_BASE* recv_packet);
        void RECV_CG_CG_ETC_GUILDINFO_ACK(MSG_BASE* recv_packet);

        // SEND_CW_GUILD
        void SEND_CW_GUILD_WAREHOUSE_LOGLIST_REQ();

        // RECV_CW_GUILD
        void RECV_CW_GUILD_CHAT_PACKET(MSG_BASE* recv_packet);
        void RECV_CW_GUILD_WAREHOUSE_LOGLIST_ANS(MSG_BASE* recv_packet);
        void RECV_CW_GUILD_WAREHOUSE_LOGLIST_NAK(MSG_BASE* recv_packet);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        void RECV_CG_GUILD_JOIN_REQUEST_ACK(MSG_BASE* recv_packet); // 길드 가입 요청 결과
        void RECV_CG_GUILD_JOIN_REQUEST_BRD(MSG_BASE* recv_packet); // 길드 가입 BRD        
        void RECV_CG_GUILD_JOIN_REQUEST_REJECT_ACK(MSG_BASE* recv_packet); // 길드 가입 거절 실패
        void RECV_CG_GUILD_JOIN_REQUEST_REJECT_BRD(MSG_BASE* recv_packet); // 길드 요청 리스트에서 삭제

        void RECV_CG_GUILD_FACILITY_TOTALINFO_ACK(MSG_BASE* recv_packet); // 길드 시설에 관한 패킷(서버에서 알아서 보내줍니다)
        void RECV_CG_GUILD_FACILITY_UPGRADE_ACK(MSG_BASE* recv_packet); // 길드 시설 업그레이드 실패
        void RECV_CG_GUILD_FACILITY_UPGRADE_BRD(MSG_BASE* recv_packet); // 길드 시설 업그레이드 성공

        void RECV_CG_GUILD_FACILITY_ACTIVE_ACK(MSG_BASE* recv_packet);  // 길드 시설 액티브
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
        void RECV_CG_GUILD_SONNENSCHEIN_UPDATE_CMD(MSG_BASE* recv_packet);
#endif //_NA_008334_20150608_SONNENSCHEIN

    }; //namespace PacketFunction

}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL