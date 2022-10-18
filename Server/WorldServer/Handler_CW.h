#pragma once

#include <windows.h>

struct MSG_BASE;
class User;
class GMCmdManager;
class GroupManager;

//---------------------------------------------------------------------------------------------------------------------//
#define Handler_CW_DECL( p )		static VOID On##p( User* pUser, MSG_BASE* pMsg, WORD wSize )
#define Handler_CW_IMPL( p )		VOID	Handler_CW::On##p( User* pUser, MSG_BASE* pMsg, WORD wSize )
//---------------------------------------------------------------------------------------------------------------------//

class Handler_CW
{
public:
	Handler_CW_DECL( CW_HEARTBEAT );
	Handler_CW_DECL( CW_WHISPER_SYN );
	Handler_CW_DECL( CW_NOTICE_SYN );
	Handler_CW_DECL( CW_WHISPER_SET_SYN );
	Handler_CW_DECL( CW_CHAT_NORMAL_SYN );
	Handler_CW_DECL( CW_CHAT_SHOUT_SYN );
	Handler_CW_DECL( CW_GM_NOTICE_SYN );
	Handler_CW_DECL( CW_GM_STRING_CMD_SYN );
	Handler_CW_DECL( CW_VIEWPORT_CHARSTATE );
	Handler_CW_DECL( CW_ARMORKIT_FINDHACK_SYN );
	Handler_CW_DECL( CW_FRIEND_NEWLIST_SYN );
	Handler_CW_DECL( CW_FRIEND_ADD_SYN );
	Handler_CW_DECL( CW_FRIEND_OK_SYN );
	Handler_CW_DECL( CW_FRIEND_NO_SYN );
	Handler_CW_DECL( CW_FRIEND_DEL_SYN );
	Handler_CW_DECL( CW_FRIEND_BLOCK_SYN );
	Handler_CW_DECL( CW_FRIEND_BLOCK_FREE_SYN );
	Handler_CW_DECL( CW_FRIEND_CHAT_SYN );
    Handler_CW_DECL( CW_FRIEND_CHATBLOCK_SYN );
    Handler_CW_DECL( CW_FRIEND_MEMO_UPDATE_SYN );   //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
	Handler_CW_DECL( CW_CHAT_PARTY_SYN );
	Handler_CW_DECL( CW_CRYPTOGRAPHY_ACK );
	Handler_CW_DECL( CW_CRYPTOGRAPHY_NAK );
	Handler_CW_DECL( CW_CHAT_VOICE_REQUEST_SYN );
	Handler_CW_DECL( CW_CHAT_VOICE_ACCEPT_SYN );
	Handler_CW_DECL( CW_CHAT_VOICE_REJECT_SYN );
	Handler_CW_DECL( CW_CHAT_VOICE_DISCONNECT_SYN );
	Handler_CW_DECL( CW_CHAT_VOICE_MIC_STATUS_SYN );
	Handler_CW_DECL( CW_CHAT_PARTY_MIC_STATUS_REQ );
	Handler_CW_DECL( CW_CHAT_PLAYER_MIC_STATUS_REQ );
	Handler_CW_DECL( CW_COMM_USERID_REQ );
	Handler_CW_DECL( CW_CHAT_CHANNEL_JOIN_SYN );
	Handler_CW_DECL( CW_CHAT_CHANNEL_SEND_SYN );
	Handler_CW_DECL( CW_CHAT_CHANNEL_INVITE_SYN );
    //_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    Handler_CW_DECL(CW_CHAT_CHANNEL_INVITE_NAK); // 초대 대상자가 채널 초대를 거부하거나 차단했을 때 호출된다.
    Handler_CW_DECL( CW_CHAT_CHANNEL_OUT_SYN );
	Handler_CW_DECL( CW_CHAT_CHANNEL_INFO_REQ );
	Handler_CW_DECL( CW_FRIEND_STATUS_REQ );
	Handler_CW_DECL( CW_FRIEND_STATUS_JOIN_SYN );
	Handler_CW_DECL( CW_REVENGE_DEL_SYN );
	Handler_CW_DECL( CW_MEMO_SEND_SYN );
	Handler_CW_DECL( CW_MEMO_LIST_REQ );
	Handler_CW_DECL( CW_MEMO_VIEW_REQ );
	Handler_CW_DECL( CW_MEMO_DELETE_SYN );

	//Guild
	Handler_CW_DECL( CW_GUILD_CHAT_SYN );
	Handler_CW_DECL( CW_GUILD_WAREHOUSE_LOGLIST_REQ);
	Handler_CW_DECL( CW_GUILD_SELECT_NOTICE_SYN);
	Handler_CW_DECL( CW_GUILD_CHANGE_NOTICE_SYN);

    // implemented by __KR_001355_20090608_GROUPINGSYSTEM_CW = {
    Handler_CW_DECL( CW_GROUP_ADD_SYN);         //그룹 멤버 추가 요청
    Handler_CW_DECL( CW_GROUP_GROUPLIST_REQ);   //그룹 리스트 요청
    Handler_CW_DECL( CW_GROUP_ADD_RESPONSE_ACK);//그룹 멤버 추가 요청 성공 응답
    Handler_CW_DECL( CW_GROUP_ADD_RESPONSE_NAK);//그룹 멤버 추가 요청 실패 응답
    Handler_CW_DECL( CW_GROUP_STATUS_REQ );     //그룹 멤버 상세 정보 요청
    Handler_CW_DECL( CW_GROUP_DEL_SYN );        //그룹 멤버 삭제 요청
    Handler_CW_DECL( CW_GROUP_ADDBLOCK_SYN );   //그룹 맴버 추가 거부 토글 요청
    Handler_CW_DECL( CW_GROUP_RESTOR_CMD );     //그룹 정보 재요청
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_CW'
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    Handler_CW_DECL(CW_LOTTO_INFO_SYN);         // (내) 하임 로또 정보 요청
    Handler_CW_DECL(CW_LOTTO_ENTRY_SYN);        // 하임 로또 응모 요청
    Handler_CW_DECL(CW_LOTTO_NOTICE_LIST_SYN);  // 당첨 안내 쪽지 목록 요청
    Handler_CW_DECL(CW_LOTTO_NOTICE_CONFIRM);   // 당첨 안내 쪽지 읽음 확인
    //}
    Handler_CW_DECL(CW_DOMINATION_RETURN_HEIM_CMD);
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    Handler_CW_DECL(CW_PARTY_FINDING_REGISTER_SYN); // 파티 모집 등록/갱신이 요청되면 호출된다.
    Handler_CW_DECL(CW_PARTY_FINDING_GET_LIST_SYN); // 파티 모집 목록이 요청되면 호출된다.
    Handler_CW_DECL(CW_PARTY_FINDING_JOIN_REQUEST_SYN); // 파티 모집 참가 신청이 요청되면 호출된다.
    Handler_CW_DECL(CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK); // 파티장으로부터 파티 모집 참가 신청에 대한 응답이 도착하면 호출된다.
    Handler_CW_DECL(CW_PARTY_FINDING_CANCEL_SYN); // 파티 모집 취소가 요청되면 호출된다.
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    Handler_CW_DECL(CW_NOTIFICATION_PREV_SYN);  // 지난 알림 메세지 요청
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

    static GMCmdManager	m_GMCmdManager;
};
