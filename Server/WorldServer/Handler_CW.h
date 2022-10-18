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
    Handler_CW_DECL(CW_CHAT_CHANNEL_INVITE_NAK); // �ʴ� ����ڰ� ä�� �ʴ븦 �ź��ϰų� �������� �� ȣ��ȴ�.
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
    Handler_CW_DECL( CW_GROUP_ADD_SYN);         //�׷� ��� �߰� ��û
    Handler_CW_DECL( CW_GROUP_GROUPLIST_REQ);   //�׷� ����Ʈ ��û
    Handler_CW_DECL( CW_GROUP_ADD_RESPONSE_ACK);//�׷� ��� �߰� ��û ���� ����
    Handler_CW_DECL( CW_GROUP_ADD_RESPONSE_NAK);//�׷� ��� �߰� ��û ���� ����
    Handler_CW_DECL( CW_GROUP_STATUS_REQ );     //�׷� ��� �� ���� ��û
    Handler_CW_DECL( CW_GROUP_DEL_SYN );        //�׷� ��� ���� ��û
    Handler_CW_DECL( CW_GROUP_ADDBLOCK_SYN );   //�׷� �ɹ� �߰� �ź� ��� ��û
    Handler_CW_DECL( CW_GROUP_RESTOR_CMD );     //�׷� ���� ���û
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_CW'
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    Handler_CW_DECL(CW_LOTTO_INFO_SYN);         // (��) ���� �ζ� ���� ��û
    Handler_CW_DECL(CW_LOTTO_ENTRY_SYN);        // ���� �ζ� ���� ��û
    Handler_CW_DECL(CW_LOTTO_NOTICE_LIST_SYN);  // ��÷ �ȳ� ���� ��� ��û
    Handler_CW_DECL(CW_LOTTO_NOTICE_CONFIRM);   // ��÷ �ȳ� ���� ���� Ȯ��
    //}
    Handler_CW_DECL(CW_DOMINATION_RETURN_HEIM_CMD);
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    Handler_CW_DECL(CW_PARTY_FINDING_REGISTER_SYN); // ��Ƽ ���� ���/������ ��û�Ǹ� ȣ��ȴ�.
    Handler_CW_DECL(CW_PARTY_FINDING_GET_LIST_SYN); // ��Ƽ ���� ����� ��û�Ǹ� ȣ��ȴ�.
    Handler_CW_DECL(CW_PARTY_FINDING_JOIN_REQUEST_SYN); // ��Ƽ ���� ���� ��û�� ��û�Ǹ� ȣ��ȴ�.
    Handler_CW_DECL(CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK); // ��Ƽ�����κ��� ��Ƽ ���� ���� ��û�� ���� ������ �����ϸ� ȣ��ȴ�.
    Handler_CW_DECL(CW_PARTY_FINDING_CANCEL_SYN); // ��Ƽ ���� ��Ұ� ��û�Ǹ� ȣ��ȴ�.
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    Handler_CW_DECL(CW_NOTIFICATION_PREV_SYN);  // ���� �˸� �޼��� ��û
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

    static GMCmdManager	m_GMCmdManager;
};
