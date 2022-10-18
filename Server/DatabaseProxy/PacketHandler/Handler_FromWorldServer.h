#pragma once

#include "./PacketHandler.h"

#define Handler_FromWorldServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromWorldServer_IMPL( p )	VOID Handler_FromWorldServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;

class Handler_FromWorldServer
{
public:
	Handler_FromWorldServer(void){}
	~Handler_FromWorldServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_FromWorldServer_DECL(DW_CONNECTION_GROUND_CMD);
	Handler_FromWorldServer_DECL( DW_FRIEND_GETLIST_SYN );			//< ģ�� ���
	Handler_FromWorldServer_DECL( DW_FRIEND_GETLIST_DBR );			//< ģ�� ���
	Handler_FromWorldServer_DECL( DW_FRIEND_ADD_SYN );				//< A->B add friend
	Handler_FromWorldServer_DECL( DW_FRIEND_ADD_DBR );				//< A->B add friend
	Handler_FromWorldServer_DECL( DW_FRIEND_DEL_SYN );				//< ģ�� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_DEL_DBR );				//< ģ�� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_SYN );			//< ���� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_DBR );			//< ���� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_FREE_SYN );		//< ���� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_FREE_DBR );		//< ���� ����
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
	Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_FREE_CMD );		//< ģ�� ��õ���� �Ϲ�ģ���� ����
	Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_FREE_DBR );		//< ģ�� ��õ���� �Ϲ�ģ���� ����    
    Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD );		//< ���� ������ ����
    Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_DBR );		//< ���� ������ ����
    //}
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    Handler_FromWorldServer_DECL( DW_FRIEND_DETAIL_INFO_SYN );      // �� ����â ���� ��û(�޸�,���) 
    Handler_FromWorldServer_DECL( DW_FRIEND_DATAIL_INFO_DBR );      // �� ����â ���� ��û(�޸�,���) 
    Handler_FromWorldServer_DECL( DW_FRIEND_MEMO_UPDATE_SYN );      // �� ����â (�޸�) 
    Handler_FromWorldServer_DECL( DW_FRIEND_MEMO_UPDATE_DBR );      // �� ����â (�޸�) 

    Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_SYN );			//< ä�ñ���
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_DBR );			//< ä�ñ���
	Handler_FromWorldServer_DECL( DW_GM_CHATALLOW_SYN );			//< ä�����
	Handler_FromWorldServer_DECL( DW_GM_CHATALLOW_DBR );			//< ä�����
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_UPDATE_SYN );		//< ä�ñ��� �ð� ����
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_UPDATE_DBR );		//< ä�ñ��� �ð� ����
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCKTIME_REQ );		//< ä�ñ��� �ð� ��û
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCKTIME_DBR );		//< ä�ñ��� �ð� ��û
	Handler_FromWorldServer_DECL( DW_CHAO_ADD_REVENGE_CHAR_SYN );	//< Add Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_ADD_REVENGE_CHAR_DBR );	//< Add Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_REMOVE_REVENGE_CHAR_SYN );	//< Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_REMOVE_REVENGE_CHAR_DBR );	//< Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_PK_REVENGE_CHAR_SYN );	//< PK Success - Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_PK_REVENGE_CHAR_DBR );	//< PK Success - Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_MEMO_SEND_SYN );				//< ���� ����
	Handler_FromWorldServer_DECL( DW_MEMO_SEND_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_LIST_REQ );				//< ���� ��� ��ȸ
	Handler_FromWorldServer_DECL( DW_MEMO_LIST_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_VIEW_REQ );				//< ���� Ȯ��
	Handler_FromWorldServer_DECL( DW_MEMO_VIEW_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_DELETE_SYN );				//< ���� ����
	Handler_FromWorldServer_DECL( DW_MEMO_DELETE_DBR );
	Handler_FromWorldServer_DECL( DW_STATUS_INTERCEPTION_REQ );
	Handler_FromWorldServer_DECL( DW_STATUS_INTERCEPTION_DBR );

	Handler_FromWorldServer_DECL(DW_GUILD_WAREHOUSE_LOGLIST_REQ);
	Handler_FromWorldServer_DECL(DW_GUILD_WAREHOUSE_LOGLIST_DBR);

	Handler_FromWorldServer_DECL(DW_GUILD_SELECT_NOTICE_SYN);
	Handler_FromWorldServer_DECL(DW_GUILD_SELECT_NOTICE_DBR);

    Handler_FromWorldServer_DECL(DW_DOMINATION_FINDMONEY_SYN);
    Handler_FromWorldServer_DECL(DW_DOMINATION_FINDMONEY_DBR);
//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	
	Handler_FromWorldServer_DECL(DW_COMMUNITY_MYLIST_SYN);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_MYLIST_DBR);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_INFO_SYN);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_INFO_DBR);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_CONTACT_ME_DBR);
//#endif
    //{__NA001254_20090109_RTTG_FUNC_ADD
	Handler_FromWorldServer_DECL(DW_MULTIMEMO_SEND_SYN);
	Handler_FromWorldServer_DECL(DW_MULTIMEMO_SEND_DBR);
    //}
    // __KR_001355_20090608_GROUPINGSYSTEM_DW = { //�׷�α� �ý���
	Handler_FromWorldServer_DECL(DW_GROUP_ADD_SYN); //�׷� �ɹ� �߰� ��û 
	Handler_FromWorldServer_DECL(DW_GROUP_ADD_DBR); //�׷� �ɹ� �߰� DB�۾� ���

	Handler_FromWorldServer_DECL(DW_GROUP_DEL_SYN); //�׷� �ɹ� ���� ��û 
	Handler_FromWorldServer_DECL(DW_GROUP_DEL_DBR); //�׷� �ɹ� ���� DB�۾� ���

	Handler_FromWorldServer_DECL(DW_GROUP_LIST_REQ); //�׷� �ɹ� ����Ʈ ��û 
	Handler_FromWorldServer_DECL(DW_GROUP_LIST_DBR); //�׷� �ɹ� ����Ʈ DB�۾� ���

    Handler_FromWorldServer_DECL(DW_GROUP_ACCUMEXP_CMD); //���� �ɹ� ����ġ ���� ��û
    Handler_FromWorldServer_DECL(DW_GROUP_ACCUMEXP_DBR); //���� �ɹ� ����ġ ���� DB�۾� ���

    Handler_FromWorldServer_DECL(DW_GROUP_INIT_ACCUMEXP_CMD); //���� �ɹ� ����ġ ���� ��û
    Handler_FromWorldServer_DECL(DW_GROUP_INIT_ACCUMEXP_DBR); //���� �ɹ� ����ġ ���� DB�۾� ���

    Handler_FromWorldServer_DECL(DW_GROUP_SET_MEMBERINFO_CMD); //�׷� ���� ����
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DW'

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_ENTRY_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_ENTRY_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_WINNER_LIST_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_WINNER_LIST_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_SAVE_ENTRY_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_SAVE_ENTRY_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ROLLOVER_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_ROLLOVER_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ACCUM_NTF);
    Handler_FromWorldServer_DECL(DW_LOTTO_ACCUM_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ADD_TICKET_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_ADD_TICKET_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_REWARD_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_REWARD_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_LIST_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_LIST_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_DEL_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_DEL_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_PAYMENT_UPDATE_NTF);
    Handler_FromWorldServer_DECL(DW_LOTTO_PAYMENT_UPDATE_DBR);
    //}
};
