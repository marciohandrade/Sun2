#pragma once

struct MSG_BASE;
class GameDBProxySession;

class Handler_DW
{
public:
	static VOID	OnDW_CONNECTION_SERVER_INFO_CMD( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID	OnDW_FRIEND_GETLIST_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_GETLIST_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_ADD_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_ADD_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_DEL_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_DEL_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_BLOCK_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_BLOCK_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_BLOCK_FREE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_FRIEND_BLOCK_FREE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    static VOID OnDW_FRIEND_DATAIL_INFO_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    static VOID OnDW_FRIEND_DETAIL_INFO_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    static VOID OnDW_FRIEND_MEMO_UPDATE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    static VOID OnDW_FRIEND_MEMO_UPDATE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );

	//채팅금지/허용
	static VOID OnDW_GM_CHATBLOCK_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATBLOCK_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATALLOW_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATALLOW_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATBLOCK_UPDATE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATBLOCKTIME_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GM_CHATBLOCKTIME_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_ADD_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_ADD_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_REMOVE_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_REMOVE_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_PK_REVENGE_CHAR_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_CHAO_PK_REVENGE_CHAR_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_SEND_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_SEND_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_LIST_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_LIST_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_VIEW_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_VIEW_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_DELETE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_MEMO_DELETE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_STATUS_INTERCEPTION_ANS( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_STATUS_INTERCEPTION_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );

	static VOID OnDW_GUILD_WAREHOUSE_LOGLIST_ANS(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
	static VOID OnDW_GUILD_WAREHOUSE_LOGLIST_NAK(GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize);

    static VOID OnDW_GUILD_SELECT_NOTICE_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GUILD_SELECT_NOTICE_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    //__NA001254_20090109_RTTG_FUNC_ADD
	static VOID OnDW_MULTIMEMO_SEND_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );

	static VOID OnDW_GROUP_ADD_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GROUP_ADD_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GROUP_DEL_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GROUP_DEL_NAK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
	static VOID OnDW_GROUP_LIST_ACK( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    static VOID OnDW_LOTTO_LOAD_ENTRY_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_LOAD_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_WINNER_LIST_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_SAVE_ENTRY_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_ROLLOVER_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_ADD_TICKET_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_REWARD_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_NOTICE_LIST_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    static VOID OnDW_LOTTO_NOTICE_DEL_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);
    //}

    static VOID OnDW_DOMINATION_FINDMONEY_ACK(GameDBProxySession* pGameDBProxy, MSG_BASE *pMsg, WORD wSize);

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    static VOID OnDW_NOTIFICATION_ITEM_SYN( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
    static VOID OnDW_NOTIFICATION_CLEAR_SYN( GameDBProxySession *pGameDBProxy, MSG_BASE *pMsg, WORD wSize );
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
};
