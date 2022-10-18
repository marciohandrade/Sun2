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

// ��� Ȯ������ ���, ��� �������� ���
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

// ��� Ȯ������ ���, ��� ���� ���� ���
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

// ���� ������ �������� ������ ��� ���� Ȯ�� �� �� ��ƾ�� �����Ѵ�.
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

            //�ֹε�� ��ȣ ���ڸ��� �ִ� 14�ڸ��� �Ѵ�.
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

    //�ֹε�� ��ȣ ���ڸ��� �ִ� 14�ڸ��� �Ѵ�.
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
		rWAREHOUSE.Block( TRUE );	// �ϴ��� �������� �ִ� ��Ȳ�̹Ƿ� ��Ź�����.
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	// ���� â�� ��� �������� Ʈ������ ���̸�...
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

	// ���� �κ��丮 ��� �������� Ʈ������ ���̸�...
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

	if( !bProcessed )	// ���� ��� �ƴϴ�... ���ɼ� ���� Ÿ��
	{
		SUNLOG( eCRITICAL_LOG,
			_T("��ȣ ���� DB ���� �� ���� ���� ��������, Ŭ���̾�Ʈ�� ��ȣ ���� Ʈ����� ���°� �̻��ϴ�. ���� ���� ã�� ��! (��������: %d)" ),
			bProcessed );
		return FALSE;
	}

	return TRUE;
}

