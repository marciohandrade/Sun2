#pragma once

//=================================================================================================
// MasterServer ����ü
//	- MasterServer���� ����ϴ� ����ü ����
//=================================================================================================
#include <BitsetOperation.h>

// �̰��� ������� �ʰ� �ִ�. �ϴ��� ���Ǹ� ���д�. �����δ� ��������.
// RTT ��� ���� ���� Ÿ��
enum eRTTARESTRICT_TYPE
{
	eRTTARESTRICT_ALL			= 0,
	eRTTARESTRICT_NOTICE,
	eRTTARESTRICT_DROPUSER,					// ���� USERDISCONN�� ���ļ� �� ���̴� ��?
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
//	// 32�� ���� BitMask, �� ������ �Ѿ�� �Ǹ�, �迭�� ��ȯ�ϰų� DWORD64�� ��ȯ����.
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
//	// Packet����� ��ƾ �߰��� ��.
//	struct MakePacket
//	{
//		// ���Ӽ����� ���� ���� �����
//		static VOID	Make_GM_OPERATION_FUNCRESTRICT_CMD( sRESTRICT_MGR* pBLOCK, MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD );
//
//		// AccountDB�κ��� ���� ��� ���� ��Ŷ�� ���� ���� ����
//		static VOID	Parse_MX_FUNCRESTRICT_RELOAD_ANS( sRESTRICT_MGR* pBLOCK, MSG_MX_FUNCRESTRICT_RELOAD_ANS* pMsgANS, BOOL bFromFile, BOOL bLogging = FALSE );
//
//		// ���Ϸκ��� ��������� �о���̴� ��� ó����ƾ : Parse_MX -> Make_GM -> SendToGameServer ��ƾ�� ���Եȴ�.
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

	DECL_RESTRICT_CONST_T	FUNCRESTRICT_DATABLOCK_LAST_CODE_ID	= 25;	//eFUNCTIONALRESTRICT_TYPE�� MAX�� Shift��ġ�� ���ƾ� �Ѵ�.
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
	// Packet����� ��ƾ �߰��� ��.
	struct MakePacket
	{
		// ���Ӽ����� ���� ���� �����
		static VOID	Make_GM_OPERATION_FUNCRESTRICT_CMD( sRESTRICT_MGR* pBLOCK, MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD );

		// AccountDB�κ��� ���� ��� ���� ��Ŷ�� ���� ���� ����
		static VOID	Parse_MX_FUNCRESTRICT_RELOAD_ANS( sRESTRICT_MGR* pBLOCK, MSG_MX_FUNCRESTRICT_RELOAD_ANS* pMsgANS, BOOL bFromFile, BOOL bLogging = FALSE );

		// ���Ϸκ��� ��������� �о���̴� ��� ó����ƾ : Parse_MX -> Make_GM -> SendToGameServer ��ƾ�� ���Եȴ�.
		static VOID Read_FUNCRESTRICT_FROM_FILE( sRESTRICT_MGR* pBLOCK, BOOL bLogging = FALSE );
	};
	//
	//
	//
#undef DECL_RESTRICT_CONST_T
};

#endif //__WAVERIX_FUNCTION_RESTRICT_EXTENDED__

