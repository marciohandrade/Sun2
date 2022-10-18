#include "SunClientPrecompiledHeader.h"
#include "CooltimeManager.h"
#include "SkillInfoParser.h"
#include "ItemInfoParser.h"
#include <../ProgramCommon/CoolTimerFactory.h>
#include "Hero.h"

#ifdef _DEV_VER
#include "GlobalFunc.h"
#endif


cCooltimeManager::cCooltimeManager()
{

}

void cCooltimeManager::Initialize()
{
    m_pSkillCoolTimeHash = new util::SolarHashTable<util::Timer *>;
    m_pItemCoolTimeHash = new util::SolarHashTable<util::Timer *>;
}

void cCooltimeManager::Release()
{
    ReleaseSkillCooltimeTable(FALSE);
    ReleaseItemCooltimeTable(FALSE);

    delete m_pSkillCoolTimeHash;
    delete m_pItemCoolTimeHash;
    m_pSkillCoolTimeHash = NULL;
    m_pItemCoolTimeHash = NULL;
}

void cCooltimeManager::InitializeCooltimeTable()
{
    InitializeSkillCooltimeTable( FALSE, 0 );
    InitializeItemCooltimeTable( FALSE, 0 );
}

void cCooltimeManager::SetSkillCoolTime( SLOTCODE SkillCode, WORD wCoolTimeMagin )
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

    SkillScriptInfo* pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );

    util::Timer* pTimer = GetSkillCooltime( SkillCode );
    if( pTimer )
        return; 

    pTimer = CoolTimerFactory::Instance()->Alloc();

    pTimer->SetTimer( pBaseSkillInfo->m_dwCoolTime - wCoolTimeMagin );
    pTimer->InitCoolTime();
    m_pSkillCoolTimeHash->Add( pTimer, SkillCode );
}

void cCooltimeManager::FreeSkillCoolTime( SLOTCODE SkillCode )
{
    util::Timer* pTimer = GetSkillCooltime( SkillCode );
    if( pTimer == NULL )
        return; 

    CoolTimerFactory::Instance()->Free( pTimer );
    m_pSkillCoolTimeHash->Remove( SkillCode );
}

void cCooltimeManager::InitializeSkillCooltimeTable( BOOL bPoolUse, WORD wCoolTimeMagin )
{
    // 쿨타임 타이머 해쉬 테이블 생성
    COOLTIME_MAP* pCoolTimeMap = SkillInfoParser::Instance()->GetSkillCoolTimeMap();
    if( pCoolTimeMap == NULL )
    {
        return;
    }

    m_pSkillCoolTimeHash->Initialize( (DWORD)pCoolTimeMap->size() );

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

void cCooltimeManager::ReleaseSkillCooltimeTable(BOOL pool_used)
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

void cCooltimeManager::ResetSkillCooltime( SLOTCODE SkillCode )
{
    if( util::Timer* pTimer = xGetCooltimeManager()->GetSkillCooltime( SkillCode ) )
    {
        pTimer->InitCoolTime(clock_function::GetTickCount());
    }
}

void cCooltimeManager::ResetSkillCooltime()
{
    if (m_pSkillCoolTimeHash)
    {
        m_pSkillCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pSkillCoolTimeHash->GetNext()) 
        {
            timer->InitCoolTime();
        }
    };
}

