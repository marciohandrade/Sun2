#pragma once


#include "ClientSingleton.h"

class cCooltimeManager
    : public Client::Singleton<cCooltimeManager>
{
private:
    util::SolarHashTable<util::Timer*>*	m_pSkillCoolTimeHash;
    util::SolarHashTable<util::Timer*>*	m_pItemCoolTimeHash;

public:
    cCooltimeManager();

    void Initialize();
    void Release();

    void InitializeCooltimeTable();

private:
    // Skill Cool Timer
    void InitializeSkillCooltimeTable( BOOL bPoolUse, WORD wCoolTimeMagin = 0 );
    void ReleaseSkillCooltimeTable( BOOL bPoolUse );
    void InitializeItemCooltimeTable( BOOL bPoolUse, DWORD dwCoolTimeMagin = 0 );
    void ReleaseItemCooltimeTable( BOOL bPoolUse );

public:
    void SetSkillCoolTime( SLOTCODE SkillCode, WORD wCoolTimeMagin = 0 );
    void FreeSkillCoolTime( SLOTCODE SkillCode );

    void ResetSkillCooltime();
    void ResetItemCooltime();
    void ResetSkillCooltime( SLOTCODE SkillCode );

    util::Timer* GetSkillCooltime( SLOTCODE SkillCode ) { return m_pSkillCoolTimeHash->GetData( SkillCode ); };
    util::Timer* GetItemCooltime( WORD wType ) { return m_pItemCoolTimeHash->GetData( wType ); };

    void StartSkillCooltime( SLOTCODE SkillCode, int CooltimeBonusTime, int CooltimeBonusRatio);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void StartItemCooltime( WORD wType, int CooltimeBunusTime, int CooltimeBonusRatio);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION


public:
    void RestoreSkillCooltime(SkillCoolTime& skill_cool_time);

};

#define xGetCooltimeManager()  cCooltimeManager::Instance()