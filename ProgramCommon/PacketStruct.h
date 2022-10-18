#ifndef __PACKETSTRUCT_H__
#define __PACKETSTRUCT_H__

#include <windows.h>
#include <StructInPacket.h>

#pragma once

#pragma pack(push,1)


/**
	��Ŷ ����ü�� Naming Rule

=> MSG_CF_��Ŷ����ü�̸�_REQ

1) ��û(Request)					_REQ
2) ��û�� ���� ����(Answer)			_ANS
3) Client -> Server �뺸(Sync)		_SYN
4) Server -> Client �뺸(Command)	-CMD
5) Server -> Client Broadcasting	_BRD

 => ���� ���� : 2004-10.19
1) ��û(Request)					_SYN
2) ��û�� ���� ���					_ACK
3) ��û�� ���� �ź�					_NAK
4) Server -> Client �뺸(Notify)	_NTF
5) Server -> Client Broadcasting	_BRD

*/



//-------------------------------------------------------------------------------------------------------
/// @name �⺻ ������

// - ���� ���� ��Ŷ

//==================================================================================================
#define	__WAVERIX_CRYPT_ACK__			// Ŭ�� CRYPTOGRAPHY_ACK �ޱ� ����, KEY<-CMD
#define	__WAVERIX_CRYPTOGRAPHY_TEST__	// �ֱ��� Ű��ȯ ���� �׽�Ʈ <WARNING> __WAVERIX_CRYPT_ACK__ �� ������. Ǯ���� �� Ǯ��.
#define	__WAVERIX_PACKET_HANDLING__		// Ŭ��� UserKey ���� ���ϱ� ���� �׽�Ʈ

//--------------------------------------------
// __WAVERIX_ABUSE_EXTENDED_FORMAT__
// ���� ����, �˻����� ��ȭ�� �����ؼ� Ȯ��� ����
#define __WAVERIX_ABUSE_EXTENDED_FORMAT__		// �߰� ����, ���� ������ �� ��ó���� �ܺη�...
#define __WAVERIX_ABUSE_EXTENDED_SECURELOG__	// (07/02/08) ���� �α� ��ȭ�� �۾�



struct MSG_BASE_INTERNAL
{
	BYTE						m_byCategory;	// ī�װ�
	union
	{
		BYTE					m_byProtocol;		// ī�װ� �Ʒ��� Ÿ��
		BYTE					m_byEncryptedBuffer[1];		// ��ȣȭ�� ����
	};
};

struct MSG_PROTO
{
#	ifdef	_SERVER
	DWORD						m_dwKey;
#	endif
};

struct MSG_BASE : public MSG_PROTO
{
	BYTE						m_byCategory;	// ī�װ�
	union
	{
		BYTE					m_byProtocol;		// ī�װ� �Ʒ��� Ÿ��
		BYTE					m_byEncryptedBuffer[1];		// ��ȣȭ�� ����
	};
};

#ifdef _YJ_IGC
class IGC_MSG_HEADER8		// ��Ŷ ���
{
public:
	DWORD		dwID;			// �������� ID
};

#endif

struct MSG_SERVER_TYPE : public MSG_BASE
{
	BYTE						m_byServerType;
};

struct MSG_BASE_FORWARD : public MSG_BASE
{
};

//==================================================================================================



struct MSG_DBPROXY_RESULT : public MSG_BASE
{
	VOID *						m_pData;
};
struct MSG_BASE_GUILD : public MSG_BASE_FORWARD
{
	DWORD						m_dwSessionIndex;
};

#pragma pack(pop)


#endif // __PACKETSTRUCT_H__

