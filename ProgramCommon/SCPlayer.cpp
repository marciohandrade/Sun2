#include "stdafx.h"

#include <Struct.h>

#include ".\SCPlayer.h"

#include <ItemInfoParser.h>
#include <CoolTimerFactory.h>

#if defined(_SERVER)

__EMPTY_FILE_TAG();

#else //if !defined(_SERVER)
// CLIENT

SCPlayer::SCPlayer() : m_pItemCoolTimeHash(NULL)
{
	m_pCharInfo	= new BASE_PLAYERINFO;

	ITEM_COOLTIME_MAP* pCoolTimeMap = ItemInfoParser::Instance()->GetItemCoolTimeMap();
	m_pItemCoolTimeHash = new PLAYER_ITEM_COOL_TIMER_HASH;
	m_pItemCoolTimeHash->Initialize( (DWORD)pCoolTimeMap->size() );
}

SCPlayer::~SCPlayer()
{
	SAFE_DELETE( m_pCharInfo );
	SAFE_DELETE( m_pItemCoolTimeHash );
}

VOID SCPlayer::SetCharInfo( const BASE_PLAYERINFO* pCharInfo )
{
	memcpy( m_pCharInfo, pCharInfo, sizeof(BASE_PLAYERINFO) );

	//m_dwWeaponKind = eWEAPONTYPE_PUNCH;

	OnSetCharInfo();
}

eCHAR_TYPE	SCPlayer::GetCharType() const			
{ 
	return (eCHAR_TYPE)SCPlayer::GetCharInfo()->m_byClassCode; 
}

LEVELTYPE	SCPlayer::GetLevel() const			
{ 
	return SCPlayer::GetCharInfo()->m_LV; 
}


VOID SCPlayer::SetItemCoolTime( BOOL bPoolUse, DWORD dwCoolTimeMagin )
{
	// 쿨타임 타이머 해쉬 테이블 생성
	ITEM_COOLTIME_MAP* pCoolTimeMap = ItemInfoParser::Instance()->GetItemCoolTimeMap();

	// 스킬별로 타이머를 생성한 후 해쉬테이블에 추가한다.
	ITEM_COOLTIME_ITER it;
	for( it = pCoolTimeMap->begin(); it != pCoolTimeMap->end(); ++it )
	{
		ITEM_COOLTIME_INFO CoolTimeInfo = (ITEM_COOLTIME_INFO)it->second;

		util::Timer * pTimer = NULL;
		if( bPoolUse )
		{
			pTimer = CoolTimerFactory::Instance()->Alloc();
		}
		else
		{
			pTimer = new util::Timer;
		}
		pTimer->SetTimer( CoolTimeInfo.m_dwCoolTime - dwCoolTimeMagin );
		pTimer->InitCoolTime();
		m_pItemCoolTimeHash->Add( pTimer, CoolTimeInfo.m_wCoolTimeType );
	}
}

void SCPlayer::FreeItemCoolTime(BOOL pool_used)
{
    if (m_pItemCoolTimeHash)
    {
        CoolTimerFactory* timer_factory = CoolTimerFactory::Instance();
        m_pItemCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pItemCoolTimeHash->GetNext())
        {
            if (pool_used)
            {
                timer_factory->Free(timer);
            }
            else
            {
                SAFE_DELETE(timer);
            }
        }
        m_pItemCoolTimeHash->RemoveAll();
    }

}

VOID SCPlayer::InitializeItemCooltime()
{
    if (m_pItemCoolTimeHash)
    {
        m_pItemCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pItemCoolTimeHash->GetNext()) {
            timer->InitCoolTime();
        }
    }
}

#endif //!defined(_SERVER)



