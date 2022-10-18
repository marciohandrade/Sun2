#pragma once


/// ī�װ�
enum eMO_CATEGORY
{
	MO_OPERATION			= 0,
};

/// ��������
enum eMO_OPERATION_PROTOCOL
{
	/*
	MO_RELOAD_DATA_SYN,
	*/
	MO_RTTG_HEARTBEAT					= 0x00,
	MO_RTTG_CERTIFY_REQ					= 0x01,
	MO_RTTG_CERTIFY_ANS					= 0x02,
	MO_RTTG_GAMENOTICE_REQ				= 0x03,
	MO_RTTG_GAMENOTICE_ANS				= 0x04,
	MO_RTTG_GAMEADMINLIST_REQ			= 0x05,
	MO_RTTG_GAMEADMINLIST_ANS			= 0x06,
	MO_RTTG_GAMEADMINADD_NTF			= 0x07,
	MO_RTTG_GAMEADMINREMOVE_NTF			= 0x08,
	MO_RTTG_DROPUSER_REQ				= 0x09,
	MO_RTTG_DROPUSER_ANS				= 0x0A,
	MO_RTTG_USERCOUNT_NTF				= 0x0B,
//	MO_RTTG_USERMONEY_NTF				= 0x0C,			// ��~
	MO_RTTG_BATTLEZONEINFO_REQ			= 0x0C,
	MO_RTTG_BATTLEZONEINFO_ANS			= 0x0D,
	MO_RTTG_ARMORKIT_FINDHACK_NTF		= 0x0E,

	MO_RTTG_ABUSING_NTF					= 0x0F,			// abuse alarm

	MO_RTTG_USERMONEY_REQ				= 0x10,			// ��~
	MO_RTTG_USERMONEY_ANS				= 0x11,

	MO_RTTG_SERVERSHUTDOWN_REQ			= 0x12,			// ���� �������� ��û
	MO_RTTG_SERVERSHUTDOWN_ANS			= 0x13,			// ���� �������� �뺸
	MO_RTTG_PATCHRESULT_REQ				= 0x14,			// ���� ��ġ ��� �뺸
	MO_RTTG_PATCHRESULT_ANS				= 0x15,

	MO_RTTG_USERDISCONNECT_REQ			= 0x16,			// ���� ���� ���� ���� ��û
	MO_RTTG_USERDISCONNECT_ANS			= 0x17,

	MO_RTTG_FUNCRESTRICT_RELOAD_CMD		= 0x18,			// ������� ���̺� �ٽ� �о Game�����鿡 Broadcast

	MO_RTTG_WAREHOUSEMONEY_REQ			= 0x19,			// â�� ��ȭ�� ��û(RTTA->Master)
	MO_RTTG_WAREHOUSEMONEY_ANS			= 0x1A,			// â�� ��ȭ�� ����(Master->RTTA)

	MO_RTTG_GAMESTATISTICS_REQ			= 0x1B,			// ���� ��� ��û
	MO_RTTG_GAMESTATISTICS_ANS			= 0x1C,			// ���� ��� ����

	MO_RTTG_ALL_SERVER_STATUS_NTF		= 0x20,			// ���� ������ ������ ON/OFF ����
    //{__NA001254_20090109_RTTG_FUNC_ADD 
	MO_RTTG_CHANGE_GUILDNAME_REQ		= 0x28,
	MO_RTTG_CHANGE_GUILDNAME_ANS		= 0x29,
	MO_RTTG_SEND_MESSAGE_REQ			= 0x2A,
	MO_RTTG_SEND_MESSAGE_ANS			= 0x2B,
    //}
	MO_RTTG_GMSCOMMAND_REQ				= 0xF0,
	MO_RTTG_GMSCOMMAND_ANS				= 0xF1,

};

//////////////////////////////////////////////////////////////////////////
//  Agent Type
#define  SERVERTYPE_AGENT				0x01			//  RTT Agent
#define  SERVERTYPE_AUTH				0x02			//  AuthServer
#define  SERVERTYPE_MASTER				0x04			//  MasterServer











