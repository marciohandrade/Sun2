#include "stdafx.h"
#include "SCCharacter.h"

#if defined(_SERVER)

__EMPTY_FILE_TAG();

#else //if !defined(_SERVER)
// CLIENT

#include <SkillInfoParser.h>
#include <CoolTimerFactory.h>


SCCharacter::SCCharacter()
{
	m_pSkillCoolTimeHash = new util::SolarHashTable<util::Timer *>;
}

SCCharacter::~SCCharacter()
{
	SAFE_DELETE( m_pSkillCoolTimeHash );
}

VOID SCCharacter::InitializeHashTable( DWORD dwTimerMaxNum )
{
    m_pSkillCoolTimeHash->Initialize( dwTimerMaxNum );
}

VOID SCCharacter::SetCoolTime( SLOTCODE SkillCode, WORD wCoolTimeMagin )
{
	ROOT_SKILLINFO *pRootInfo = SkillInfoParser::Instance()->GetInfo( SkillCode );
	if( !pRootInfo ) 
	{
		if( SkillCode )	SUNLOG( eCRITICAL_LOG, "[SetCoolTime] Skill[%d] doesn't exist \n", SkillCode );
		return;
	}
	else if( pRootInfo->IsStyle() )
	{
		return;
	}

	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );

	util::Timer * pTimer = GetCoolTimer( SkillCode );
	if( pTimer )	return; 

	pTimer = CoolTimerFactory::Instance()->Alloc();

	pTimer->SetTimer( pBaseSkillInfo->m_dwCoolTime - wCoolTimeMagin );
	pTimer->InitCoolTime();
	m_pSkillCoolTimeHash->Add( pTimer, SkillCode );
}

VOID SCCharacter::FreeCoolTime( SLOTCODE SkillCode )
{
	util::Timer * pTimer = GetCoolTimer( SkillCode );
	if( !pTimer )	return; 

	CoolTimerFactory::Instance()->Free( pTimer );
	m_pSkillCoolTimeHash->Remove( SkillCode );
}

VOID SCCharacter::SetAllCoolTime( BOOL bPoolUse, WORD wCoolTimeMagin )
{
	// 쿨타임 타이머 해쉬 테이블 생성
	COOLTIME_MAP* pCoolTimeMap = SkillInfoParser::Instance()->GetSkillCoolTimeMap();

	// 스킬별로 타이머를 생성한 후 해쉬테이블에 추가한다.
	COOLTIME_ITER it;
	for( it = pCoolTimeMap->begin(); it != pCoolTimeMap->end(); ++it )
	{
		SKILL_COOLTIME_INFO CoolTimeInfo = (SKILL_COOLTIME_INFO)it->second;

		util::Timer * pTimer = NULL;
		if( bPoolUse )
		{
			pTimer = CoolTimerFactory::Instance()->Alloc();
		}
		else
		{
			pTimer = new util::Timer;
		}

		pTimer->SetTimer( CoolTimeInfo.m_dwCoolTime - wCoolTimeMagin );
		pTimer->InitCoolTime();
		m_pSkillCoolTimeHash->Add( pTimer, CoolTimeInfo.m_SkillCode );
	}
}

void SCCharacter::FreeAllCoolTime(BOOL pool_used)
{
    // CoolTime Timer 삭제
    if (m_pSkillCoolTimeHash)
    {
        CoolTimerFactory* timer_factory = CoolTimerFactory::Instance();
        m_pSkillCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pSkillCoolTimeHash->GetNext())
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
        m_pSkillCoolTimeHash->RemoveAll();
    }
}

void SCCharacter::InitializeCooltime()
{
    if (m_pSkillCoolTimeHash)
    {
        m_pSkillCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pSkillCoolTimeHash->GetNext()) {
            timer->InitCoolTime();
        }
    };
}

#endif //!defined(_SERVER)
