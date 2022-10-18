#pragma once

#include "Protocol.h"
#include "Protocol_CG.h"

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		A : Agent Server
//		G : Game Server
//		W : World Server
//
// * Postfix
//		SYN - Server�� ��û
//		ACK - ��û�� ���� ���
//		NAK - ��û�� ���� �ź�
//		CMD - Server�� ���
//		BRD - Server�� ����ȭ ���� ������ �˸�
//		DBR - DBProxy������ �����!
//
// * �������� Naming Rule
//		Prefix_ī�װ���_�������ݸ�_Postfix
//		ex) CW_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------

#if !defined(_NA000000_CW_PROTOCOL_UNIFICATION)
// Client <-> World Server														
enum eCW_CATEGORY														
{														
	CW_CONNECTION	= CATEGORY_GENERATOR_001,
	CW_CHAT			= CATEGORY_GENERATOR_002,							// ĳ���� ����		, ĳ���� ����
	CW_GM			= CATEGORY_GENERATOR_003,							// GM ��ɾ�
	CW_VIEWPORT		= CATEGORY_GENERATOR_004,							// ���� ����Ʈ ����
	CW_ARMORKIT		= CATEGORY_GENERATOR_005,							// ArmorKit ����
	CW_FRIEND		= CATEGORY_GENERATOR_006,							// ģ�� ����
	CW_GUILD		= CATEGORY_GENERATOR_007,							// ��� ����
	CW_CRYPTOGRAPHY	= CATEGORY_GENERATOR_009,							// ���� ����
	CW_COMMUNITY	= CATEGORY_GENERATOR_010,							// ��Ÿ Ŀ�´�Ƽ ����
	CW_REVENGE		= CATEGORY_GENERATOR_011,							// ������(����ģ��) ���� ����
	CW_MEMO			= CATEGORY_GENERATOR_012,							// ���� ����
	//CW_SYNC			= CATEGORY_GENERATOR_013,
    CW_GROUP		= CATEGORY_GENERATOR_014,							// �׷� �α� �ý��� ���� ��������
    CW_LOTTO        = CATEGORY_GENERATOR_015,                           // ���� �ζ� ����
    CW_PARTY        = CATEGORY_GENERATOR_016,                           // ��Ƽ ���� ��������
    CW_NOTIFICATION = CATEGORY_GENERATOR_017,                           // �˸��ý��� ���� ��������
};
#endif

enum eCW_CONNECTION_PROTOCOL
{
	CW_HEARTBEAT		= PROTOCOL_GENERATOR_001,
	CW_ENTER_SERVER_SYN		= PROTOCOL_GENERATOR_002,
	CW_ENTER_READY_BRD		= PROTOCOL_GENERATOR_003,							// ���� ���ڸ��� �˷��ش�
};														

