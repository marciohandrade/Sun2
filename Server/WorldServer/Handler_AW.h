#pragma once

struct MSG_BASE;
class AgentServerSession;
class GMCmdManager;

class Handler_AW
{
public:
	static VOID	OnAW_AGENTSERVER_INFO_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID	OnAW_PREPARE_NEW_USER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_SET_CHARNAME_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_UNSET_CHARNAME_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_ENTER_ZONE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_LEAVE_ZONE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_LOGOUT( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_RESTORE_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_PARTY_MAKE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_PARTY_JOIN_MEMBER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_PARTY_WITHDRAW_MEMBER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_PARTY_DESTROY_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // 파티장이 변경되면 호출된다.
    static VOID OnAW_PARTY_CHANGE_MASTER_SYN(AgentServerSession* agent_server, 
                                             MSG_BASE* receive_msg, 
                                             WORD receive_msg_size);
    
    // 파티 모집에서 파티 참가 신청에 대한 응답이 오면 호출된다.
    static VOID OnAW_PARTY_FINDING_JOIN_RESULT_ACK(AgentServerSession* agent_server, 
                                                   MSG_BASE* receive_msg, 
                                                   WORD receive_msg_size);
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

//	static VOID OnAW_USERCOUNT_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
//	static VOID OnAW_USERMONEY_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_GM_WHISPER_SET_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_GM_USERCHAT_BLOCK_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_USER_LEVELUP_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_CHAO_REQUEST_REVENGEINFO_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_CHAO_REMOVE_REVENGE_CHAR_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_CHAO_ADD_REVENGE_CHAR_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize );
	static VOID OnAW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD(AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize);
	static VOID OnAW_ITEM_SPECIAL_ITEM_CLEAR_CMD(AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize);
	static VOID OnAW_ITEM_USE_DECREE_ITEM_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
	static VOID OnAW_ITEM_USE_DECREE_ITEM_NAK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    static VOID OnAW_ITEM_USE_HEIM_LOTTO_TICKET_SYN(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#endif

	static VOID OnAW_USER_STATUS_INTERCEPTION_SYN(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
	static VOID OnAW_OPERATION_ROOM_CTRL_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
	static VOID OnAW_GM_NOTICE_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    static VOID OnAW_GM_LOTTO_NUM_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    static VOID OnAW_GM_LOTTO_SETTLE_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    static VOID OnAW_GM_LOTTO_FUND_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //}
    static VOID OnAW_STATUS_GROUP_EXP_ACQUISITION_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
    static VOID OnAW_USER_STATUS_HEIM_ACQUISITION_NTF(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
    static VOID OnAW_USER_STATUS_HEIM_CONSUMPTION_NTF(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //__KR_001355_20090608_GROUPINGSYSTEM_GW
    static VOID OnAW_STATUS_GROUP_START_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    static VOID OnAW_USER_LOTTO_REWARD_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    static VOID OnAW_USER_LOTTO_TICKET_PURCHASE_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    //}
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    static void OnAW_USER_STATUS_PCBANG_BILLING_NTF(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#endif
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    static void OnAW_USER_SYSTEMMEMO_SEND_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    static void OnAW_FRIEND_INVITATIONR_INSERT_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize);
#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    static void OnAW_PARTY_ROOM_KEY_SYN(AgentServerSession* agentServer_session, MSG_BASE* message, WORD message_size);
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

    static GMCmdManager	m_GMCmdManager;
};
