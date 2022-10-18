//=============================================================================================================================
/// SolarAuth Cryptography Control class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 26
	@remark
		- SolarAuth, Master���� ���Ǵ� �⺻ ��ȣȭ ���� ��Ʈ�� Ŭ����
	@note
	@history 
		-  
*/
//=============================================================================================================================


#pragma once

#ifndef __CRYPT_FLOW_POLICY_H
#define __CRYPT_FLOW_POLICY_H

// 
#include <winsock2.h>

// About Cryptography
#include <RandomSNKeyS2S.h>
#include <CryptManagerS2S.h>

class CryptFlowPolicy
{
public:
	struct Utility
	{
		static DWORD GetChunkValue();
		inline static DWORD GetIPAddress( const CHAR* strIPAddress )
		{
			return (DWORD)inet_addr( strIPAddress );
		}
	};

	struct MakePacket
	{
		// Master -> SolarAuth
		// <Arguments>
		//   2: dwServerIPAddress : DWORD
		//     ��� Master������ �ν� ������ ���� IP (public) : SolarAuth�� �޾Ƶ��̴� IP�� �ٸ� �� �ִ�. (ex.NAT)
		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static DWORD PostRecv_SM_CONNECTION_SYN_1st( BYTE* pMsgACK, DWORD dwServerIPAddress );

		// SolarAuth -> Master
		// <Action>
		//   encode�� ��Ŷ decode, �ش� ��Ŷ�� �����ʹ� decode�ȴ�.
		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static DWORD PostRecv_SM_CONNECTION_ACK_1st( BYTE* pMsgACK, DWORD dwServerIPAddress );

		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static DWORD PostRecv_DefConverter( BYTE* pMsgBASE, DWORD dwCryptKey, DWORD dwSeqCryptKey );

		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static DWORD PreSend_DefConverter( BYTE* pMsgBASE, DWORD dwCryptKey, DWORD dwSeqCryptKey );

		// Master -> SolarAuth
		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static VOID PostRecv_SM_CONNECTION_SYN_2nd( BYTE* pMsgACK, DWORD dwCryptKey, DWORD dwSeqCryptKey )
		{	PostRecv_DefConverter( pMsgACK, dwCryptKey, dwSeqCryptKey );	}

		// SolarAuth -> Master
		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static VOID PostRecv_SM_CONNECTION_ACK_2nd( BYTE* pMsgACK, DWORD dwCryptKey, DWORD dwSeqCryptKey )
		{	PostRecv_DefConverter( pMsgACK, dwCryptKey, dwSeqCryptKey );	}
	};
};

// <WARNING>
//   �̰��� Singleton��ü�� �ƴմϴ�.
class sOBJECT_CHANGER
{
private:
	sOBJECT_CHANGER() : m_dwMaxObjectCount(0), m_AMBIGOUS(NULL) {}
public:
	// �ʹ� ���� ���� ������ �Է����� ������. �ʿ��� ��ŭ��... 100���� �����մϴ�.
	static sOBJECT_CHANGER*		CreateObjectChanger( DWORD dwMaxObjectCount );
	static VOID					ReleaseObjectChanger( sOBJECT_CHANGER* pInst );

	// �����ڿ� �ϴ� �� ���� ���� ������?
	template<typename _TYPE_>
	inline BOOL					RegisterObject( DWORD dwAlignIndex )
	{
		BYTE instObj[sizeof(_TYPE_)];
		_TYPE_* pInst = new (instObj) _TYPE_;
		return _RegisterObject( dwAlignIndex, pInst );
	}

	BOOL						ObjectChangeTo( PVOID targetObj, DWORD toObj );

private:
	BOOL						_RegisterObject( DWORD dwAlignIndex, PVOID instanceObj );
private:
	DWORD	m_dwMaxObjectCount;
	PVOID	m_AMBIGOUS;
};

#pragma pack(push,1)

// ��ȣȭ�� ��Ŷ Ǯ������...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED {
	BYTE						m_bySN;			// Sequence Number : received
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// ��Ŷ ��ȣȭ �ϱ� ����...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING {
	BYTE						m_bySN;			// Sequence Number : sending
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// ��Ŷ���� �ٸ� ũ�⸦ ���� ���̴�.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

// Agent ���ؿ�
struct	PACKET_CONTROL_SM
{
	static const DWORD	MAX_PACKET_SIZE = 16384;

	union uPACKET_STRUCTURE
	{
		BYTE	PacketStream[PACKET_CONTROL_SM::MAX_PACKET_SIZE];

		struct
		{
			DWORD	dwBLANK;
			BYTE	pS2S_PacketPtr[1];
		} PK_NORMAL;

		struct
		{
			WORD	wBLANK;
			union
			{
				MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING	pMSG_ENCODING[1];
				MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED	pMSG_ENCODED[1];
			};
		} PK_ENDECODE;
	};

	static uPACKET_STRUCTURE		sRECV_PACKET_STRUCTURE;
	static uPACKET_STRUCTURE		sSEND_PACKET_STRUCTURE;

	// Default PacketPointer
	static BYTE*		GetBasePacketPtr4Send() { return (BYTE*)sSEND_PACKET_STRUCTURE.PK_NORMAL.pS2S_PacketPtr; }
	// Default PacketPointer
	static BYTE*		GetBasePacketPtr4Recv() { return (BYTE*)sRECV_PACKET_STRUCTURE.PK_NORMAL.pS2S_PacketPtr; }

	// User(crypt)
	static MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED* GetPacketPtr_ENCODED()
	{ return sRECV_PACKET_STRUCTURE.PK_ENDECODE.pMSG_ENCODED; }
	// User(crypt)
	static MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING* GetPacketPtr_ENCODING()
	{ return sSEND_PACKET_STRUCTURE.PK_ENDECODE.pMSG_ENCODING; }
};
#pragma pack(pop)

#endif //__CRYPT_FLOW_POLICY_H