enum eCW_CHAT_PROTOCOL														
{														
	CW_WHISPER_SYN		= PROTOCOL_GENERATOR_001,									// �ӼӸ�		
	CW_WHISPER_ACK		= PROTOCOL_GENERATOR_002,											
	CW_WHISPER_NAK		= PROTOCOL_GENERATOR_003,											
	CW_NOTICE_SYN		= PROTOCOL_GENERATOR_004,									// ����		
	CW_NOTICE_BRD		= PROTOCOL_GENERATOR_005,											
	CW_WHISPER_SET_SYN		= PROTOCOL_GENERATOR_006,								// �Ӹ� ��/��			
	CW_WHISPER_SET_ACK		= PROTOCOL_GENERATOR_007,											
	CW_WHISPER_SET_NAK		= PROTOCOL_GENERATOR_008,											
	CW_CHAT_NORMAL_SYN		= PROTOCOL_GENERATOR_009,								// �Ϲ� ä�� ��û			
	CW_CHAT_NORMAL_BRD		= PROTOCOL_GENERATOR_010,								// �Ϲ� ä�� ��ε� ĳ����			
	CW_CHAT_SHOUT_SYN		= PROTOCOL_GENERATOR_011,								// ��ġ�� ��û			
	CW_CHAT_SHOUT_BRD		= PROTOCOL_GENERATOR_012,								// ��ġ�� ���� ��ε� ĳ����			
	CW_CHAT_PARTY_SYN		= PROTOCOL_GENERATOR_013,								// ��Ƽ ä��(���� �ʿ��� ���� �����Ͽ� �̸� ����-20060215)			
	CW_CHAT_PARTY_BRD		= PROTOCOL_GENERATOR_014,								// ��Ƽ ä�� ��ε� ĳ����			
	CW_CHAT_VOICE_REQUEST_SYN		= PROTOCOL_GENERATOR_015,						// ���� ä�� ��û(C->W)					
	CW_CHAT_VOICE_REQUEST_BRD		= PROTOCOL_GENERATOR_016,						// ���� ä�� ��û����(W->C)					
	CW_CHAT_VOICE_REQUEST_NAK		= PROTOCOL_GENERATOR_017,						// ���� ä�� ��û ����(W->C)					
	CW_CHAT_VOICE_ACCEPT_SYN		= PROTOCOL_GENERATOR_018,						// ���� ä�� ����(C->W)					
	CW_CHAT_VOICE_ACCEPT_BRD		= PROTOCOL_GENERATOR_019,						// ���� ä�� ��������(W->C)					
	CW_CHAT_VOICE_ACCEPT_NAK		= PROTOCOL_GENERATOR_020,						// ���� ä�� ���� ����(W->C)					
	CW_CHAT_VOICE_REJECT_SYN		= PROTOCOL_GENERATOR_021,						// ���� ä�� ����(C->W)					
	CW_CHAT_VOICE_REJECT_BRD		= PROTOCOL_GENERATOR_022,						// ���� ä�� ��������(W->C)					
	CW_CHAT_VOICE_DISCONNECT_SYN		= PROTOCOL_GENERATOR_023,					// ���� ä�� ���� ��û(C->W)						
	CW_CHAT_VOICE_DISCONNECT_BRD		= PROTOCOL_GENERATOR_024,					// ���� ä�� ����(W->C)						
	CW_CHAT_VOICE_DISCONNECT_NAK		= PROTOCOL_GENERATOR_025,					// ���� ä�� ���� ����(W->C)						
	CW_CHAT_NORMAL_NAK					= PROTOCOL_GENERATOR_026,					// �Ϲ� ä�� ����
	CW_CHAT_VOICE_MIC_STATUS_SYN		= PROTOCOL_GENERATOR_027,					// ���� ä�� ����ũ ON/OFF (C->W)
	CW_CHAT_VOICE_MIC_STATUS_BRD		= PROTOCOL_GENERATOR_028,					// ���� ä�� ����ũ ON/OFF ��ε�(W->C)
	CW_CHAT_VOICE_MIC_STATUS_NAK		= PROTOCOL_GENERATOR_029,					// ���� ä�� ����ũ ON/OFF ����(W->C)
	CW_CHAT_SHOUT_NAK					= PROTOCOL_GENERATOR_030,					// ��ġ�� ����
	CW_CHAT_PARTY_MIC_STATUS_REQ		= PROTOCOL_GENERATOR_031,					// ��Ƽ �������� �� ���� ��Ƽ���� ���� ä�� ���� ��û(C->W)
	CW_CHAT_PARTY_MIC_STATUS_ANS		= PROTOCOL_GENERATOR_032,					// ���� ��Ƽ���� ���� ä�� ���� ����(W->C)
	CW_CHAT_PARTY_MIC_STATUS_NAK		= PROTOCOL_GENERATOR_033,					// ���� ��Ƽ���� ���� ä�� ���� ���� ����(W->C)
	CW_CHAT_PLAYER_MIC_STATUS_REQ		= PROTOCOL_GENERATOR_034,					// �ٸ� ĳ���� ���� ä�� on/off ���� Ȯ�� ��û(C->W)
	CW_CHAT_PLAYER_MIC_STATUS_ANS		= PROTOCOL_GENERATOR_035,					// �ٸ� ĳ���� ���� ä�� on/off ���� Ȯ�� ����(W->C)
	CW_CHAT_BLOCKED_CMD					= PROTOCOL_GENERATOR_036,					// ä�� ���� ���ߴٰ� �˷���
	CW_CHAT_CHANNEL_JOIN_SYN			= PROTOCOL_GENERATOR_037,					// ä�� ä�ù� ���� ��û
	CW_CHAT_CHANNEL_JOIN_ACK			= PROTOCOL_GENERATOR_038,					// ä�� ä�ù� ���� ����(���� ���ε� ǥ��)
	CW_CHAT_CHANNEL_JOIN_NAK			= PROTOCOL_GENERATOR_039,					// ä�� ä�ù� ���� ����
	CW_CHAT_CHANNEL_JOIN_BRD			= PROTOCOL_GENERATOR_040,					// ä�� ä�ù濡 ���� ������
	CW_CHAT_CHANNEL_SEND_SYN			= PROTOCOL_GENERATOR_041,					// ä�� ä�ù濡�� ä��
	CW_CHAT_CHANNEL_SEND_BRD			= PROTOCOL_GENERATOR_042,					// ä�� ä�ù濡�� ä��
	CW_CHAT_CHANNEL_SEND_NAK			= PROTOCOL_GENERATOR_043,					// ä�� ä�ù濡�� ä�� ����
	CW_CHAT_CHANNEL_INVITE_SYN			= PROTOCOL_GENERATOR_044,					// ä�� ä�ù濡 ��� �ʴ�
	CW_CHAT_CHANNEL_INVITE_ACK			= PROTOCOL_GENERATOR_045,					// ä�� ä�ù濡 ��� �ʴ� ����
	CW_CHAT_CHANNEL_INVITE_NAK			= PROTOCOL_GENERATOR_046,					// ä�� ä�ù濡 ��� �ʴ� ����
	CW_CHAT_CHANNEL_INVITE_CMD			= PROTOCOL_GENERATOR_047,					// ä�� ä�ù濡 ��밡 �ʴ�����
	CW_CHAT_CHANNEL_OUT_SYN				= PROTOCOL_GENERATOR_048,					// ä�� ä�ù濡�� ����
	CW_CHAT_CHANNEL_OUT_ACK				= PROTOCOL_GENERATOR_049,					// ä�� ä�ù濡�� ���� ����
	CW_CHAT_CHANNEL_OUT_NAK				= PROTOCOL_GENERATOR_050,					// ä�� ä�ù濡�� ���� ����
	CW_CHAT_CHANNEL_OUT_BRD				= PROTOCOL_GENERATOR_051,					// ä�� ä�ù濡�� ������ ��������
	CW_CHAT_CHANNEL_INFO_REQ			= PROTOCOL_GENERATOR_052,					// ä�� ä�ù� ���� ��û(�ο�, �̸�)
	CW_CHAT_CHANNEL_INFO_ANS			= PROTOCOL_GENERATOR_053,					// ä�� ä�ù� ����
	CW_CHAT_CHANNEL_INFO_NAK			= PROTOCOL_GENERATOR_054,					// ä�� ä�ù� ���� ��û ����
};														

enum eCW_GM_PROTOCOL														
{														
	CW_GM_NOTICE_SYN		= PROTOCOL_GENERATOR_001,							// ����				
	CW_GM_NOTICE_NAK		= PROTOCOL_GENERATOR_002,							// ���� ����(������ �����ϸ� �ڽŵ� �ޱ� ������ ACK�� �ʿ����)				

	CW_GM_STRING_CMD_SYN		= PROTOCOL_GENERATOR_004,						// ��Ʈ������ ��ɾ� ���� ��û(�����ʿ��� ��ɾ� �Ľ��ؼ� ó���� ��� ���)					
	CW_GM_STRING_CMD_NAK		= PROTOCOL_GENERATOR_005,											

	CW_GM_WHISPER_SET_ACK		= PROTOCOL_GENERATOR_007,						// GM�ӼӸ� ��/�� ����
	CW_GM_WHISPER_SET_NAK		= PROTOCOL_GENERATOR_008,						// GM�ӼӸ� ��/�� ����
	CW_GM_USERCHAT_BLOCK_ACK	= PROTOCOL_GENERATOR_009,						// ���� ä�ñ���/��� ����
	CW_GM_USERCHAT_BLOCK_NAK	= PROTOCOL_GENERATOR_010,						// ���� ä�ñ���/��� ����
};														

enum eCW_VIEWPORT_PROTOCOL														
{														
	CW_VIEWPORT_CHARSTATE		= PROTOCOL_GENERATOR_001,						// ĳ���� ��ġ ����					
};														

enum eCW_ARMORKIT_PROTOCOL														
{														
	CW_ARMORKIT_FINDHACK		= PROTOCOL_GENERATOR_001,						// ��ŷ �õ� �߰�					
};														

enum eCW_FRIEND_PROTOCOL														
{														
	CW_FRIEND_NEWLIST_SYN		= PROTOCOL_GENERATOR_001,					// ó�� ������ �� ģ��/���� ��� ��û					
	CW_FRIEND_FRIENDLIST_ACK		= PROTOCOL_GENERATOR_002,				// ģ�� ���						
	CW_FRIEND_BLOCKLIST_ACK		= PROTOCOL_GENERATOR_003,					// ���� ���						
	CW_FRIEND_NEWLIST_NAK		= PROTOCOL_GENERATOR_004,					// ģ��/���� ��� ��û ����					
	CW_FRIEND_LOGIN_BRD		= PROTOCOL_GENERATOR_005,						// ���� �߿� ģ�� �α׿�					
	CW_FRIEND_LOGOUT_BRD		= PROTOCOL_GENERATOR_006,											
	//	CW_FRIEND_CHGSTATE_BRD		= PROTOCOL_GENERATOR_007,						// ���� �߿� ģ�� ���� �ٲ�					
	CW_FRIEND_ADD_SYN		= PROTOCOL_GENERATOR_008,						// ģ�� �߰�			
	CW_FRIEND_ADD_ACK		= PROTOCOL_GENERATOR_009,						// ģ�� �߰� ����				
	CW_FRIEND_ADD_NAK		= PROTOCOL_GENERATOR_010,						// ģ�� �߰� ����				
	CW_FRIEND_DEL_SYN		= PROTOCOL_GENERATOR_011,						// ģ�� ���� ��û				
	CW_FRIEND_DEL_ACK		= PROTOCOL_GENERATOR_012,						// ģ�� ���� ����				
	CW_FRIEND_DEL_NAK		= PROTOCOL_GENERATOR_013,						// ģ�� ���� ����				
	CW_FRIEND_BLOCK_SYN		= PROTOCOL_GENERATOR_014,						// ���� ��û					
	CW_FRIEND_BLOCK_ACK		= PROTOCOL_GENERATOR_015,						// ���� ���					
	CW_FRIEND_BLOCK_NAK		= PROTOCOL_GENERATOR_016,						// ���� ����					
	CW_FRIEND_BLOCK_FREE_SYN		= PROTOCOL_GENERATOR_017,				// ���� ���� ��û						
	CW_FRIEND_BLOCK_FREE_ACK		= PROTOCOL_GENERATOR_018,				// ���� ���� ���						
	CW_FRIEND_BLOCK_FREE_NAK		= PROTOCOL_GENERATOR_019,				// ���� ���� ����						
	CW_FRIEND_CHAT_SYN		= PROTOCOL_GENERATOR_020,						// ģ�� ä��(�޽��� ä��)				
	CW_FRIEND_CHAT_BRD		= PROTOCOL_GENERATOR_021,						// ģ���� ������ ä���� �õ��ؿ�				
	CW_FRIEND_CHAT_NAK		= PROTOCOL_GENERATOR_022,						// ģ�� ä�� ����				
	CW_FRIEND_CHATBLOCK_SYN		= PROTOCOL_GENERATOR_023,					// �޽��� ä�� �ź� ��û						
	CW_FRIEND_CHATBLOCK_ACK		= PROTOCOL_GENERATOR_024,					// �޽��� ä�� �ź� ����						
	CW_FRIEND_CHATBLOCK_NAK		= PROTOCOL_GENERATOR_025,					// �޽��� ä�� �ź� ����						
	//	CW_FRIEND_CHAT_INVITE_SYN		= PROTOCOL_GENERATOR_026,					// ģ�� ä�� �ʴ�(���ܵ��� �ʾҴٸ� �ڵ����� ��ȭ ����)						
	//	CW_FRIEND_CHAT_INVITE_BRD		= PROTOCOL_GENERATOR_027,					// ģ�� ä�� �ʴ� ����						
	//	CW_FRIEND_CHAT_INVITE_NAK		= PROTOCOL_GENERATOR_028,					// ģ�� ä�� �ʴ� ����						
	CW_FRIEND_LEVELUP_BRD	= PROTOCOL_GENERATOR_029,						// ģ�� ������
	CW_FRIEND_STATUS_REQ	= PROTOCOL_GENERATOR_030,						// ģ�� ������
	CW_FRIEND_STATUS_ANS	= PROTOCOL_GENERATOR_031,
	CW_FRIEND_STATUS_NAK	= PROTOCOL_GENERATOR_032,
	CW_FRIEND_STATUS_JOIN_SYN	= PROTOCOL_GENERATOR_033,					// ģ���� ���� �����ϱ�
	CW_FRIEND_STATUS_JOIN_ACK	= PROTOCOL_GENERATOR_034,
	CW_FRIEND_STATUS_JOIN_NAK	= PROTOCOL_GENERATOR_035,
    CW_FRIENDINVITATE_INVITATELIST_CMD	= PROTOCOL_GENERATOR_036, //_KR_0_20091021_FIRENDINVITATION_EXTEND
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    CW_FRIEND_MEMO_UPDATE_SYN = PROTOCOL_GENERATOR_037,
    CW_FRIEND_MEMO_UPDATE_ACK = PROTOCOL_GENERATOR_038,
    CW_FRIEND_MEMO_UPDATE_NAK = PROTOCOL_GENERATOR_039,
};														

