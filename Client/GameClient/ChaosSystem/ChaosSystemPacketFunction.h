#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

namespace ChaosSystem
{
    namespace PacketFunction
    {
        // 존 관련
        void RECV_CG_CHAOS_ZONE_ZONE_JOIN_ACK(MSG_BASE* recv_packet);

        // 전장 관련
        void SEND_CG_BATTLE_GROUND_RESERVATION_SYN(DWORD select_mapcode);    // 예약신청
        void SEND_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN();  // 예약취소
        void SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN(); // 부활대기 거부상태
        void SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN(); // 부활대기 거부 취소
        void SEND_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN(); // 전장 오픈타임 요청

        void RECV_CG_BATTLE_GROUND_RESERVATION_ACK(MSG_BASE* recv_message); // 예약 성공
        void RECV_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK(MSG_BASE* recv_message);  // 예약취소 성공
        void RECV_CG_BATTLE_GROUND_RESERVATION_BRD(MSG_BASE* recv_message);  // 대기 인원 BRD
        void RECV_CG_BATTLE_GROUND_MATCHING_RESULT_BRD(MSG_BASE* recv_message); // 매칭 성공
        //MSG_CG_BATTLE_GROUND_NON_NORMAL_EXIT_USER_REMOVE_SYN  //비정상 접속종료 시 유저 제거
        void RECV_CG_BATTLE_GROUND_TEAM_INFO_CMD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_TEAM_INFO_BRD(MSG_BASE* recv_message);

        void RECV_CG_BATTLE_GROUND_STATE_RESURECTION_CMD(MSG_BASE* recv_message); // 부활상태 명령(상태+시간)
        void RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(MSG_BASE* recv_message, bool is_show_message = true);
        void RECV_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_FINISH_RESULT_BRD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_TIME_INFO_CMD(MSG_BASE* recv_message);

        void RECV_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD(MSG_BASE* recv_message);

        // 전장 투표
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        void SEND_CG_VOTE(TCHAR* character_name); // 투표 발의
        void SEND_CG_BATTLE_GROUND_VOTE_SYN(bool vote_result);
        void RECV_CG_BATTLE_GROUND_INITIATE_VOTE_NAK(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_INITIATE_VOTE_BRD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_VOTE_STATUS_BRD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_VOTE_RESULT_BRD(MSG_BASE* recv_message);
        void RECV_CG_BATTLE_GROUND_VANP_PLAYER_CMD(MSG_BASE* recv_message);
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

        // 자유전투지역 관련
        void SEND_CG_FREE_PVP_ROOM_LIST_SYN();
        void SEND_CG_FREE_PVP_CREATE_ROOM_SYN(BaseFreePVPRoomInfo* base_room_info);
        void SEND_CG_FREE_PVP_JOIN_ROOM_SYN(KEYTYPE room_key, bool is_private_room, TCHAR* password);
        void SEND_CG_FREE_PVP_RESURRECTION_SYN();

        void RECV_CG_FREE_PVP_ROOM_LIST_ACK(MSG_BASE* recv_message);
        void RECV_CG_FREE_PVP_CREATE_ROOM_ACK(MSG_BASE* recv_message);
        void RECV_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD(MSG_BASE* recv_message);
        void RECV_CG_FREE_PVP_JOIN_ROOM_ACK(MSG_BASE* recv_message);

        // 골드 러쉬 관련
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void SEND_CG_GOLDRUSH_RESERVATION_SYN(DWORD select_map_code);
        void SEND_CG_GOLDRUSH_RESERVATION_CANCEL_SYN();  // 예약취소
        void RECV_CG_GOLDRUSH_RESERVATION_ACK(MSG_BASE* recv_message); // 예약 성공
        void RECV_CG_GOLDRUSH_RESERVATION_CANCEL_ACK(MSG_BASE* recv_message);  // 예약취소 성공
        void RECV_CG_GOLDRUSH_RESERVATION_BRD(MSG_BASE* recv_message);  // 대기 인원 BRD

        void RECV_CG_GOLDRUSH_TEAM_INFO_CMD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_TEAM_INFO_BRD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD(MSG_BASE* recv_message); // 시작시에 모든 팀에 팀 골드를 지급

        void RECV_CG_GOLDRUSH_STATE_RESURECTION_CMD(MSG_BASE* recv_message); // 부활상태 명령(상태+시간)

        void RECV_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_FINISH_RESULT_BRD(MSG_BASE* recv_message); // 골드러쉬승패여부
        void RECV_CG_GOLDRUSH_TIME_INFO_CMD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD(MSG_BASE* recv_message);

        // 골드러쉬 투표
        void SEND_CG_GOLDRUSH_VOTE(TCHAR* character_name); // 투표 발의
        void SEND_CG_GOLDRUSH_VOTE_SYN(bool vote_result);
        void RECV_CG_GOLDRUSH_INITIATE_VOTE_NAK(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_INITIATE_VOTE_BRD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_VOTE_STATUS_BRD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_VOTE_RESULT_BRD(MSG_BASE* recv_message);
        void RECV_CG_GOLDRUSH_VANP_PLAYER_CMD(MSG_BASE* recv_message);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH        

        // 소넨샤인 관련
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void SEND_CG_SONNENSCHEIN_CREATE_SYN();
        void RECV_CG_SONNENSCHEIN_CREATE_ACK(MSG_BASE* recv_message);
        void SEND_CG_SONNENSCHEIN_ENTER_CHECK_SYN(SLOTCODE need_item);
        void RECV_CG_SONNENSCHEIN_ENTER_CHECK_ACK(MSG_BASE* recv_message);
        void RECV_CG_SONNENSCHEIN_BEGIN_STAMP_BRD(MSG_BASE* recv_message);
        void RECV_CG_SONNENSCHEIN_CANCEL_STAMP_BRD(MSG_BASE* recv_message);
        void SEND_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN();
        void RECV_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK(MSG_BASE* recv_message);
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void SEND_CG_ONEDAYSGUILDMISSION_CREATE_SYN();
        void RECV_CG_ONEDAYSGUILDMISSION_CREATE_ACK(MSG_BASE* recv_message);
        void SEND_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(SLOTCODE need_item);
        void RECV_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK(MSG_BASE* recv_message);
        void RECV_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD(MSG_BASE* recv_message);
        void RECV_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD(MSG_BASE* recv_message);
        void SEND_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN();
        void RECV_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK(MSG_BASE* recv_message);
        void RECV_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD(MSG_BASE* recv_message);
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };
};


#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND