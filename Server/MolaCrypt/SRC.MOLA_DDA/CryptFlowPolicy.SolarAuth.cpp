//=============================================================================================================================
/// SolarAuth Cryptography Control class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 26
	@remark
		- SolarAuth 에서 사용되는 기본 암호화 관련 컨트롤 클래스 구현
	@note
	@history 
		-  
*/
//=============================================================================================================================

#include "stdafx.h"

#include <SolarAuth.CryptFlowPolicy.h>

#pragma pack(push, 1)
// 6Byte
union PacketTokens
{
	struct
	{
		WORD wToken1;
		DWORD dwToken2;
	} msgBASE_RELOC;

	struct
	{
		DWORD dwToken1;
		WORD wToken2;
	} msgBASE_NORMAL;
};
#pragma pack(pop)

//
PACKET_CONTROL_SM::uPACKET_STRUCTURE	PACKET_CONTROL_SM::sRECV_PACKET_STRUCTURE;
PACKET_CONTROL_SM::uPACKET_STRUCTURE	PACKET_CONTROL_SM::sSEND_PACKET_STRUCTURE;



//
DWORD CryptFlowPolicy::Utility::GetChunkValue()
{
	SYSTEMTIME st;
	GetLocalTime( &st );

	DWORD dwValue = (DWORD)st.wYear;
	dwValue *= st.wMonth;
	dwValue *= st.wDay;
	dwValue *= st.wHour;
	dwValue *= st.wMinute;
	dwValue *= st.wSecond;

	return dwValue;
}

DWORD
CryptFlowPolicy::MakePacket::PostRecv_SM_CONNECTION_SYN_1st( BYTE* pMsgACK, DWORD dwServerIPAddress )
{
	DWORD dwChunkValue = CryptFlowPolicy::Utility::GetChunkValue();
	DWORD dwTickCount = GetTickCount();
	DWORD dwMixCryptKey = dwChunkValue ^ dwTickCount;

	PacketTokens* pPacket = (PacketTokens*)pMsgACK;
	pPacket->msgBASE_NORMAL.dwToken1 = dwMixCryptKey;
	pPacket->msgBASE_NORMAL.dwToken1 ^= dwServerIPAddress;
	pPacket->msgBASE_RELOC.dwToken2 ^= dwServerIPAddress;

	return dwMixCryptKey;
	return 0;
}

DWORD
CryptFlowPolicy::MakePacket::PostRecv_SM_CONNECTION_ACK_1st( BYTE* pMsgACK, DWORD dwServerIPAddress )
{
	PacketTokens* pPacket = (PacketTokens*)pMsgACK;
	pPacket->msgBASE_RELOC.dwToken2 ^= dwServerIPAddress;
	pPacket->msgBASE_NORMAL.dwToken1 ^= dwServerIPAddress;
	return pPacket->msgBASE_NORMAL.dwToken1;
	return 0;
}

DWORD
CryptFlowPolicy::MakePacket::PostRecv_DefConverter( BYTE* pMsgBASE, DWORD dwCryptKey, DWORD dwSeqCryptKey )
{
	PacketTokens* pPacket = (PacketTokens*)pMsgBASE;
	pPacket->msgBASE_RELOC.dwToken2 ^= (dwCryptKey ^ dwSeqCryptKey);
	pPacket->msgBASE_NORMAL.dwToken1 ^= dwSeqCryptKey;
	return pPacket->msgBASE_NORMAL.dwToken1;
	return 0;
}

DWORD
CryptFlowPolicy::MakePacket::PreSend_DefConverter( BYTE* pMsgBASE, DWORD dwCryptKey, DWORD dwSeqCryptKey )
{
	PacketTokens* pPacket = (PacketTokens*)pMsgBASE;
	pPacket->msgBASE_NORMAL.dwToken1 ^= dwSeqCryptKey;
	pPacket->msgBASE_RELOC.dwToken2 ^= (dwCryptKey ^ dwSeqCryptKey);
	return pPacket->msgBASE_NORMAL.dwToken1;
	return 0;
}


//



// <WARNING>
//   이것은 Singleton객체가 아닙니다.
//     DWORD	m_dwMaxObjectCount;
//	   PVOID	m_AMBIGOUS;

union uConvOBJ
{
	VOID*	pINST;
	DWORD*	pDWORD;
};

// 너무 많은 수의 개수를 입력하지 마세요. 필요한 만큼만... 100개로 제한합니다.
sOBJECT_CHANGER*		sOBJECT_CHANGER::CreateObjectChanger( DWORD dwMaxObjectCount )
{
	sOBJECT_CHANGER* pInst = new sOBJECT_CHANGER;

	if( dwMaxObjectCount > 100 )
		return NULL;

	pInst->m_dwMaxObjectCount = dwMaxObjectCount;
	pInst->m_AMBIGOUS = new DWORD[dwMaxObjectCount];
	memset( pInst->m_AMBIGOUS, 0, sizeof(DWORD)*dwMaxObjectCount );

	return pInst;
}

VOID					sOBJECT_CHANGER::ReleaseObjectChanger( sOBJECT_CHANGER* pInst )
{
	if( pInst )
	{
		if( pInst->m_AMBIGOUS )
		{
			delete pInst->m_AMBIGOUS;
			pInst->m_dwMaxObjectCount = 0;
			pInst->m_AMBIGOUS = NULL;
		}
	}
}


BOOL					sOBJECT_CHANGER::ObjectChangeTo( PVOID targetObj, DWORD toObj )
{
	if( !(m_dwMaxObjectCount > toObj) )
		return FALSE;

	DWORD* m_pDWARRAY = (DWORD*)m_AMBIGOUS;
	if( m_pDWARRAY[toObj] && targetObj )
	{
		uConvOBJ uConv;
		uConv.pINST = targetObj;
		*uConv.pDWORD = m_pDWARRAY[toObj];

		return TRUE;
	}

	return FALSE;
}

BOOL					sOBJECT_CHANGER::_RegisterObject( DWORD dwAlignIndex, PVOID instanceObj )
{
	if( !(m_dwMaxObjectCount > dwAlignIndex) )
		return FALSE;

	if( instanceObj )
	{
		uConvOBJ uConv;
		uConv.pINST = instanceObj;
		DWORD dwCode = *uConv.pDWORD;

		DWORD* m_pDWARRAY = (DWORD*)m_AMBIGOUS;
		m_pDWARRAY[dwAlignIndex] = dwCode;

		return TRUE;
	}

	return FALSE;
}