enum eCW_GUILD_PROTOCOL														
{														
	CW_GUILD_CHAT_SYN				= PROTOCOL_GENERATOR_003,		// ��� ä�� ��û				
	CW_GUILD_CHAT_BRD				= PROTOCOL_GENERATOR_004,		// ��� ä��				
	CW_GUILD_CHAT_NAK				= PROTOCOL_GENERATOR_005,		// ��� ä�� ����				

	CW_GUILD_RELATION_BRD				= PROTOCOL_GENERATOR_015,	// ��� ����
	CW_GUILD_RELATION_DELETE_BRD		= PROTOCOL_GENERATOR_016,	// ��� ���� ����
	CW_GUILD_RELATION_BOTH_AGAINST_BRD	= PROTOCOL_GENERATOR_017,	// �ֹ� ����� ����
	CW_GUILD_RELATION_MOVE_BRD			= PROTOCOL_GENERATOR_018,	// ��� ���� �̵�

	//CW_GUILD_CHANGE_DUTY_BRD			= PROTOCOL_GENERATOR_019,		// ��� ��å
	//CW_GUILD_CHANGE_DUTY_CMD			= PROTOCOL_GENERATOR_020,		// ��� ��å
	//CW_GUILD_RIGHT_DELEGATION_CMD		= PROTOCOL_GENERATOR_021,		// ��� ���� ����
	//CW_GUILD_CHANGE_MASTER_BRD          = PROTOCOL_GENERATOR_022,		// ��� ������ ����

	CW_GUILD_RECOVERY_GUILD_BRD		= PROTOCOL_GENERATOR_023,		//��� ����

	CW_GUILD_GUILDMARK_REGISTER_BRD = PROTOCOL_GENERATOR_025,
	CW_GUILD_GUILDMARK_DELETE_BRD = PROTOCOL_GENERATOR_026,

	CW_GUILD_ALLPLAYERS_GUILD_INFO_CMD			= PROTOCOL_GENERATOR_029,	// �ʵ�   ��� ���� ���� ����
	CW_GUILD_VILLAGE_ALLPLAYERS_GUILD_INFO_CMD	= PROTOCOL_GENERATOR_030,	// ����   ��� ���� ���� ����
	CW_GUILD_PLAYER_GUILD_INFO_BRD				= PROTOCOL_GENERATOR_031,	// �ڽ��� ��� ���� ����	

	//40���� �뿪�� ��� ���� �۾� ���� ���.
	//CW_GUILD_CREATE_CORPS_BRD			= PROTOCOL_GENERATOR_040,		// ��� ���� ����
	//CW_GUILD_DESTROY_CORPS_BRD			= PROTOCOL_GENERATOR_041,		// ��� ���� �ػ�
	//CW_GUILD_CHANGE_COMMANDER_BRD		= PROTOCOL_GENERATOR_042,		// ��� ������ ����
	//CW_GUILD_CHANGE_CORPS_BRD			= PROTOCOL_GENERATOR_043,		// ��� ���� �̵�
	//CW_GUILD_CREATE_REAR_CAMP_BRD		= PROTOCOL_GENERATOR_044,		// ��� ���� �Ʒ�ķ�� ����
	//CW_GUILD_DESTROY_REAR_CAMP_BRD		= PROTOCOL_GENERATOR_045,		// ��� ���� �Ʒ�ķ�� �ػ�
	CW_GUILD_WAREHOUSE_LOGLIST_REQ	= PROTOCOL_GENERATOR_050,		// ��� â�� �α� ��û
	CW_GUILD_WAREHOUSE_LOGLIST_ANS	= PROTOCOL_GENERATOR_051,		
	CW_GUILD_WAREHOUSE_LOGLIST_NAK	= PROTOCOL_GENERATOR_052,		 

	CW_GUILD_CHANGE_NOTICE_SYN		= PROTOCOL_GENERATOR_055,		// ��� ���� ����
	CW_GUILD_CHANGE_NOTICE_BRD		= PROTOCOL_GENERATOR_056,
	CW_GUILD_CHANGE_NOTICE_NAK		= PROTOCOL_GENERATOR_057,
	CW_GUILD_NOTICE_CMD					= PROTOCOL_GENERATOR_058,		// ��� ������ ������( ��� ���Խó� �α��ν� ������. )
	CW_GUILD_SELECT_NOTICE_SYN		= PROTOCOL_GENERATOR_059,
	CW_GUILD_SELECT_NOTICE_ACK			= PROTOCOL_GENERATOR_060,
	CW_GUILD_SELECT_NOTICE_NAK		= PROTOCOL_GENERATOR_061,
    //_KR_001385_20091202_DOMINATION_RETURN_HEIM
    CW_DOMINATION_RETURN_HEIM_CMD   = PROTOCOL_GENERATOR_062,       //���� ������ ���� ���� ����� ȯ�� ��û
};														

