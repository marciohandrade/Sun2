#include "SunClientPrecompiledHeader.h"

#include "triggermanex.h"

#include <TriggerGroupInfo.h>
#include <TriggerInfoManager.h>
#include <IActionTrigger.h>
#include <IConditionTrigger.h>
#include <TriggerCommon.h>
#include "Trigger.h"

#include "ObjectManager.h"
#include "MapObject.h"

TriggerManEx::TriggerManEx(void)
{
}

TriggerManEx::~TriggerManEx(void)
{
}

VOID		TriggerManEx::Init( TriggerGroupInfo& IN rTriggerGroupInfo)
{
	ASSERT( GetTriggerCount() == 0 );

	// 트리거 정보 참조
	const TRIGGER_INFO_LIST& mapTriggerInfo = rTriggerGroupInfo.GetTriggerInfos();
    TRIGGER_INFO_LIST::const_iterator iter = mapTriggerInfo.begin();
    const TRIGGER_INFO_LIST::const_iterator end = mapTriggerInfo.end();

	for (; iter != end; ++iter)
	{
		TriggerInfo* pTriggerInfo = *iter;

		if( !pTriggerInfo )
		{
			ASSERT( NULL );
			continue;
		}

		Trigger* pTrigger = new Trigger;
		pTrigger->Init( (TriggerManager*)this, pTriggerInfo );
		pTrigger->OnInit();

		if( !InsertTrigger( pTrigger ) )
		{
			delete pTrigger;
			assert(!"TriggerManEx::Init");
			continue;
		}
	}
}

VOID		TriggerManEx::Release()
{
	TriggerManager::Release();
	int iSize = (int)m_vecTrgObj.size();
	for(int i = 0; i < iSize; i++)
	{
		SAFE_DELETE(m_vecTrgObj[i]);
	}
	m_vecTrgObj.clear();
}

VOID		TriggerManEx::OnMsg( TRIGGER_MSG * pMsg, BOOL bDone)
{
    TRIGGER_LIST::iterator iter;
	for( iter = m_listTrigger.begin(); iter !=m_listTrigger.end(); ++iter )
	{
		Trigger* pTrigger = static_cast<Trigger*>( iter->second );

		if( pTrigger )
		{
			pTrigger->OnMsg( pMsg );
		}
	}
}