BOOL	WarehouseInventoryProtector::OnRecvCG_WAREHOUSE_PWD_LOCKING_SYN( MSG_CG_WAREHOUSE_PWD_LOCKING_SYN* pMsg, WORD wSize )
{
	// ������ �˻� �ʿ��ϴ�.
	ProtectorWarehouse& rPROTECT = WAREHOUSE();

	// â�� ���۵� ���ߴµ�, ��Ŷ�� ������ ��� ���� ����
	CWarehouseSlotContainer* pContainer = USER().GetWarehouseContainer();
	BOOL bValidStatus = ( pContainer->IsStarted() );
	if( !bValidStatus )
	{
		MSG_CG_WAREHOUSE_PWD_LOCKING_NAK msgNAK;

		msgNAK.ErrorCode() = MSG_CG_WAREHOUSE_PWD_LOCKING_NAK::IRREGULAR_LOCKING_POLICYMENT;	//������ ��
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
	// [����] ~PWD_SETTING_SYN ��Ŷ��
	//   �������¿��� ó����� ���� ��û�ø� �ش�������� ����Ѵ�.
	//   (�ʱ� ����) ��������, �������
	// ũ�� �˻�
	BOOL bSizeMIN = (BOOL)(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_WAREHOUSE_PWD_SETTING_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorWarehouse& rPROTECT = WAREHOUSE();

	MSG_CG_WAREHOUSE_PWD_SETTING_SYN::eREQ_CODE eREQ = (MSG_CG_WAREHOUSE_PWD_SETTING_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_WAREHOUSE_PWD_SETTING_NAK msgNAK;

	// [����] �տ��� �������, �������¿��� �Ѵ�.
	// [����] ����� ���´ٴ� ���� ����� ����� �Ѵ�.
	// â�� ���۵� ���ߴµ�, ��Ŷ�� ������ ��� ���� ����
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
	{	// ó�� ��Ŷ�� �ƴ� �̻� ��� Ʈ�������� �ɷ��־�� �Ѵ�.
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

			// ���� ������ �����Ѵٴ� ���� �̹� ��� ���� Ʈ�������� �������� ���´�.
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
			// ���⼭�� Ȱ��ȭ ���� �ʴ� ����� ��û�ߴ�.
			USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			//	CHECK_CMP_SSN trans���¿��� ��Ұ� ���� ���, ����� ����ִ� ���¿��� �ӽ� ���÷� ����� �����̸�,
			//	���� ���� ����� �ʱ�ȭ �ϸ� ���� ���·� ������ ���̴�.
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

			// ������� ���¿��� ������ ����� �Էµ� ��Ȳ
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

			// ������ ������ �д�.
			rPROTECT.SetPasswordFromDB( tmpBuffer.GetBUFFER() );

			// ���� ������ ����� ���� ������ ���� SSN Ȯ�� ��û
			msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// ������->������� (��� �ʱ�ȭ) ��ȯ�� ���� ����
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
				// DB�� ��� �����Ų��.
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_WAREHOUSE;
				msgSYN.Set( rPROTECT.GetPassword() );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// ������ ������ ���� �ڵ带 �����ؾ� �Ѵ�.
				//msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN Ʋ���� ��� ���� ��û�Ѵ�
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
	// [����] ~PWD_CHECK_SYN ��Ŷ��
	//   ������(�������, �ӽ����)���� ������� ��û�ø� �ش�������� ����Ѵ�.
	// ũ�� �˻�
	BOOL bSizeMIN = (BOOL)(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_WAREHOUSE_PWD_CHECK_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorWarehouse& rPROTECT = WAREHOUSE();
	MSG_CG_WAREHOUSE_PWD_CHECK_SYN::eREQ_CODE eREQ = (MSG_CG_WAREHOUSE_PWD_CHECK_SYN::eREQ_CODE)pMsg->ReqCode();

	MSG_CG_WAREHOUSE_PWD_CHECK_NAK msgNAK;

	// [����] �տ��� �������, �����¿��� �Ѵ�.
	// [����] ����� ���´ٴ� ���� ����� ���� ���°� �Ǿ�� �ȵȴ�.
	// â�� ���۵� ���ߴµ�, ��Ŷ�� ������ ��� ���� ����
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
	{	// ó�� ��Ŷ�� �ƴ� �̻� ��� Ʈ�������� �ɷ��־�� �Ѵ�.
		// �Ʒ��� ���Ǵ� ������ ����. Transaction�� �ɼ� �ִ°�? TRUE���, �� �� �ִٴ� �ǹ��̸�,
		// ���� ���� Transaction�� �ɷ����� �ʴ� ��Ȳ�� �ȴ�.
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

			// ���� ������ �����Ѵٴ� ���� �̹� ��� ���� Ʈ�������� �������� ���´�.
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

			// ��� ���� Ʈ������ ���̾�� �Ѵ�.
			//if( !rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
			//{
			//	msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
			//	goto PROCESS_CHECK_END;
			//}

			// ��� �ʱ�ȭ ������ ó���� �����ؾ� �Ѵ�.
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

			// ������->�������� ��ȯ�� ���� ����
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( rPROTECT.IsEqualPassword( tmpBuffer.GetBUFFER() ) )
			{	// �������·� ��ȯ
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				rPROTECT.Block( FALSE );
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
			else
			{	// ��� Ʋ���� ��� ���� ��û�Ѵ�
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

			// ������->������� (��� �ʱ�ȭ) ��ȯ�� ���� ����
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{	// �������·� ��ȯ
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;

				// <WARINING> DB�� ���� ��ȯ��Ű��, ��� �ʱ�ȭ ���Ѿ� �Ѵ�.DB&LOCAL
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_WAREHOUSE;
				CHAR emptyChar[1] = { 0 };
				msgSYN.Set( emptyChar );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// ������ ������ ���� ��Ŷ�� �����ؾ� �Ѵ�.
				//msgACK.ReturnCode() = MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN Ʋ���� ��� ���� ��û�Ѵ�
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
	// ������ �˻� �ʿ��ϴ�.
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
	// [����] ~PWD_SETTING_SYN ��Ŷ��
	//   �������¿��� ó����� ���� ��û�ø� �ش�������� ����Ѵ�.
	//   (�ʱ� ����) ��������, �������
	// ũ�� �˻�
	BOOL bSizeMIN = (BOOL)(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorInventory& rPROTECT = INVENTORY();

	MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::eREQ_CODE eREQ = (MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK msgNAK;

	// [����] �տ��� �������, �������¿��� �Ѵ�.
	// [����] ����� ���´ٴ� ���� ����� ����� �Ѵ�.
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
	{	// ó�� ��Ŷ�� �ƴ� �̻� ��� Ʈ�������� �ɷ��־�� �Ѵ�.
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

			// ���� ������ �����Ѵٴ� ���� �̹� ��� ���� Ʈ�������� �������� ���´�.
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
		// ���⼭�� Ȱ��ȭ ���� �ʴ� ����� ��û�ߴ�.
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_CANCEL:
		{
			if( !bSizeMIN )	return FALSE;

			//	CHECK_CMP_SSN trans���¿��� ��Ұ� ���� ���, ����� ����ִ� ���¿��� �ӽ� ���÷� ����� �����̸�,
			//	���� ���� ����� �ʱ�ȭ �ϸ� ���� ���·� ������ ���̴�.
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

			// ������� ���¿��� ������ ����� �Էµ� ��Ȳ
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

			// ������ ������ �д�.
			rPROTECT.SetPasswordFromDB( tmpBuffer.GetBUFFER() );

			// ���� ������ ����� ���� ������ ���� SSN Ȯ�� ��û
			msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_NEED_SSN;
			rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN;
			USER().SendPacket( &msgACK, sizeof(msgACK) );
			return TRUE;
		}
		break;
	case MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN::SYNC_WITH_SSN:
		{
			if( !bSizeMAX )	return FALSE;

			// ������->������� (��� �ʱ�ȭ) ��ȯ�� ���� ����
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
				// DB�� ��� �����Ų��.
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_INVENTORY;
				msgSYN.Set( rPROTECT.GetPassword() );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// ������ ������ ���� �ڵ带 �����ؾ� �Ѵ�.
				//msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN Ʋ���� ��� ���� ��û�Ѵ�
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
	// [����] ~PWD_CHECK_SYN ��Ŷ��
	//   ������(�������, �ӽ����)���� ������� ��û�ø� �ش�������� ����Ѵ�.
	// ũ�� �˻�
	BOOL bSizeMIN = (BOOL)(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::MINSIZE() == wSize);
	BOOL bSizeMAX = (BOOL)((WORD)sizeof(MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN) == wSize);
	if( !(bSizeMAX || bSizeMIN) )
		return FALSE;

	ProtectorInventory& rPROTECT = INVENTORY();

	MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::eREQ_CODE eREQ = (MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::eREQ_CODE)pMsg->ReqCode();
	MSG_CG_ITEM_INVENTORY_PWD_CHECK_NAK msgNAK;

	// [����] �տ��� �������, �����¿��� �Ѵ�.
	// [����] ����� ���´ٴ� ���� ����� ����� �Ѵ�.
	BOOL bUsable = rPROTECT.IsUsable();
	BOOL bValidStatus = ( !bUsable && !rPROTECT.IsEmptyPassword() );
	if( !bValidStatus )
	{
		USER().SendPacket( &msgNAK, sizeof(msgNAK) );
		return FALSE;
	}

	if( MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN::SYNC_FIRST != eREQ )
	{	// ó�� ��Ŷ�� �ƴ� �̻� ��� Ʈ�������� �ɷ��־�� �Ѵ�.
		// �Ʒ��� ���Ǵ� ������ ����. Transaction�� �ɼ� �ִ°�? TRUE���, �� �� �ִٴ� �ǹ��̸�,
		// ���� ���� Transaction�� �ɷ����� �ʴ� ��Ȳ�� �ȴ�.
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

			// ���� ������ �����Ѵٴ� ���� �̹� ��� ���� Ʈ�������� �������� ���´�.
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

			// ��� ���� Ʈ������ ���̾�� �Ѵ�.
			//if( !rPROTECT.TEST_Transaction_PWD_CONFIG( TRUE ) )
			//{
			//	msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
			//	goto PROCESS_CHECK_END;
			//}

			// ��� �ʱ�ȭ ������ ó���� �����ؾ� �Ѵ�.
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

			// ������->�������� ��ȯ�� ���� ����
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_PWD != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( rPROTECT.IsEqualPassword( tmpBuffer.GetBUFFER() ) )
			{	// �������·� ��ȯ
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				rPROTECT.Block( FALSE );
				USER().SendPacket( &msgACK, sizeof(msgACK) );
				return TRUE;
			}
			else
			{	// ��� Ʋ���� ��� ���� ��û�Ѵ�
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

			// ������->������� (��� �ʱ�ȭ) ��ȯ�� ���� ����
			if( sPACKET_PROTECT_BASE_INFO::CHECK_CMP_SSN != rPROTECT.PasswordTransactionMode() )
			{
				msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_FAIL;
				goto PROCESS_CHECK_END;
			}
			tmpBuffer.CopyFrom( strRD );
			tmpBuffer.MakeSafeString();

			if( IsEqualSSN( (BYTE*)tmpBuffer.GetBUFFER() ) )
			{	// �������·� ��ȯ
				rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::APPLY_TRANSACTION;

				// <WARINING> DB�� ���� ��ȯ��Ű��, ��� �ʱ�ȭ ���Ѿ� �Ѵ�.DB&LOCAL
				MSG_DG_WAREHOUSE_PWD_SETTING_SYN msgSYN;
				msgSYN.m_dwKey = pMsg->m_dwKey;
				msgSYN.GetTypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_INVENTORY;
				CHAR emptyChar[1] = { 0 };
				msgSYN.Set( emptyChar );
				g_pGameServer->SendToServer( GAME_DBPROXY, &msgSYN, sizeof(msgSYN) );
				return TRUE;

				// ������ ������ ���� ��Ŷ�� �����ؾ� �Ѵ�.
				//msgACK.ReturnCode() = MSG_CG_ITEM_INVENTORY_PWD_CHECK_ACK::RET_SUCCESS;
				//rPROTECT.PasswordTransactionMode() = sPACKET_PROTECT_BASE_INFO::CHECK_END;
				//USER().SendPacket( &msgACK, sizeof(msgACK) );
				//return TRUE;
			}
			else
			{	// SSN Ʋ���� ��� ���� ��û�Ѵ�
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



