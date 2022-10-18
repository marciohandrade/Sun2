#pragma once

//=================================================================================================
// MasterServer 구조체
//	- MasterServer에서 사용하는 구조체 정의
//=================================================================================================
#include <BitsetOperation.h>

// 이것은 사용하지 않고 있다. 일단은 정의만 놔둔다. 구현부는 제거하자.
// RTT 명령 수행 제한 타입
enum eRTTARESTRICT_TYPE
{
	eRTTARESTRICT_ALL			= 0,
	eRTTARESTRICT_NOTICE,
	eRTTARESTRICT_DROPUSER,					// 현재 USERDISCONN과 겹쳐서 안 쓰이는 듯?
	eRTTARESTRICT_USERCOUNT,
	eRTTARESTRICT_BATTLEZONEINFO,
	eRTTARESTRICT_ARMORKIT,
	eRTTARESTRICT_ABUSE,
	eRTTARESTRICT_USERMONEY,
	eRTTARESTRICT_SERVERSHUTDOWN,
	eRTTARESTRICT_USERDISCONN,
	eRTTARESTRICT_GMSCOMMAND,
};

//
//
//
class MasterServerEx;
//

struct MSG_GM_OPERATION_FUNCRESTRICT_CMD;
struct MSG_MX_FUNCRESTRICT_RELOAD_ANS;
//
//
//

#ifndef __WAVERIX_FUNCTION_RESTRICT_EXTENDED__

//struct sRESTRICT_MGR
//{
//	//
//	static const DWORD mc_FUNCRESTRICT_DATABLOCK_MAX_INDEX		= 31;
//	static const DWORD mc_FUNCRESTRICT_DATABLOCK_MAX_SIZE		= mc_FUNCRESTRICT_DATABLOCK_MAX_INDEX+1;
//	static const DWORD mc_FUNCRESTRICT_DATABLOCK_ALLOWED_MASK	= eFUNCTIONALRESTRICT_ALL;
//	//
//
//	// 32개 관련 BitMask, 이 개수를 넘어가게 되면, 배열로 전환하거나 DWORD64로 전환하자.
//	DWORD	m_dwFuncRestrict;
//
//	// <METHODs>
//	inline VOID		Clear()
//	{
//		m_dwFuncRestrict = 0;
//	}
//
//	// <FUNCTION RESTRICT>
//	inline VOID		FunctionAllow( eFUNCTIONALRESTRICT_TYPE functionType )
//	{
//		DWORD dwFunc = (DWORD)functionType;
//		if( mc_FUNCRESTRICT_DATABLOCK_ALLOWED_MASK & dwFunc )
//		{
//			m_dwFuncRestrict |= dwFunc;
//		}
//	}
//
//	inline VOID		FunctionRestrict( eFUNCTIONALRESTRICT_TYPE functionType )
//	{
//		DWORD dwFunc = (DWORD)functionType;
//		if( mc_FUNCRESTRICT_DATABLOCK_ALLOWED_MASK & dwFunc )
//		{
//			m_dwFuncRestrict &= ~dwFunc;
//		}
//	}
//
//	inline BOOL		IsFunctionAllowd( eFUNCTIONALRESTRICT_TYPE functionType )
//	{
//		DWORD dwFunc = (DWORD)functionType;
//		if( mc_FUNCRESTRICT_DATABLOCK_ALLOWED_MASK & dwFunc )
//		{
//			return TRUE;
//		}
//		return FALSE;
//	}
//
//	//
//	// Packet만들기 루틴 추가할 것.
//	struct MakePacket
//	{
//		// 게임서버로 보낼 정보 만들기
//		static VOID	Make_GM_OPERATION_FUNCRESTRICT_CMD( sRESTRICT_MGR* pBLOCK, MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD );
//
//		// AccountDB로부터 받은 기능 제한 패킷을 통해 정보 설정
//		static VOID	Parse_MX_FUNCRESTRICT_RELOAD_ANS( sRESTRICT_MGR* pBLOCK, MSG_MX_FUNCRESTRICT_RELOAD_ANS* pMsgANS, BOOL bFromFile, BOOL bLogging = FALSE );
//
//		// 파일로부터 기능제한을 읽어들이는 경우 처리루틴 : Parse_MX -> Make_GM -> SendToGameServer 루틴이 포함된다.
//		static VOID Read_FUNCRESTRICT_FROM_FILE( sRESTRICT_MGR* pBLOCK, BOOL bLogging = FALSE );
//	};
//	//
//	//
//	//
//};

#else //__WAVERIX_FUNCTION_RESTRICT_EXTENDED__

struct sRESTRICT_MGR
{
#define DECL_RESTRICT_CONST_T	static const DWORD

	DECL_RESTRICT_CONST_T	FUNCRESTRICT_DATABLOCK_LAST_CODE_ID	= 25;	//eFUNCTIONALRESTRICT_TYPE의 MAX의 Shift수치와 같아야 한다.
	DECL_RESTRICT_CONST_T	FUNCRESTRICT_DATABLOCK_UPPERBOUND	= FUNCRESTRICT_DATABLOCK_LAST_CODE_ID+1;

//////////////////////////////////////////////////////////////////////////
// <FIELDs>
	util::BitSetOperation<FUNCRESTRICT_DATABLOCK_UPPERBOUND>	m_FunctionRestrictTable;

//////////////////////////////////////////////////////////////////////////
// <METHODs>
	inline VOID		Clear()
	{
		m_FunctionRestrictTable.ClearAll();
	}

	// <FUNCTION RESTRICT>
	inline VOID		FunctionAllow( eFUNCTIONALRESTRICT_TYPE functionType )
	{
		DWORD dwFunc = (DWORD)functionType;
		if( FUNCRESTRICT_DATABLOCK_UPPERBOUND > dwFunc )
			m_FunctionRestrictTable.Set( dwFunc );
	}

	inline VOID		FunctionRestrict( eFUNCTIONALRESTRICT_TYPE functionType )
	{
		DWORD dwFunc = (DWORD)functionType;
		if( FUNCRESTRICT_DATABLOCK_UPPERBOUND > dwFunc )
			m_FunctionRestrictTable.UnSet( dwFunc );
	}

	inline BOOL		IsFunctionAllowd( eFUNCTIONALRESTRICT_TYPE functionType )
	{
		DWORD dwFunc = (DWORD)functionType;
		return m_FunctionRestrictTable.IsSet( dwFunc );
	}

	//
	// Packet만들기 루틴 추가할 것.
	struct MakePacket
	{
		// 게임서버로 보낼 정보 만들기
		static VOID	Make_GM_OPERATION_FUNCRESTRICT_CMD( sRESTRICT_MGR* pBLOCK, MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD );

		// AccountDB로부터 받은 기능 제한 패킷을 통해 정보 설정
		static VOID	Parse_MX_FUNCRESTRICT_RELOAD_ANS( sRESTRICT_MGR* pBLOCK, MSG_MX_FUNCRESTRICT_RELOAD_ANS* pMsgANS, BOOL bFromFile, BOOL bLogging = FALSE );

		// 파일로부터 기능제한을 읽어들이는 경우 처리루틴 : Parse_MX -> Make_GM -> SendToGameServer 루틴이 포함된다.
		static VOID Read_FUNCRESTRICT_FROM_FILE( sRESTRICT_MGR* pBLOCK, BOOL bLogging = FALSE );
	};
	//
	//
	//
#undef DECL_RESTRICT_CONST_T
};

#endif //__WAVERIX_FUNCTION_RESTRICT_EXTENDED__

