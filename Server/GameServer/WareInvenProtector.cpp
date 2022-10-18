#include "stdafx.h"

#include "GameServerEx.h"

#include <PacketStruct_DG.h>

#include "Player.h"
#include "WareInvenProtector.h"

#include <Wz_MD5.h>

#include "WarehouseSlotContainer.h"

#ifdef _KR_0_20120207_CRYPT_SHA256
#include "../ProgramCommon/SHA256.h"
#endif

//////////////////////////////////////////////////////////////////////////
//

#ifdef __CN_000005_20090522_WAREHOUSE_ISACCEPTABLE_X
	#define ISALPHA_X(c)    ( ((c) == 'x') || ((c) == 'X') )
	#define ISDIGIT(c)		( ((c) >= '0') && ((c) <= '9') )
#endif

BOOL		sPASSWORD_BLOCK::IsAcceptableFormat( CHAR* pwd )
{
	if( !pwd ) return FALSE;

	BOOL bAccept = FALSE;
	CHAR* pOffset = pwd;
	INT iStrLen = 0;
	CHAR chr;

	while( (chr = *pOffset) && ( ++iStrLen < (8+1) ) )
	{
	#ifdef __CN_000005_20090522_WAREHOUSE_ISACCEPTABLE_X
		if( ISDIGIT(chr) ||	ISALPHA_X(chr) )
	#else
		if( ! ((chr<'0') || (chr>'9')) )
	#endif	
		{
			bAccept = TRUE;
			++pOffset;
			continue;
		}

		bAccept = FALSE;
		break;
	}

	if( bAccept )
	{
		if( (iStrLen < 4) || (iStrLen > 8) )
			return FALSE;
	}

	return bAccept;
}

//
//
BOOL		ProtectorWarehouse::TEST_Transaction_PWD_CONFIG( BOOL bIsChangeEnable )
{
	BOOL bRet = CONTROL_FIELD()[eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION];
	bRet = bIsChangeEnable ? !bRet : bRet;

	//( !CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION ) ? TRUE : FALSE )
	//: ( CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION ) ? TRUE : FALSE );

	return bRet;
}

// 비번 확인중일 경우, 비번 설정중일 경우
BOOL		ProtectorWarehouse::TESTnSET_Transaction_PWD_CONFIG( BOOL bIsEnable )
{
	BOOL bRet = TEST_Transaction_PWD_CONFIG( bIsEnable );

	if( bRet )
	{
		if( bIsEnable )	CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION );
		else			CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_PWD_TRANSACTION );

		return TRUE;
	}

	return FALSE;
}

VOID		ProtectorWarehouse::Block( BOOL bIsBlock, BOOL bDBUpdate )
{
	if( bIsBlock )
		CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );
	else
		CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::WAREHOUSE_USABLE );

	if( bDBUpdate )
	{
		MSG_DG_WAREHOUSE_FLAG_STREAM_NTF msgNTF;
		msgNTF.m_dwKey = PLAYER().GetUserGuid();
		//printf( "DEBUG - CURR WARE %u %08X\n", bIsBlock, GetControlFieldStream() );
		msgNTF.m_dwFlagStream = GetControlFieldStream();
		g_pGameServer->SendToServer( GAME_DBPROXY, &msgNTF, sizeof(msgNTF) );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//
//
BOOL			ProtectorInventory::TEST_Transaction_PWD_CONFIG( BOOL bIsChangeEnable )
{
	BOOL bRet = CONTROL_FIELD()[eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_PWD_TRANSACTION];
	bRet = bIsChangeEnable ? !bRet : bRet;

	//BOOL bRet = bIsEnable ?
	//	( !CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_PWD_TRANSACTION ) ? TRUE : FALSE )
	//	: ( CONTROL_FIELD().IsSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_PWD_TRANSACTION ) ? TRUE : FALSE );

	return bRet;
}

// 비번 확인중일 경우, 비번 설정 중일 경우
BOOL			ProtectorInventory::TESTnSET_Transaction_PWD_CONFIG( BOOL bIsEnable )
{
	BOOL bRet = TEST_Transaction_PWD_CONFIG( bIsEnable );

	if( bRet )
	{
		if( bIsEnable )	CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_PWD_TRANSACTION );
		else			CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_PWD_TRANSACTION );

		return TRUE;
	}

	return FALSE;
}