// <Cryptography>														
enum eCW_CRYPTOGRAPHY_PROTOCOL														
{														
	CW_CRYPTOGRAPHY_KEY		= PROTOCOL_GENERATOR_001,						// ��Ŷ ��ȣȭ Ű ��ȯ(W->C)					
	CW_CRYPTOGRAPHY_ACK		= PROTOCOL_GENERATOR_002,						// ��Ŷ ��ȣȭ Ű ���� ����(C->W)					
	CW_CRYPTOGRAPHY_NAK		= PROTOCOL_GENERATOR_003,						// ��Ŷ ��ȣȭ Ű ���� ����(C->W)					
	CW_CRYPTOGRAPHY_CMD		= PROTOCOL_GENERATOR_004,						// ��Ŷ ��ȣȭ Ű ����ϵ��� ���(W->C)					
	CW_CRYPTOGRAPHY_PACKET_ENCODED		= PROTOCOL_GENERATOR_005,											
	CW_CRYPTOGRAPHY_PACKET_ENCODING		= PROTOCOL_GENERATOR_006,											
	//		= PROTOCOL_GENERATOR_007,											
	CW_CRYPTOGRAPHY_INFO_TYPE0_SUB_CMD		= PROTOCOL_GENERATOR_008,	// 144 + n										
	CW_CRYPTOGRAPHY_INFO_TYPE1_SUB_CMD		= PROTOCOL_GENERATOR_009,											
	CW_CRYPTOGRAPHY_INFO_TYPE2_SUB_CMD		= PROTOCOL_GENERATOR_010,											
	CW_CRYPTOGRAPHY_INFO_TYPE3_SUB_CMD		= PROTOCOL_GENERATOR_011,											
	CW_CRYPTOGRAPHY_INFO_TYPE4_SUB_CMD		= PROTOCOL_GENERATOR_012,											
	CW_CRYPTOGRAPHY_INFO_TYPE5_SUB_CMD		= PROTOCOL_GENERATOR_013,											
	CW_CRYPTOGRAPHY_INFO_TYPE6_SUB_CMD		= PROTOCOL_GENERATOR_014,											
	CW_CRYPTOGRAPHY_INFO_TYPE7_SUB_CMD		= PROTOCOL_GENERATOR_015,											
};														

enum eCW_COMMUNITY_PROTOCOL
{
	CW_COMM_USERID_REQ		= PROTOCOL_GENERATOR_001,						// �ٸ� ������� UserID ��û(�� ���� ����)
	CW_COMM_USERID_ACK		= PROTOCOL_GENERATOR_002,
	CW_COMM_USERID_NAK		= PROTOCOL_GENERATOR_003,
};

enum eCW_REVENGE_PROTOCOL
{
	CW_REVENGE_ADD_CMD		= PROTOCOL_GENERATOR_001,					// Revenge User Added

	CW_REVENGE_DEL_SYN		= PROTOCOL_GENERATOR_002,					// ������ ����Ʈ���� ����
	CW_REVENGE_DEL_ACK		= PROTOCOL_GENERATOR_003,
	CW_REVENGE_DEL_NAK		= PROTOCOL_GENERATOR_004,
	CW_REVENGE_LIST_CMD		= PROTOCOL_GENERATOR_005,					// ������ ����Ʈ
	CW_REVENGE_LOGIN_BRD	= PROTOCOL_GENERATOR_006,					// ������ ��� �α���
	CW_REVENGE_LOGOUT_BRD	= PROTOCOL_GENERATOR_007,					// ������ ��� �α׾ƿ�
	CW_REVENGE_LEVELUP_BRD	= PROTOCOL_GENERATOR_008,					// ������ ��� ������
};

enum eCW_MEMO_PROTOCOL
{
	CW_MEMO_SEND_SYN		= PROTOCOL_GENERATOR_001,					// ���� ����
	CW_MEMO_SEND_ACK		= PROTOCOL_GENERATOR_002,
	CW_MEMO_SEND_NAK		= PROTOCOL_GENERATOR_003,

	CW_MEMO_LIST_REQ		= PROTOCOL_GENERATOR_005,					// ���� ���
	CW_MEMO_LIST_ANS		= PROTOCOL_GENERATOR_006,
	CW_MEMO_LIST_NAK		= PROTOCOL_GENERATOR_007,

	CW_MEMO_VIEW_REQ		= PROTOCOL_GENERATOR_009,					// ���� Ȯ��
	CW_MEMO_VIEW_ANS		= PROTOCOL_GENERATOR_010,
	CW_MEMO_VIEW_NAK		= PROTOCOL_GENERATOR_011,

	CW_MEMO_DELETE_SYN		= PROTOCOL_GENERATOR_013,					// ���� ����
	CW_MEMO_DELETE_ACK		= PROTOCOL_GENERATOR_014,
	CW_MEMO_DELETE_NAK		= PROTOCOL_GENERATOR_015,

	CW_MEMO_NEWMEMO_BRD		= PROTOCOL_GENERATOR_017,
};

//__KR_001355_20090608_GROUPINGSYSTEM_CW
enum eCW_GROUP_PROTOCOL										
{
	CW_GROUP_LOGIN_BRD			= PROTOCOL_GENERATOR_001,				//���� �߿� �׷�ɹ� �α׿� �뺸
	CW_GROUP_LOGOUT_BRD			= PROTOCOL_GENERATOR_002,				//���� �߿� �׷�ɹ� �α׾ƿ� �뺸
	CW_GROUP_LEVELUP_BRD		= PROTOCOL_GENERATOR_003,				//�׷� ��� ������ �뺸
	CW_GROUP_ACCUMEXP_BRD		= PROTOCOL_GENERATOR_004,				//���� ����� �׾� ���� ����ġ�� �޾����� �뺸
	CW_GROUP_BONUSMONEY_BRD		= PROTOCOL_GENERATOR_005,				//�׷� ���ʽ� ���� �뺸
	CW_GROUP_BONUSEXP_BRD		= PROTOCOL_GENERATOR_006,				//�׷� ���ʽ� ����ġ �뺸
    CW_GROUP_DEL_BRD            = PROTOCOL_GENERATOR_007,               //�׷� �ɹ� ���� �뺸
    CW_GROUP_RESTOR_CMD         = PROTOCOL_GENERATOR_008,               //�׷� ���� ���û

	CW_GROUP_GROUPLIST_REQ		= PROTOCOL_GENERATOR_009,				//�׷� ��� ��û
	CW_GROUP_GROUPLIST_ANS		= PROTOCOL_GENERATOR_010,				//�׷� ���	����
	CW_GROUP_GROUPLIST_NAK		= PROTOCOL_GENERATOR_011,	    		//�׷� ���	����

	CW_GROUP_ADD_SYN			= PROTOCOL_GENERATOR_012,				//�׷� ��� �߰� �۾� ��û			
	CW_GROUP_ADD_ACK			= PROTOCOL_GENERATOR_013,				//�׷� ��� �߰� ����				
	CW_GROUP_ADD_NAK			= PROTOCOL_GENERATOR_014,				//�׷� ��� �߰� ����		

    CW_GROUP_ADD_RESPONSE_SYN   = PROTOCOL_GENERATOR_015,				//�׷� ��� �߰� �۾� ���� 			
    CW_GROUP_ADD_RESPONSE_ACK	= PROTOCOL_GENERATOR_016,				//�׷� ��� �߰� ����				
    CW_GROUP_ADD_RESPONSE_NAK	= PROTOCOL_GENERATOR_017,				//�׷� ��� �߰� ����		

	CW_GROUP_DEL_SYN			= PROTOCOL_GENERATOR_018,				//�׷� ��� ���� �۾� ��û				
	CW_GROUP_DEL_ACK			= PROTOCOL_GENERATOR_019,				//�׷� ��� ���� ����				
	CW_GROUP_DEL_NAK			= PROTOCOL_GENERATOR_020,				//�׷� ��� ���� ����
    
	// ���� _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    // ���� �� ���� ��û���� ����
	CW_GROUP_STATUS_REQ			= PROTOCOL_GENERATOR_021,				//�׷� ��� ������ ��û   
	CW_GROUP_STATUS_ANS			= PROTOCOL_GENERATOR_022,				//�׷� ��� ������ ���   
    CW_GROUP_STATUS_NAK         = PROTOCOL_GENERATOR_023,               //�׷� ��� ������ ����   

    CW_GROUP_ADDBLOCK_SYN       = PROTOCOL_GENERATOR_024,               //�ɹ� �߰� �ź� ��� ��û
    CW_GROUP_ADDBLOCK_ACK       = PROTOCOL_GENERATOR_025,               //�ɹ� �߰� �ź� ��� ��û ����
    CW_GROUP_ADDBLOCK_NAK       = PROTOCOL_GENERATOR_026,               //�ɹ� �߰� �ź� ��� ��û ����

    //�׷� �ɹ� �߰� ���� ���� ����
    CW_GROUP_REWARD_FAIL_CMD    = PROTOCOL_GENERATOR_027,               //�׷� �ɹ��� 6�� �̻��� ��쿡 ���� ó���� ���ش�.


};

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
enum eCW_LOTTO_PROTOCOL
{
    CW_LOTTO_START_BRD          = PROTOCOL_GENERATOR_001,   // ���� �ζ� ���� ���� ����
    CW_LOTTO_INFO_SYN           = PROTOCOL_GENERATOR_002,   // (��) ���� �ζ� ���� ��û
    CW_LOTTO_INFO_ACK           = PROTOCOL_GENERATOR_003,   // (��) ���� �ζ� ���� ����
    CW_LOTTO_INFO_NAK           = PROTOCOL_GENERATOR_004,   // (��) ���� �ζ� ���� ���� ����
    CW_LOTTO_TICKET_NTF         = PROTOCOL_GENERATOR_005,   // ���� �ζ� ���� ȹ�� �˸� : ���ǿ��� ���� ���� ����� ������� ���� ��
    CW_LOTTO_ENTRY_SYN          = PROTOCOL_GENERATOR_006,   // ���� �ζ� ���� ��û : ���ǿ��� ���� ���� ��� ���� ����  ����
    CW_LOTTO_ENTRY_ACK          = PROTOCOL_GENERATOR_007,   // ���� �ζ� ���� ���� ����
    CW_LOTTO_ENTRY_NAK          = PROTOCOL_GENERATOR_008,   // ���� �ζ� ���� ���� ����
    CW_LOTTO_ACCUM_BRD          = PROTOCOL_GENERATOR_009,   // ���� �ζ� ������ ���� ����
    CW_LOTTO_COUNTDOWN_BRD      = PROTOCOL_GENERATOR_010,   // ���� �ζ� ��÷ ī��Ʈ�ٿ� ����
    CW_LOTTO_FINISH_BRD         = PROTOCOL_GENERATOR_011,   // ���� �ζ� ��÷/���� ����
    CW_LOTTO_NOTICE_ALM         = PROTOCOL_GENERATOR_012,   // ��÷ �ȳ� ���� ���� �˸�
    CW_LOTTO_NOTICE_NTF         = PROTOCOL_GENERATOR_013,   // ��÷ �ȳ� ���� �뺸
    CW_LOTTO_NOTICE_LIST_SYN    = PROTOCOL_GENERATOR_014,   // ��÷ �ȳ� ���� ��� ��û
    CW_LOTTO_NOTICE_LIST_ACK    = PROTOCOL_GENERATOR_015,   // ��÷ �ȳ� ���� ��� ����
    CW_LOTTO_NOTICE_CONFIRM     = PROTOCOL_GENERATOR_016,   // ��÷ �ȳ� ���� ���� Ȯ��
};
//}

// _NA_008283_20150511_NOTIFICATIONS_SYSTEM
enum eCW_NOTIFICATION_PROTOCOL
{
    CW_NOTIFICATION_BRD         = PROTOCOL_GENERATOR_001,   // �˸� �޼��� ��ε�
    CW_NOTIFICATION_PREV_SYN    = PROTOCOL_GENERATOR_002,   // ���� �˸� �޼��� ��û
    CW_NOTIFICATION_PREV_ACK    = PROTOCOL_GENERATOR_003,   // ���� �˸� �޼��� ����
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ ���� ��������
enum eCW_PARTY_PROTOCOL
{
    CW_PARTY_FINDING_REGISTER_SYN = PROTOCOL_GENERATOR_001, // ��Ƽ ���� ���/���� ��û 

    CW_PARTY_FINDING_GET_LIST_SYN = PROTOCOL_GENERATOR_002, // ��Ƽ ���� ��� ��û
    CW_PARTY_FINDING_GET_LIST_ACK = PROTOCOL_GENERATOR_003, // ��Ƽ ���� ��� ����
    
    CW_PARTY_FINDING_JOIN_REQUEST_SYN = PROTOCOL_GENERATOR_004, // ��Ƽ ���� ���� ��û ��û
    
    CW_PARTY_FINDING_JOIN_RESPONSE_CMD = PROTOCOL_GENERATOR_005, // ��Ƽ ���� ���� ��û�� ���� ���� ��û
    CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK = PROTOCOL_GENERATOR_006, // ��Ƽ ���� ���� ��û�� ���� ����

    CW_PARTY_FINDING_CANCEL_SYN = PROTOCOL_GENERATOR_007, // ��Ƽ ���� ��� ��û
    
    CW_PARTY_FINDING_RESULT_ACK = PROTOCOL_GENERATOR_008, // ��Ƽ ���� ���/����/����/��� ��û�� ���� ����

}; // eCW_PARTY_PROTOCOL

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////
