//=============================================================================================================================
/// SolarAuth Cryptography Control class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 26
	@remark
		- SolarAuth, Master에서 사용되는 기본 암호화 관련 컨트롤 클래스
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
		//     상대 Master서버가 인식 가능한 서버 IP (public) : SolarAuth가 받아들이는 IP와 다를 수 있다. (ex.NAT)
		// <returns>
		//   random CRYPT_KEY by SolarAuth : DWORD
		static DWORD PostRecv_SM_CONNECTION_SYN_1st( BYTE* pMsgACK, DWORD dwServerIPAddress );

		// SolarAuth -> Master
		// <Action>
		//   encode된 패킷 decode, 해당 패킷의 데이터는 decode된다.
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
//   이것은 Singleton객체가 아닙니다.
class sOBJECT_CHANGER
{
private:
	sOBJECT_CHANGER() : m_dwMaxObjectCount(0), m_AMBIGOUS(NULL) {}
public:
	// 너무 많은 수의 개수를 입력하지 마세요. 필요한 만큼만... 100개로 제한합니다.
	static sOBJECT_CHANGER*		CreateObjectChanger( DWORD dwMaxObjectCount );
	static VOID					ReleaseObjectChanger( sOBJECT_CHANGER* pInst );

	// 생성자에 하는 일 없는 쪽이 좋겠죠?
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

// 암호화된 패킷 풀기위한...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED {
	BYTE						m_bySN;			// Sequence Number : received
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// 패킷마다 다른 크기를 가질 것이다.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};

// 패킷 암호화 하기 위한...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING {
	BYTE						m_bySN;			// Sequence Number : sending
	BYTE						m_byCRC;
	BYTE						m_byStream[1];	// 패킷마다 다른 크기를 가질 것이다.

	BYTE&						GetSNKey() {	return m_bySN;	}
	VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
	BYTE&						GetCRC() {	return m_byCRC;	}
	VOID						SetCRC( BYTE byCRC ) {	m_byCRC = byCRC;	}
	BYTE*						GetPacketStream() {		return m_byStream;	}
	DWORD						GetHeaderSize() { return (sizeof(BYTE)*2); }
};
// <END>

// Agent 기준용
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