// 유저 정보를 가져오는 시점에 비번 유무 확인 후 이 루틴을 수행한다.
VOID			ProtectorInventory::Block( BOOL bIsBlock, BOOL bDBUpdate )
{
	if( bIsBlock )
		CONTROL_FIELD().UnSet( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );
	else
		CONTROL_FIELD().Set( eWARE_INVEN_FILTER_FLAG_INFO::INVENTORY_USABLE );

	if( bDBUpdate )
	{
		MSG_DG_ITEM_INVENTORY_FLAG_STREAM_NTF msgNTF;
		msgNTF.m_dwKey = PLAYER().GetUserGuid();
		//printf( "DEBUG - CURR INVEN %u %08X\n", bIsBlock, GetControlFieldStream() );
		msgNTF.m_dwFlagStream = GetControlFieldStream();
		g_pGameServer->SendToServer( GAME_DBPROXY, &msgNTF, sizeof(msgNTF) );
	}
}

BOOL	WarehouseInventoryProtector::IsEqualSSN( BYTE psSSN[MAX_SSN_STREAM_LENGTH] )
{
#ifdef _KR_0_20120207_CRYPT_SHA256
    int mode = g_pGameServer->GetMode();
    switch (mode)
    {
    case eSERVER_DEV:
        {
            BYTE digest[MAX_SSN_ENCODED_STREAM_LENGTH+1] = { 0, };
            wchar_t wszUserLastSSN[MAX_SSN_STREAM_LENGTH+1];
            wchar_t wszUserGUID[64];

            ZeroMemory( wszUserLastSSN, sizeof(wszUserLastSSN) );
            ZeroMemory( wszUserGUID, sizeof(wszUserGUID) );

            //주민등록 번호 뒷자리는 최대 14자리로 한다.
            for( INT i=0 ; i<MAX_SSN_STREAM_LENGTH ; ++i )
                wszUserLastSSN[i] = (wchar_t)psSSN[i];

            _ultow( USER().GetUserGuid(), wszUserGUID, 10 );

            MD5::EncodeString( (BYTE*)wszUserLastSSN, (INT)wcslen(wszUserLastSSN)*2, digest, (BYTE*)wszUserGUID, (int)wcslen(wszUserGUID)*2 );
            if( 0 == strncmp( (CHAR*)SSN_ENCODED_STREAM().GetBUFFER(), (CHAR*)digest, MAX_SSN_ENCODED_STREAM_LENGTH ) )
                return TRUE;
        }
        break;
    case eSERVER_LIVE:
    case eSERVER_TESTLIVE:
    case eSERVER_INTRAOFFICE:
        {
            Crypt::SHA256 sha256;
            bool result = sha256.Compare((const char*)psSSN, "WEBZEN", (const char*)SSN_ENCODED_STREAM().GetBUFFER());
            return result;
        }
        break;
    default:
        return false;
    }
#else
    BYTE digest[MAX_SSN_ENCODED_STREAM_LENGTH+1] = { 0, };
    wchar_t wszUserLastSSN[MAX_SSN_STREAM_LENGTH+1];
    wchar_t wszUserGUID[64];

    ZeroMemory( wszUserLastSSN, sizeof(wszUserLastSSN) );
    ZeroMemory( wszUserGUID, sizeof(wszUserGUID) );

    //주민등록 번호 뒷자리는 최대 14자리로 한다.
    for( INT i=0 ; i<MAX_SSN_STREAM_LENGTH ; ++i )
        wszUserLastSSN[i] = (wchar_t)psSSN[i];

    _ultow( USER().GetUserGuid(), wszUserGUID, 10 );

	MD5::EncodeString( (BYTE*)wszUserLastSSN, (INT)wcslen(wszUserLastSSN)*2, digest, (BYTE*)wszUserGUID, (int)wcslen(wszUserGUID)*2 );
	if( 0 == strncmp( (CHAR*)SSN_ENCODED_STREAM().GetBUFFER(), (CHAR*)digest, MAX_SSN_ENCODED_STREAM_LENGTH ) )
		return TRUE;
#endif

	return FALSE;
}

BOOL	WarehouseInventoryProtector::OnRecvDG_WAREHOUSE_PWD_SETTING_ACK( MSG_DG_WAREHOUSE_PWD_SETTING_ACK* pMsg, WORD wSize )
{
	MSG_DG_WAREHOUSE_PWD_SETTING_ACK::eRET_CODE eRET
		= (MSG_DG_WAREHOUSE_PWD_SETTING_ACK::eRET_CODE)pMsg->ReturnCode();

	BOOL bProcessed = FALSE;

	ProtectorWarehouse& rWAREHOUSE = WAREHOUSE();
	ProtectorInventory& rINVENTORY = INVENTORY();

	if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_START_FAIL == eRET )
	{
		MSG_CG_WAREHOUSE_PWD_SETTING_NAK msgNAK;
		rWAREHOUSE.Block( TRUE );	// 일단은 문제점이 있는 상황이므로 잠궈버리자.
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	// 현재 창고 비번 설정관련 트랜젝션 중이면...
	if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_WAREHOUSE == pMsg->TypeCode()
		&& rWAREHOUSE.TEST_Transaction_PWD_CONFIG( FALSE ) )
	{
		if( rWAREHOUSE.PasswordTransactionMode() == sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION )
		{
			if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS == eRET )
			{
				MSG_CG_WAREHOUSE_PWD_SETTING_ACK msgACK;
				msgACK.ReturnCode() = ('\0' == pMsg->m_Password[0]) ?
					MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS_CLEAR : MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS;
				rWAREHOUSE.SetPasswordFromDB( pMsg->m_Password );
				rWAREHOUSE.Block( FALSE );
				USER().SendPacket( &msgACK, sizeof(msgACK) );
			}
			else //if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL == bSuccess )
			{
				MSG_CG_WAREHOUSE_PWD_SETTING_NAK msgNAK;
				//msgNAK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
				rWAREHOUSE.Block( TRUE );
				USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			}

			bProcessed = TRUE;
			rWAREHOUSE.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
			rWAREHOUSE.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		}
	}

	// 현재 인벤토리 비번 설정관련 트랜젝션 중이면...
	if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_INVENTORY == pMsg->TypeCode()
		&& rINVENTORY.TEST_Transaction_PWD_CONFIG( FALSE ) )
	{
		if( rINVENTORY.PasswordTransactionMode() == sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION )
		{
			if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS == eRET )
			{
				MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK msgACK;
				msgACK.ReturnCode() = ('\0' == pMsg->m_Password[0]) ?
					MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_SUCCESS_CLEAR : MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_SUCCESS;
				rINVENTORY.SetPasswordFromDB( pMsg->m_Password );
				BASE_PLAYERINFO* pINFO = USER().GetCharInfo();
				strncpy( pINFO->m_InventoryPwd, rINVENTORY.GetPassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
				rINVENTORY.Block( FALSE );

				USER().SendPacket( &msgACK, sizeof(msgACK) );
			}
			else //if( MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL == bSuccess )
			{
				MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK msgNAK;
				//msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
				rINVENTORY.SetPasswordFromDB( pMsg->m_Password );
				BASE_PLAYERINFO* pINFO = USER().GetCharInfo();
				strncpy( pINFO->m_InventoryPwd, rINVENTORY.GetPassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
				rINVENTORY.Block( TRUE );

				USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			}

			bProcessed = TRUE;
			rINVENTORY.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
			rINVENTORY.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		}
	}

	if( !bProcessed )	// 양쪽 어디도 아니다... 가능성 여부 타전
	{
		SUNLOG( eCRITICAL_LOG,
			_T("암호 설정 DB 질의 및 응답 받은 상태지만, 클라이언트의 암호 설정 트랜잭션 상태가 이상하다. 로직 버그 찾을 것! (성공여부: %d)" ),
			bProcessed );
		return FALSE;
	}

	return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_WAREHOUSE_PWD_LOCKING_SYN( MSG_CG_WAREHOUSE_PWD_LOCKING_SYN* pMsg, WORD wSize )
{
	// 사이즈 검사 필요하다.
	ProtectorWarehouse& rPROTECT = WAREHOUSE();

	// 창고 시작도 안했는데, 패킷을 보내는 경우 방지 목적
	CWarehouseSlotContainer* pContainer = USER().GetWarehouseContainer();
	BOOL bValidStatus = ( pContainer->IsStarted() );
	if( !bValidStatus )
	{
		MSG_CG_WAREHOUSE_PWD_LOCKING_NAK msgNAK;

		msgNAK.ErrorCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_NAK::IRREGULAR_LOCKING_POLICYMENT;	//수정할 것
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return TRUE;
	}

	BOOL bExistPwd = (BOOL) !rPROTECT.IsEmptyPassword();
	BOOL bUsable = rPROTECT.IsUsable();

	MSG_CG_WAREHOUSE_PWD_LOCKING_ACK msgACK;

	if( bUsable )
	{
		if( bExistPwd )
		{
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_SUCCESS_LOCKED;
			rPROTECT.Block(TRUE);
		}
		else
		{
			// (WAVERIX) WAREINVEN - CHANGE LOGIC
			//   RETCODE: NEED_PWD_FOR_LOCKING
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_LOCKING;
		}
	}
	else
	{
		if( bExistPwd )
		{
			// (WAVERIX) WAREINVEN - CHANGE LOGIC
			//   Un-Locking Process
			//   RETCODE : NEED_PWD_CHANGE_LOCK_TO_UNLOCK
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_UNLOCKING;
		}
		else
		{
			MSG_CG_WAREHOUSE_PWD_LOCKING_NAK msgNAK;

			msgNAK.ErrorCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_NAK::IRREGULAR_LOCKING_POLICYMENT;
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return TRUE;
		}
	}

	rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
	rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
	USER().SendPacket( &msgACK, sizeof(msgACK) );

	return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_WAREHOUSE_PWD_SETTING_SYN( MSG_CG_WAREHOUSE_PWD_SETTING_SYN* pMsg, WORD wSize )
{
	// [전제] ~PWD_SETTING_SYN 패킷은
	//   열림상태에서 처음잠김 상태 요청시만 해당사항으로 취급한다.
	//   (초기 상태) 열림상태, 비번없음
	// 크기 검사
	BOOL bSizeMIN = (BOOL)(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_WAREHOUSE_PWD_SETTING_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorWarehouse& rPROTECT = WAREHOUSE();

	MSG_CG_WAREHOUSE_PWD_SETTING_SYN::eREQ_CODE eREQ = (MSG_CG_WAREHOUSE_PWD_SETTING_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_WAREHOUSE_PWD_SETTING_NAK msgNAK;

	// [전제] 앞에서 언급했음, 열림상태여야 한다.
	// [전제] 여기로 들어온다는 것은 비번이 없어야 한다.
	// 창고 시작도 안했는데, 패킷을 보내는 경우 방지 목적
	BOOL bUsable = rPROTECT.IsUsable();
	CWarehouseSlotContainer* pContainer = USER().GetWarehouseContainer();
	BOOL bValidStatus = ( bUsable && pContainer->IsStarted() );

    if( bValidStatus )
	{
		switch( eREQ )
		{
		case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_CANCEL:
		case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN:
			break;
		default:
			bValidStatus = rPROTECT.IsEmptyPassword();
			break;
		}
	}

	if( !bValidStatus )
	{
		//msgNAK.ErrorCode() = MSG_CG_WAREHOUSE_PWD_SETTING_NAK;
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	if( MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_FIRST != eREQ )
	{	// 처음 패킷이 아닌 이상 모두 트랜젝션이 걸려있어야 한다.
		if( rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
		{
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return FALSE;
		}
	}

	MSG_CG_WAREHOUSE_PWD_SETTING_ACK msgACK;
	util::cSTRING_REF strRD( pMsg->m_PWDnSSN.GetBUFFER(), pMsg->m_PWDnSSN.GetBUFFER_LENGTH() );
	util::cSTRING_REF_RW_WITH_BUFFER<MAX_SSN_STREAM_LENGTH+1> tmpBuffer;

	switch( eREQ )
	{
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_FIRST:
		{
			if( !bSizeMIN )	return FALSE;

			// 다음 조건을 만족한다는 것은 이미 비번 관련 트랜젝션이 진행중인 상태다.
			if( !rPROTECT.TESTnSET_Transaction_PWD_CONFIG( TRUE ) )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}

			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_NEED_PWD;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_CLEAR:
			// 여기서는 활성화 되지 않는 기능을 요청했다.
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			//	CHECK_CMP_SSN trans상태에서 취소가 오는 경우, 비번은 비어있는 상태에서 임시 로컬로 저장된 상태이며,
			//	따라서 로컬 비번을 초기화 하면 이전 상태로 복구될 것이다.
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN == rPROTECT.PasswordTransactionMode() )
			{
				CHAR emptyChar[1] = { 0 };
				rPROTECT.SetPasswordFromDB( emptyChar );
			}

            msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_CANCELED;
			goto PROCESS_CHECK_END;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_PWD:
		{
			if( !bSizeMAX )	return FALSE;

			// 비번없음 상태에서 설정할 비번이 입력된 상황
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( !sPASSWORD_BLOCK::IsAcceptableFormat( tmpBuffer.GetBUFFER() ) )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_INVALID_PWD;
				goto PROCESS_CHECK_END;
			}

			// 로컬은 갱신해 둔다.
			rPROTECT.SetPasswordFromDB( tmpBuffer.GetBUFFER() );

			// 현재 인정된 비번에 대한 적용을 위한 SSN 확인 요청
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->잠김해제 (비번 초기화) 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;
				// DB에 비번 적용시킨다.
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_WAREHOUSE;
				msgSYN.Set( rPROTECT.GetPassword() );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// 성공시 다음과 같은 코드를 적용해야 한다.
				//msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_SSN;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	}

PROCESS_CHECK_END:
	{
		rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
		rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		USER().SendPacket( &msgACK, sizeof(msgACK) );
	} return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_WAREHOUSE_PWD_CHECK_SYN( MSG_CG_WAREHOUSE_PWD_CHECK_SYN* pMsg, WORD wSize )
{
	// [전제] ~PWD_CHECK_SYN 패킷은
	//   잠김상태(비번존재, 임시잠김)에서 잠김해제 요청시만 해당사항으로 취급한다.
	// 크기 검사
	BOOL bSizeMIN = (BOOL)(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_WAREHOUSE_PWD_CHECK_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorWarehouse& rPROTECT = WAREHOUSE();
	MSG_CG_WAREHOUSE_PWD_CHECK_SYN::eREQ_CODE eREQ = (MSG_CG_WAREHOUSE_PWD_CHECK_SYN::eREQ_CODE)pMsg->ReqCode();

	MSG_CG_WAREHOUSE_PWD_CHECK_NAK msgNAK;

	// [전제] 앞에서 언급했음, 잠김상태여야 한다.
	// [전제] 여기로 들어온다는 것은 비번이 없는 상태가 되어서는 안된다.
	// 창고 시작도 안했는데, 패킷을 보내는 경우 방지 목적
	BOOL bUsable = rPROTECT.IsUsable();
	CWarehouseSlotContainer* pContainer = USER().GetWarehouseContainer();
	BOOL bValidStatus = ( !bUsable && pContainer->IsStarted() && !rPROTECT.IsEmptyPassword() );
	if( !bValidStatus )
	{
		//msgNAK.ErrorCode() = MSG_CG_WAREHOUSE_PWD_CHECK_NAK;
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	if( MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_FIRST != eREQ )
	{	// 처음 패킷이 아닌 이상 모두 트랜젝션이 걸려있어야 한다.
		// 아래의 질의는 다음과 같다. Transaction을 걸수 있는가? TRUE라면, 걸 수 있다는 의미이며,
		// 따라서 현재 Transaction이 걸려있지 않는 상황이 된다.
		if( rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
		{
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return FALSE;
		}
	}

	MSG_CG_WAREHOUSE_PWD_CHECK_ACK msgACK;
	util::cSTRING_REF strRD( pMsg->m_PWDnSSN.GetBUFFER(), pMsg->m_PWDnSSN.GetBUFFER_LENGTH() );
	util::cSTRING_REF_RW_WITH_BUFFER<MAX_SSN_STREAM_LENGTH+1> tmpBuffer;

	switch( eREQ )
	{
	case MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_FIRST:
		{
			if( !bSizeMIN )	return FALSE;

			// 다음 조건을 만족한다는 것은 이미 비번 관련 트랜젝션이 진행중인 상태다.
			if( !rPROTECT.TESTnSET_Transaction_PWD_CONFIG( TRUE ) )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}

			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_PWD;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_CLEAR:
		{
			if( !bSizeMIN )	return FALSE;

			// 비번 관련 트랜젝션 중이어야 한다.
			//if( !rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
			//{
			//	msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
			//	goto PROCESS_CHECK_END;
			//}

			// 비번 초기화 관련한 처리를 수행해야 한다.
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_CANCELED;
			goto PROCESS_CHECK_END;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_WITH_PWD:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->열림상태 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( rPROTECT.IsEqualPassword( tmpBuffer.GetBUFFER() ) )
			{	// 열림상태로 전환
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				rPROTECT.Block( FALSE );
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
			else
			{	// 비번 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_PWD;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->잠김해제 (비번 초기화) 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{	// 열림상태로 전환
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;

				// <WARINING> DB로 상태 전환시키고, 비번 초기화 시켜야 한다.DB&LOCAL
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_WAREHOUSE;
				CHAR emptyChar[1] = { 0 };
				msgSYN.Set( emptyChar );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// 성공시 다음과 같은 패킷을 전송해야 한다.
				//msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_SSN;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	}

PROCESS_CHECK_END:
	{
		rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
		rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		USER().SendPacket( &msgACK, sizeof(msgACK) );
	} return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_ITEM_INVENTORY_PWD_LOCKING_SYN( MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN* pMsg, WORD wSize )
{
	// 사이즈 검사 필요하다.
	ProtectorInventory& rPROTECT = INVENTORY();

	BOOL bExistPwd = (BOOL) !rPROTECT.IsEmptyPassword();
	BOOL bUsable = rPROTECT.IsUsable();

	MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK msgACK;

	if( bUsable )
	{
		if( bExistPwd )
		{
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_SUCCESS_LOCKED;
			rPROTECT.Block(TRUE);
		}
		else
		{
			// (WAVERIX) WAREINVEN - CHANGE LOGIC
			//   RETCODE: NEED_PWD_FOR_LOCKING
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_LOCKING;
		}
	}
	else
	{
		if( bExistPwd )
		{
			// (WAVERIX) WAREINVEN - CHANGE LOGIC
			//   Un-Locking Process
			//   RETCODE : NEED_PWD_CHANGE_LOCK_TO_UNLOCK
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_UNLOCKING;
		}
		else
		{
			MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK msgNAK;

			msgNAK.ErrorCode() = MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK::IRREGULAR_LOCKING_POLICYMENT;
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return TRUE;
		}
	}

	rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
	rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
	USER().SendPacket( &msgACK, sizeof(msgACK) );

	return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_ITEM_INVENTORY_PWD_SETTING_SYN( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN* pMsg, WORD wSize )
{
	// [전제] ~PWD_SETTING_SYN 패킷은
	//   열림상태에서 처음잠김 상태 요청시만 해당사항으로 취급한다.
	//   (초기 상태) 열림상태, 비번없음
	// 크기 검사
	BOOL bSizeMIN = (BOOL)(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorInventory& rPROTECT = INVENTORY();

	MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::eREQ_CODE eREQ = (MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK msgNAK;

	// [전제] 앞에서 언급했음, 열림상태여야 한다.
	// [전제] 여기로 들어온다는 것은 비번이 없어야 한다.
	BOOL bValidStatus = rPROTECT.IsUsable();

    if( bValidStatus )
	{
		switch( eREQ )
		{
		case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_CANCEL:
		case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_SSN:
			break;
		default:
			bValidStatus = rPROTECT.IsEmptyPassword();
			break;
		}
	}

	if( !bValidStatus )
	{
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	if( MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_FIRST != eREQ )
	{	// 처음 패킷이 아닌 이상 모두 트랜젝션이 걸려있어야 한다.
		if( rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
		{
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return FALSE;
		}
	}

	MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK msgACK;
	util::cSTRING_REF strRD( pMsg->m_PWDnSSN.GetBUFFER(), pMsg->m_PWDnSSN.GetBUFFER_LENGTH() );
	util::cSTRING_REF_RW_WITH_BUFFER<MAX_SSN_STREAM_LENGTH+1> tmpBuffer;

	switch( eREQ )
	{
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_FIRST:
		{
			if( !bSizeMIN )	return FALSE;

			// 다음 조건을 만족한다는 것은 이미 비번 관련 트랜젝션이 진행중인 상태다.
			if( !rPROTECT.TESTnSET_Transaction_PWD_CONFIG( TRUE ) )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}

			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_NEED_PWD;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_CLEAR:
		// 여기서는 활성화 되지 않는 기능을 요청했다.
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			//	CHECK_CMP_SSN trans상태에서 취소가 오는 경우, 비번은 비어있는 상태에서 임시 로컬로 저장된 상태이며,
			//	따라서 로컬 비번을 초기화 하면 이전 상태로 복구될 것이다.
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN == rPROTECT.PasswordTransactionMode() )
			{
				CHAR emptyChar[1] = { 0 };
				rPROTECT.SetPasswordFromDB( emptyChar );
			}

            msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_CANCELED;
			goto PROCESS_CHECK_END;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_PWD:
		{
			if( !bSizeMAX )	return FALSE;

			// 비번없음 상태에서 설정할 비번이 입력된 상황
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( !sPASSWORD_BLOCK::IsAcceptableFormat( tmpBuffer.GetBUFFER() ) )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_INVALID_PWD;
				goto PROCESS_CHECK_END;
			}

			// 로컬은 갱신해 둔다.
			rPROTECT.SetPasswordFromDB( tmpBuffer.GetBUFFER() );

			// 현재 인정된 비번에 대한 적용을 위한 SSN 확인 요청
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->잠김해제 (비번 초기화) 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_SETTING_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;
				// DB에 비번 적용시킨다.
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_INVENTORY;
				msgSYN.Set( rPROTECT.GetPassword() );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// 성공시 다음과 같은 코드를 적용해야 한다.
				//msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_INVALID_SSN;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	}

PROCESS_CHECK_END:
	{
		rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
		rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		USER().SendPacket( &msgACK, sizeof(msgACK) );
	} return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_ITEM_INVENTORY_PWD_CHECK_SYN( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN* pMsg, WORD wSize )
{
	// [전제] ~PWD_CHECK_SYN 패킷은
	//   잠김상태(비번존재, 임시잠김)에서 잠김해제 요청시만 해당사항으로 취급한다.
	// 크기 검사
	BOOL bSizeMIN = (BOOL)(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorInventory& rPROTECT = INVENTORY();

	MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::eREQ_CODE eREQ = (MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_NAK msgNAK;

	// [전제] 앞에서 언급했음, 잠김상태여야 한다.
	// [전제] 여기로 들어온다는 것은 비번이 없어야 한다.
	BOOL bUsable = rPROTECT.IsUsable();
	BOOL bValidStatus = ( !bUsable && !rPROTECT.IsEmptyPassword() );
	if( !bValidStatus )
	{
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	if( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_FIRST != eREQ )
	{	// 처음 패킷이 아닌 이상 모두 트랜젝션이 걸려있어야 한다.
		// 아래의 질의는 다음과 같다. Transaction을 걸수 있는가? TRUE라면, 걸 수 있다는 의미이며,
		// 따라서 현재 Transaction이 걸려있지 않는 상황이 된다.
		if( rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
		{
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
			return FALSE;
		}
	}

	MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK msgACK;
	util::cSTRING_REF strRD( pMsg->m_PWDnSSN.GetBUFFER(), pMsg->m_PWDnSSN.GetBUFFER_LENGTH() );
	util::cSTRING_REF_RW_WITH_BUFFER<MAX_SSN_STREAM_LENGTH+1> tmpBuffer;

	switch( eREQ )
	{
	case MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_FIRST:
		{
			if( !bSizeMIN )	return FALSE;

			// 다음 조건을 만족한다는 것은 이미 비번 관련 트랜젝션이 진행중인 상태다.
			if( !rPROTECT.TESTnSET_Transaction_PWD_CONFIG( TRUE ) )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}

			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_PWD;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_CLEAR:
		{
			if( !bSizeMIN )	return FALSE;

			// 비번 관련 트랜젝션 중이어야 한다.
			//if( !rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
			//{
			//	msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
			//	goto PROCESS_CHECK_END;
			//}

			// 비번 초기화 관련한 처리를 수행해야 한다.
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_CANCELED;
			goto PROCESS_CHECK_END;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_WITH_PWD:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->열림상태 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( rPROTECT.IsEqualPassword( tmpBuffer.GetBUFFER() ) )
			{	// 열림상태로 전환
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				rPROTECT.Block( FALSE );
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
			else
			{	// 비번 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_INVALID_PWD;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// 잠김상태->잠김해제 (비번 초기화) 전환을 위한 것임
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{	// 열림상태로 전환
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;

				// <WARINING> DB로 상태 전환시키고, 비번 초기화 시켜야 한다.DB&LOCAL
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_INVENTORY;
				CHAR emptyChar[1] = { 0 };
				msgSYN.Set( emptyChar );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// 성공시 다음과 같은 패킷을 전송해야 한다.
				//msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN 틀렸을 경우 재차 요청한다
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_INVALID_SSN;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
		}
		break;
	}

PROCESS_CHECK_END:
	{
		rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
		rPROTECT.TESTnSET_Transaction_PWD_CONFIG( FALSE );
		USER().SendPacket( &msgACK, sizeof(msgACK) );
	} return TRUE;
}