void cCooltimeManager::InitializeItemCooltimeTable( BOOL bPoolUse, DWORD dwCoolTimeMagin )
{
    // 쿨타임 타이머 해쉬 테이블 생성
    ITEM_COOLTIME_MAP* pCoolTimeMap = ItemInfoParser::Instance()->GetItemCoolTimeMap();
    if( pCoolTimeMap == NULL )
    {
        return;
    }

    m_pItemCoolTimeHash->Initialize( (DWORD)pCoolTimeMap->size() );

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

void cCooltimeManager::ReleaseItemCooltimeTable(BOOL pool_used)
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

void cCooltimeManager::ResetItemCooltime()
{
    if (m_pItemCoolTimeHash)
    {
        m_pItemCoolTimeHash->SetFirst();
        while (util::Timer* timer = m_pItemCoolTimeHash->GetNext()) 
        {
            timer->InitCoolTime();
        }
    }
}

void cCooltimeManager::RestoreSkillCooltime(SkillCoolTime& skill_cool_time)
{
    ResetSkillCooltime();

    for (int i = 0; i < skill_cool_time.count; ++i)
    {
        if( util::Timer* cool_time = GetSkillCooltime(skill_cool_time.info[i].skill_code) )
        {
            cool_time->InitCoolTime(clock_function::GetTickCount() + skill_cool_time.info[i].remain_second);
        }
    }
}

void cCooltimeManager::StartSkillCooltime( SLOTCODE SkillCode, int CooltimeBonus, int CooltimeBonusRatio )
{
#ifdef _DEV_VER
    if( CGeneralGameParam::Instance()->IsEnableAutoCooltimeReset() == true )
    {
        GlobalFunc::SendGMCommand("쿨타임");
        return;
    }
#endif

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if( SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(SkillCode) )
    {
#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
        // 토글 스킬 패킷펜딩
        if (pSkillInfo->IsToggleEffectSkill() == true ||
            pSkillInfo->IsToggleSkill() == true ||
            pSkillInfo->IsAuraSkill() == true)
        {
            g_pHero->ToggleSkill::UnsetPacketPending();
        }
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING


        // 토글 스킬은 쿨타임을 공유하지 않는다.
        if( pSkillInfo->IsAuraSkill() == true )
        {
            std::vector<SLOTCODE> Array;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            g_pHero->GetToggleSkillCodeArray(Array);
#else
            g_pHero->GetAuraSkillCodeArray(Array);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            for( size_t i=0;i!=Array.size();++i)
            {
                if( util::Timer* pTimer = GetSkillCooltime(Array[i]) )
                {
                    int iSkillCoolTime = pTimer->GetBaseIntervalTime();
                    if( iSkillCoolTime > 0 )
                    {
                        int iBonusCoolTime = CooltimeBonus;
                        float fBonusCoolTimeRatio = CooltimeBonusRatio / 100.f;
                        fBonusCoolTimeRatio = max( -1.0f, fBonusCoolTimeRatio );

                        iSkillCoolTime = iSkillCoolTime + iBonusCoolTime;
                        iSkillCoolTime = max( 0, iSkillCoolTime );
                        iSkillCoolTime = (int)( iSkillCoolTime * ( 1 + fBonusCoolTimeRatio ) );

                        int iBonusTime = iSkillCoolTime - pTimer->GetBaseIntervalTime();
                        pTimer->SetBonusIntervalTime( iBonusTime );
                    }

                    pTimer->Reset();
                }
            }
            return;
        }
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

    if( util::Timer* pTimer = GetSkillCooltime(SkillCode) )
    {
        int iSkillCoolTime = pTimer->GetBaseIntervalTime();
        if( iSkillCoolTime > 0 )
        {
            int iBonusCoolTime = CooltimeBonus;
            float fBonusCoolTimeRatio = CooltimeBonusRatio / 100.f;
            fBonusCoolTimeRatio = max( -1.0f, fBonusCoolTimeRatio );

            iSkillCoolTime = iSkillCoolTime + iBonusCoolTime;
            iSkillCoolTime = max( 0, iSkillCoolTime );
            iSkillCoolTime = (int)( iSkillCoolTime * ( 1 + fBonusCoolTimeRatio ) );

            int iBonusTime = iSkillCoolTime - pTimer->GetBaseIntervalTime();
            pTimer->SetBonusIntervalTime( iBonusTime );
        }

        pTimer->Reset();
    }
}
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void cCooltimeManager::StartItemCooltime( WORD wType, int CooltimeBonus, int CooltimeBonusRatio )
{
#ifdef _DEV_VER
    if( CGeneralGameParam::Instance()->IsEnableAutoCooltimeReset() == true )
    {
        GlobalFunc::SendGMCommand("쿨타임");
        return;
    }
#endif
    
    if( util::Timer* pTimer = GetItemCooltime(wType) )
    {
        int iItemCoolTime = pTimer->GetBaseIntervalTime();
        if( iItemCoolTime > 0 )
        {
            int iBonusCoolTime = CooltimeBonus;
            float fBonusCoolTimeRatio = CooltimeBonusRatio / 100.f;
            fBonusCoolTimeRatio = max( -1.0f, fBonusCoolTimeRatio );

            iItemCoolTime = iItemCoolTime + iBonusCoolTime;
            iItemCoolTime = max( 0, iItemCoolTime );
            iItemCoolTime = (int)( iItemCoolTime * ( 1 + fBonusCoolTimeRatio ) );

            int iBonusTime = iItemCoolTime - pTimer->GetBaseIntervalTime();
            pTimer->SetBonusIntervalTime( iBonusTime );
        }

        pTimer->Reset();
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION