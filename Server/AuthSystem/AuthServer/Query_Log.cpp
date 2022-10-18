#include "stdafx.h"
#include "Query_Log.h"
#include "AccountManager.h"


VOID QueryAuthLog::onResult()
{
	QueryObject::onResult();

	if( m_btLogout )
	{
		return;
	}

	AccountInfo* pInfo = AccountManager::Instance()->Find( m_account );

	if( pInfo == 0 )
	{
		SUNLOG( eFULL_LOG, "[QueryAuthLog::onResult] Cannot find account info : %s\n", m_account );
		return;
	}
    
    if (m_btLogout == 0) //로그인 로그를 남겼다면 ...
    {
        pInfo->m_IsLoginLogWrite = true;
    }
    else
    {
        pInfo->m_IsLoginLogWrite = false;
    }

	pInfo->m_dwLogSeq = m_dwLogSeq;
}

