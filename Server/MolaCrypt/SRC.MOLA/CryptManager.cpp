
#include "stdafx.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// <MODULE VARIABLs>
BYTE	CryptManager::ms_DecryptStream[256] =
{
#include "Macros/SortIncrement.h"
};


#ifdef __MOLA_CLIENT_DEBUGGING__
HANDLE	shFILE = INVALID_HANDLE_VALUE;
DWORD	sdwLOGINC = 0;
#endif //

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// 서버에서 사용하는 함수...
DWORD	CryptManager::GetSecureFaultAcceptSize()
{
	//const DWORD cdwSIZEofSNKnCRC = (sizeof(BYTE)*2);
	//const DWORD cdwSIZEofPADDING = (DWORD_KEY_1_GENERATOR_004);
	return (DWORD)sizeof(MSGINTER_CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK) - sizeof(DWORD);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
sCLIENT_INIT_PRE_ENTRY::sCLIENT_INIT_PRE_ENTRY( WzGResTable* pGResTable )
{
	if( pGResTable )
	{
		pGResTable->Register();
		CryptManager::InitSecureInfoParser();
	}
}
//
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
BOOL	CryptManager::StartLogic( BOOL bset )
{
	//ASSERT( m_pCryptMod != NULL );
	if( !m_pCryptMod )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	pHSEL->SetInit( bset );
	m_Inited = TRUE;
	return TRUE;
}

DWORD	CryptManager::GetMolaProtocolCRC()
{
	return MOLA_PROTOCOL_CRC;
}
